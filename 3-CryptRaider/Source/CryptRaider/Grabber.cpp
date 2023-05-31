// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//-- Debugging Physic Handle
	// Gets hold of UPhysicallHandlerComponent, it uses header #include "PhysicsEngine/PhysicsHandleComponent.h"
	GetPhysicsHandle();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//--- Debugging 

	//-- References example
	//float Damage = 0;
	//float& DamageRef = Damage;
	//DamageRef = 5;
	//UE_LOG(LogTemp, Display, TEXT("DamageRef: %f, Damage: %f"), DamageRef, Damage);

	/*FRotator MyRotation = GetComponentRotation();
	FString RotationString = MyRotation.ToCompactString();
	UE_LOG(LogTemp, Display, TEXT("The Graber's Rotation: %s"), *RotationString);*/

	//UWorld* World = GetWorld();
	//float Time = World->TimeSeconds;
	//float Time = GetWorld()->TimeSeconds; // GetWorld() uses header #include "Engine/World.h"
	//UE_LOG(LogTemp, Display, TEXT("Current Time Is: %f"), Time);

	// Physics Handle
	// GetPhysicsHandle() Gets hold of UPhysicallHandlerComponent, it uses header #include "PhysicsEngine/PhysicsHandleComponent.h"
	// Make sure that the Actor being grabbed, details in the editor are set to movable and simulate physics is enable
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	//if (PhysicsHandle == nullptr)
	//{
	//	return;
	//}
	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

}

void UGrabber::Grab()
{
	// Physics Handle
	// Gets hold of UPhysicallHandlerComponent, it uses header #include "PhysicsEngine/PhysicsHandleComponent.h"
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (HasHit)
	{

		
		////-- Debugging hit result
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5); // It is the location where the character player should be to grab the object 100cm away
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Cyan, false, 5);
		//AActor* HitActor = HitResult.GetActor();
		//UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// Physics Handle
		// GetPhysicsHandle() Gets hold of UPhysicallHandlerComponent, it uses header #include "PhysicsEngine/PhysicsHandleComponent.h"
		// Make sure that the Actor being grabbed, details in the editor are set to movable and simulate physics is enable
		GetPhysicsHandle()->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None, 
			HitResult.ImpactPoint,
			GetComponentRotation()
		);

		
	}
	else
	{
		////-- Debugging Grab
		// UE_LOG(LogTemp, Display, TEXT("No actor hit"));

		//PhysicsHandle->GrabComponentAtLocationWithRotation(
		//	HitResult.GetComponent(),
		//	NAME_None,
		//	HitResult.ImpactPoint,
		//	GetComponentRotation()
		//);

	}
	
}
bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	//-- Drawing a debugging line and Sphere
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange);  // DrawDebugLine() uses header #include "DrawDebugHelpers.h"
	//DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);  // DrawDebugSphere() uses header #include "DrawDebugHelpers.h"

	//-- Geometry tracing, Sweeper
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel( // GetWorld() uses header #include "Engine/World.h"
		OutHitResult,
		Start, End,
		FQuat::Identity, // Math for no rotation
		// Go to the project's folder Config --> DefalutEngine.ini open with Visual Studio search for the component name (Grabber for this program)
		ECC_GameTraceChannel2, // line in the DefalutEngine.ini: +DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Ignore,bTraceType=True,bStaticObject=False,Name="Grabber")
		Sphere
	);
}

void UGrabber::Release()
{
	//-- Debugging Release
	//UE_LOG(LogTemp, Display, TEXT("Release Grabber"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	//if (PhysicsHandle == nullptr)
	//{
	//	return;
	//}

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");

		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies(); // After a well Actors go to sleep if not moved, this awakes them
		PhysicsHandle->ReleaseComponent(); // part of the handle physics
	}

}

//-- Physic Handle
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	// Gets hold of UPhysicallHandlerComponent, it uses header #include "PhysicsEngine/PhysicsHandleComponent.h"
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// Unreal Engine crash issue, checks for null pointer
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return Result;

}