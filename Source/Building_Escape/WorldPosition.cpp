// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldPosition.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"


UWorldPosition::UWorldPosition()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	const FString ActorName = GetOwner()->GetName();
	const FVector ActorPos = GetOwner()->GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("Hi from UWorldPosition in the Actor: %s"), *ActorName);
	UE_LOG(LogTemp, Warning, TEXT("%s position is %s"), *ActorName, *ActorPos.ToCompactString());
}

void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

