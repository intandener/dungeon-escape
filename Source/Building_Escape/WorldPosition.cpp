// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldPosition.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"


// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	const FString ActorName = GetOwner()->GetName();
	const FVector ActorPos = GetOwner()->GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("Hi from UWorldPosition in the Actor: %s"), *ActorName);
	UE_LOG(LogTemp, Warning, TEXT("%s position is %s"), *ActorName, *ActorPos.ToCompactString());
	
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

