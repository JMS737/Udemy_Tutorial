// Copyright Jeremy Sapsed 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY() // Should be included on all components (classes beginning with U) so that Unreal can clean up components properly.
	UAudioComponent* Audio = nullptr;

	UPROPERTY(EditAnywhere)
	float RequiredMass = 50.f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float CloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed = 4.f;

	float InitialYaw;
	float CurrentYaw;
	float LastOpened = 0.f;
	bool AudioTriggeredOpen = false;
	bool AudioTriggeredClose = true;

	void SetDoorRotation(float angle);
	void FindPressurePlate();
	void FindAudioComponent();
};
