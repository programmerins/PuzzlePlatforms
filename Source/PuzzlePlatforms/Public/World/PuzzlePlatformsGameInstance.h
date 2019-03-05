// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"

#include "MainMenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"


class UMainMenu;


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance final : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()


public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void Host() override; /// implemented interface abstact function

	UFUNCTION(Exec)
	virtual void Join(const FString& IPAddress) override; /// implemented interface abstact function

	UFUNCTION()
	virtual void LoadMainMenu() override;


private:
	TSubclassOf<UUserWidget> MainMenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;


private:
	virtual void Init() override;

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool Success);

	IOnlineSessionPtr SessionInterface;
};
