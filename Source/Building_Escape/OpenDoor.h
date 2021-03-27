// Copyright Stefan Zaruba 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


enum class EDoorState : uint8
{
	FULLY_OPEN = 1, CLOSED = 2, OPENING = 3, CLOSING = 4, CLOSE_AFTER_DELAY = 5
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor();
	void CloseDoor();

private:
	// Degrees per second
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	float MaxAngle = 90.f;

	UPROPERTY(EditAnywhere)
	bool bClockwiseOpening = true;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* OpenTrigger;

	UPROPERTY(EditAnywhere)
	AActor* OpeningActor;

	EDoorState DoorState = EDoorState::CLOSED;
	
	float ClosedYaw;
	float OpenedYaw;
	float CurrentYaw;

	float SecondsPassed = 0.f;
	float CloseAtSeconds;

	UPROPERTY(EditAnywhere)
	float CloseDelaySeconds = 2.f;
};

