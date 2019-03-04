// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "BaseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBaseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

private:
	IMainMenuInterface* MainMenuInterface;


public:
	void Setup();

	void SetInterFace(IMainMenuInterface* MainMenuInterface);

	FORCEINLINE bool IsVaildInterface() const { return MainMenuInterface != nullptr; }

	FORCEINLINE IMainMenuInterface* const GetInterface() const { return MainMenuInterface; }


protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
