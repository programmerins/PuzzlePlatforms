// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

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
	void Host(FString ServerName) override; /// implemented interface abstact function

	UFUNCTION(Exec)
	void Join(uint32 Index) override; /// implemented interface abstact function

	void LoadMainMenu() override;

	void RefreshServerList() override;

	void StartSession();


private:
	FString DesiredServerName;

	UMainMenu* MainMenu;

	TSubclassOf<UUserWidget> MainMenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;


private:
	virtual void Init() override;

	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool Success);

	void OnDestroySessionComplete(FName SessionName, bool Success);

	void OnFindSessionComplete(bool Success);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
