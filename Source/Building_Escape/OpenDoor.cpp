// Copyright Stefan Zaruba 2021

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"


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

	ClosedYaw = FRotator::ClampAxis(GetOwner()->GetActorRotation().Yaw);
	OpenedYaw = ClosedYaw + MaxAngle;
	CurrentYaw = ClosedYaw;

	UE_LOG(LogTemp, Warning, TEXT("Closed Yaw: %f, OpenedYaw: %f, Actor Yaw: %f"), ClosedYaw, OpenedYaw, GetOwner()->GetActorRotation().Yaw);

	if (OpenTrigger == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("The Actor %s has an OpenDoor Component with no OpenTrigger assigned! Assign it!"), *(GetOwner()->GetName()));
	}

	OpeningActor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SecondsPassed += DeltaTime;
	
	if (OpenTrigger && OpenTrigger->IsOverlappingActor(OpeningActor))
	{
		OpenDoor();
	}
	else if (DoorState == EDoorState::CLOSE_AFTER_DELAY)
	{
		if (GetWorld()->GetTimeSeconds() >= CloseAtSeconds)
		{
			CloseDoor();
		}
	}
	else
	{
		if (DoorState == EDoorState::FULLY_OPEN)
		{
			DoorState = EDoorState::CLOSE_AFTER_DELAY;
			CloseAtSeconds = GetWorld()->GetTimeSeconds() + CloseDelaySeconds;
		}
		else
		{
			CloseDoor();
		}
	}

	if (DoorState == EDoorState::OPENING || DoorState == EDoorState::CLOSING)
	{
		if (DoorState == EDoorState::OPENING)
		{
			CurrentYaw += RotationSpeed * DeltaTime;

			if (CurrentYaw > OpenedYaw)
			{
				CurrentYaw = OpenedYaw;
				DoorState = EDoorState::FULLY_OPEN;
			}
		}
		else
		{
			CurrentYaw -= RotationSpeed * DeltaTime;

			if (CurrentYaw < ClosedYaw)
			{
				CurrentYaw = ClosedYaw;
				DoorState = EDoorState::CLOSED;
			}
		}
		
		FRotator ActorRotation = GetOwner()->GetActorRotation();
		ActorRotation.Yaw = CurrentYaw;
		GetOwner()->SetActorRotation(ActorRotation);

		UE_LOG(LogTemp, Display, TEXT("%f: Current Yaw: %f, ActorRotation.Yaw: %f, Actor Yaw: %f"), SecondsPassed, CurrentYaw, ActorRotation.Yaw, GetOwner()->GetActorRotation().Yaw);
	}
}

void UOpenDoor::OpenDoor()
{
	if (DoorState == EDoorState::CLOSED || DoorState == EDoorState::CLOSING)
	{
		DoorState = EDoorState::OPENING;
	}
	else if (DoorState == EDoorState::CLOSE_AFTER_DELAY)
	{
		DoorState = EDoorState::FULLY_OPEN;
	}
}

void UOpenDoor::CloseDoor()
{
	if (DoorState == EDoorState::FULLY_OPEN || DoorState == EDoorState::OPENING || DoorState == EDoorState::CLOSE_AFTER_DELAY)
	{
		DoorState = EDoorState::CLOSING;
	}
}
