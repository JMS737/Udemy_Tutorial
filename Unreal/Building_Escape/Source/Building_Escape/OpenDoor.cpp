// Copyright Jeremy Sapsed 2021

#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Initialise the target position based on the current yaw.
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' is missing a PressurePlate on the OpenDoor component"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	Audio = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!Audio)
	{
		UE_LOG(LogTemp, Warning, TEXT("No AudioComponent found on '%s'"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() >= RequiredMass)
	{
		OpenDoor(DeltaTime);
	}
	else if (GetWorld()->GetTimeSeconds() - LastOpened > CloseDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, OpenSpeed);
	SetDoorRotation(CurrentYaw);
	LastOpened = GetWorld()->GetTimeSeconds();

	if (!AudioTriggeredOpen)
	{
		AudioTriggeredOpen = true;
		AudioTriggeredClose = false;
		Audio->Play();
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, CloseSpeed);
	SetDoorRotation(CurrentYaw);

	if (!AudioTriggeredClose)
	{
		AudioTriggeredClose = true;
		AudioTriggeredOpen = false;
		Audio->Play();
	}
}

void UOpenDoor::SetDoorRotation(float angle)
{
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = angle;
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (!PressurePlate)
	{
		return TotalMass;
	}

	TArray<AActor *> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}