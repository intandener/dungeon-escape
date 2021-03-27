// Copyright Stefan Zaruba 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "CollisionQueryParams.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has Grabber component attached, but no PhysicsHandle component. Add a PhysicsHandle component to fix the issue."), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbing!!!"));
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	if (HitResult.GetActor())
	{
		UE_LOG(LogTemp, Display, TEXT("Grabbing the actor: %s"), *HitResult.GetActor()->GetName());

		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, HitResult.ImpactPoint);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Releasing!!!"));
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;

	// Ray-cast
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(Hit, PlayerViewLocation, LineTraceEnd, ObjectParams, TraceParams);

	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the players viewport
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	// Draw line from player showing the reach
	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewLocation, LineTraceEnd, FColor::Green, false, 0.f, 0u, 5.f);

	// Alternative way to get the player's viewport
	//FVector PlayerViewLocationOwner;
	//FRotator PlayerViewRotationOwner;
	//GetOwner()->GetActorEyesViewPoint(PlayerViewLocationOwner, PlayerViewRotationOwner);

	//UE_LOG(LogTemp, Display, TEXT("WorldLocation: %s, OwnerLocation: %s, WorldRotation: %s, OwnerRotation: %s"), 
	//	*PlayerViewLocation.ToCompactString(), *PlayerViewLocationOwner.ToCompactString(), 
	//	*PlayerViewRotation.ToCompactString(), *PlayerViewRotationOwner.ToCompactString());

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

	
}

