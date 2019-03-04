// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenuWidget.h"
#include "MainMenu.generated.h"


class UButton;
class UWidgetSwitcher;
class UEditableTextBox;


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UBaseMenuWidget
{
	GENERATED_BODY()


private:
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IPAddressField;


private:
	virtual bool Initialize() override;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();
};
