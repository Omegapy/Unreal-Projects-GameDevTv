// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverRotation.h"

// Sets default values for this component's properties
UMoverRotation::UMoverRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoverRotation::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetOwner()->GetActorRotation();
	
}


// Called every frame
void UMoverRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator TargetRotation = OriginalRotation;

	if (ShouldRotate)
	{
		TargetRotation = OriginalRotation + RotationOffset;
	}

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	float Speed = RotationOffset.Vector().Length() / MoveTime;

	FRotator NewRotation= FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
	GetOwner()->SetActorRotation(NewRotation);
}

void UMoverRotation::SetShouldRotate(bool NewShouldRotate)
{
	ShouldRotate = NewShouldRotate;
}
