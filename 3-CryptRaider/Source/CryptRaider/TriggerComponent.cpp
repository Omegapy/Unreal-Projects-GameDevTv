// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*TArray<AActor*> Actors;
	GetOverlappingActors(Actors);*/
	//for (int32 i = 0; i < Actors.Num(); i++)
	//{
	//	FString ActorName = Actors[i]->GetActorNameOrLabel();
	//	UE_LOG(LogTemp, Display, TEXT("Overlapping: %s"), *ActorName);
	//}
	/*for (AActor* Actor : Actors)
	{

		if (Actor->ActorHasTag(AcceptableActorTag))
		{
			UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		}

	}*/

	if (Mover == nullptr && MoverRotator1 == nullptr && MoverRotator2 == nullptr)
	{
		return;
	}
	AActor* Actor = GetAcceptableActor();
	if (Mover != nullptr)
	{
		if (Actor != nullptr)
		{
			//UE_LOG(LogTemp, Display, TEXT("Unlocking))
			UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (Component != nullptr)
			{
				Component->SetSimulatePhysics(false); // Disable Dimalated physics
			}
			// Attached actors 
			Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

			Mover->SetShouldMove(true);
		}
		else
		{
			Mover->SetShouldMove(false);
		}

		return;
	}
	
	MoverRotate(MoverRotator1, Actor);
	MoverRotate(MoverRotator2, Actor);

}

void UTriggerComponent::MoverRotate(UMoverRotation* MoverRotator, AActor* Actor)
{
	if (MoverRotator != nullptr)
	{
		if (Actor != nullptr)
		{
			UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (Component != nullptr)
			{
				Component->SetSimulatePhysics(false); // Disable Dimalated physics
			}
			// Attached actors 
			Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			MoverRotator->SetShouldRotate(true);
		}
		else
		{
			MoverRotator->SetShouldRotate(false);
		}
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

void UTriggerComponent::SetMoverRotation(UMoverRotation* NewMoverRotator1, UMoverRotation* NewMoverRotator2)
{
	MoverRotator1 = NewMoverRotator1;
	MoverRotator2 = NewMoverRotator2;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		// bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		// bool IsGrabbed = Actor->ActorHasTag("Grabbed");
		//for (FName Name: AcceptableActorTag)
		//{
			if (Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
			{
				// UE_LOG(LogTemp, Display, TEXT("Unlocking"));
				return Actor;
			}
		//}
	}

	return nullptr;
}

