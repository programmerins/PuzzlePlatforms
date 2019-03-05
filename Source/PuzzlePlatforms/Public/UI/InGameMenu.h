// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenuWidget.h"
#include "InGameMenu.generated.h"


class UButton;


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu final : public UBaseMenuWidget
{
	GENERATED_BODY()


private:
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;


private:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnQuitButtonPressed();

	UFUNCTION()
	void OnCancelButtonPressed();
};
