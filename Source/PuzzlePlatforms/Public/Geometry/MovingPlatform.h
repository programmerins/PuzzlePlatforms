// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()


public:
	AMovingPlatform();

	UFUNCTION()
	void AddActiveTrigger();

	void RemoveActiveTrigger();


protected:
	UPROPERTY(EditInstanceOnly, Category = "Actor")
	float MovingSpeed = 20;

	UPROPERTY(EditInstanceOnly, Category = "Actor", meta = (MakeEditWidget=true))
	FVector TargetLocation;


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


private:
	FVector GlobalTargetLocation;

	FVector GlobalStartLocation;

	UPROPERTY(EditAnyWhere)
	int32 ActiveTriggers = 1;
};
