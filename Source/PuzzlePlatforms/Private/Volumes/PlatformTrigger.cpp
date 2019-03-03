// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"


#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Box Components"));
	SetRootComponent(TriggerVolume);
}


// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnBeginOverlap);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnEndOverlap);
}


// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StaticMeshComp && TargetEmissiveAlpha > -1)
	{
		EmissiveAlpha = FMath::FInterpTo(EmissiveAlpha, TargetEmissiveAlpha, DeltaTime, 3.0f);
		StaticMeshComp->SetScalarParameterValueOnMaterials(FName("Alpha"), EmissiveAlpha);

		if (FMath::RoundToInt(EmissiveAlpha) == TargetEmissiveAlpha)
		{
			TargetEmissiveAlpha = -1;
		}
	}
}


void APlatformTrigger::SetPressurePadEmissiveColor(bool bFlag)
{
	EmissiveAlpha = StaticMeshComp->GetScalarParameterDefaultValue(FName("Alpha"));
	TargetEmissiveAlpha = (bFlag) ? 20 : 0;
}


void APlatformTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}

	SetPressurePadEmissiveColor(true);
}


void APlatformTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}

	SetPressurePadEmissiveColor(false);
}
