// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Called every frame
void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Find the distance to the Tank
    if (Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        // Check to see if the Tank is in range
        if (Distance <= FireRange)
        {
            // If in range, rotate turret toward Tank
            RotateTurret(Tank->GetActorLocation());
        }
    }
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle, 
        this, 
        &ATower::CheckFireCondition, 
        FireRate, 
        true // loop
    ); 
}

void ATower::CheckFireCondition()
{
    if (TankNotOnSight())
    {
        return;
    }
    if (InFireRange() && Tank->bAlive)
    {
        Fire();
    }
}

bool ATower::InFireRange()
{
    if (Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}

// --- Checks if the Tank is not sighted
bool ATower::TankNotOnSight() 
{
    //-- Drawing a debugging line and Sphere
    USceneComponent* Turret = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Spawn Point")));
    FVector Start = Turret->GetComponentLocation();
    FVector End = Start + Turret->GetForwardVector() * FireRange;
    
    //-- Geometry tracing, Sweeper
    FCollisionShape Sphere = FCollisionShape::MakeSphere(25.f);
    FHitResult SightHitResult;
    bool HitSomething = GetWorld()->SweepSingleByChannel( // GetWorld() uses header #include "Engine/World.h"
        SightHitResult,
        Start, End,
        FQuat(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z, 0), // Math for no rotation
        // Go to the project's folder Config --> DefalutEngine.ini open with Visual Studio search for the component name (SightTank for this program)
        ECC_GameTraceChannel1, // line in the DefalutEngine.ini: ex: +DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Ignore,bTraceType=True,bStaticObject=False,Name="SightTank")
        Sphere
    );

    if (SightHitResult.GetActor() == nullptr)
    {
        return false;
    }

    if (SightHitResult.GetActor() != Tank)
    {
        //UE_LOG(LogTemp, Warning, TEXT("---- tank not sighted -----"));
        return true;
    }
    // UE_LOG(LogTemp, Warning, TEXT("---- tank sighted -----"));
    return false;
}


