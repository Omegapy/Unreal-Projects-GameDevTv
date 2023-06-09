// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
// #include "DrawDebugHelpers.h"

// --- Contructor
ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Carmera"));
	Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// Set the Heath of the Tank
	FindComponentByClass<UHealthComponent>()->SetHealth(MaxHealth);

	// Casting AControler* into APlayerController*, APlayerController class is a child class of the AControler class 
	TankPlayerController = Cast<APlayerController>(GetController());

	if (TankEngine)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TankEngine, GetActorLocation());
	}

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility, // Trace channel, enum
			false,
			HitResult);

		//DrawDebugSphere(
		//	GetWorld(), // Map
		//	HitResult.ImpactPoint,
		//	25.f, // Radius
		//	12, // Number of segment lines to draw the sphere
		//	FColor::Red,
		//	false, // Drawn sphere permanently
		//	-1.f); // Drawn sphere duration in second, -1 is every frame

		RotateTurret(HitResult.ImpactPoint);
	}

}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	// Hidding take after health=0, so we can still get the view from the camera
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	bAlive = false;
}

void ATank::Move(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	// Yaw = Value * DeltaTime * TurnRate
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

float ATank::GetTankHealth()
{
	return FindComponentByClass<UHealthComponent>()->GetHealth();
}

