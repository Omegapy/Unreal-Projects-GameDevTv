// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Mover.h"
#include "MoverRotation.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"

/** 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SetMover(UMover* Mover); // #include "Mover.h"
	
	UFUNCTION(BlueprintCallable)
		void SetMoverRotation(UMoverRotation* NewMoverRotator1, UMoverRotation* NewMoverRotator2); // #include "MoverRoatation.h"


private:
	UPROPERTY(EditAnywhere)
		FName AcceptableActorTag;
		//TArray<FName> AcceptableActorTag;

	UMover* Mover; // #include "Mover.h"

	UMoverRotation* MoverRotator1, *MoverRotator2;

	void MoverRotate(UMoverRotation* MoverRotator, AActor* Actor);

	AActor* GetAcceptableActor() const;
};
