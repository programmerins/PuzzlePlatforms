// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MainMenu.generated.h"


class UButton;
class UWidgetSwitcher;


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetMainMenuInterface(IMainMenuInterface* MainMenuInterface);

	void Setup();


protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;


private:
	IMainMenuInterface* MainMenuInterface;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;


private:
	virtual bool Initialize() override;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();
};
