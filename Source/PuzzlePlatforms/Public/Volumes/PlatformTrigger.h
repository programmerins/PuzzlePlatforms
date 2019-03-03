// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"


class UBoxComponent;
class UStaticMeshComponent;
class AMovingPlatform;


UCLASS()
class PUZZLEPLATFORMS_API APlatformTrigger : public AActor
{
	GENERATED_BODY()	
	

public:	
	// Sets default values for this actor's properties
	APlatformTrigger();


protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere)
	TArray<AMovingPlatform*> PlatformsToTrigger;

	float EmissiveAlpha = 0;

	float TargetEmissiveAlpha = -1;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetPressurePadEmissiveColor(bool bFlag);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
