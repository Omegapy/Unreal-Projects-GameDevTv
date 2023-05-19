// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	
}

// Called every frame
// DeltaTime tells how long a frame takes to execute, makes the game frame rate independent, for example velocity for moving object
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	// Send platform back if gone too far
	if (ShouldPlatFormReturn()) 
	{
		// Shows message in editor output log console
		//float OverShoot = DistanceMoved - MaxDistanceTraveled;
		//FString Name = GetName(); // FString unreal string, convention F means it is a struc and String is a string, Note A means it is an actor
		//UE_LOG(LogTemp, Display, TEXT("The starting point of the actor %s is overshoot by %f"), *Name, OverShoot);
		
		// If platform is moving very fast it can overshoot the StartLocation, the DistanceMoved is significant greater than MaxDistanceTraveled
		// it is depending of the DeltaTime, in other words, the consequence is that the original starting point can drift over time
		// GetSafeNormal() It returns a normalised vector. It’s a vector going in the same direction but with a magnitude of 1 i.e. it only has direction (-1, 1).
		// The ‘Safe’ in GetSafeNormal just means it protects against a potential divide-by-zero.
		FVector MovedDirection = PlatformVelocity.GetSafeNormal();
		StartLocation = StartLocation + MovedDirection * MaxDistanceTraveled;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
		} else // Move platform fowards
		{
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation = CurrentLocation + PlatformVelocity * DeltaTime; // DeltaTime tells how long a frame takes to execute, makes the game frame rate independent, for example velocity for moving object
			SetActorLocation(CurrentLocation);
		}
	
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{

	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AMovingPlatform::ShouldPlatFormReturn() const
{
	return DistanceMoved() > MaxDistanceTraveled;
	
}

float AMovingPlatform::DistanceMoved() const
{
	return FVector:: Dist(StartLocation, GetActorLocation());
}