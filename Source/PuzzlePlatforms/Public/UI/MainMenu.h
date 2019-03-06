// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenuWidget.h"
#include "MainMenu.generated.h"


class UButton;
class UWidgetSwitcher;
class UEditableTextBox;
class UPanelWidget;


USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;

	uint16 CurrentPlayers;

	uint16 MaxPlayers;

	FString HostUserName;
};


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu final : public UBaseMenuWidget
{
	GENERATED_BODY()


public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerDatas);

	void SetVisibilityFindSessionIcon(ESlateVisibility&& NewSlateVisibility);

	void SelectIndex(uint32 Index);


private:
	TSubclassOf<UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

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

	UPROPERTY (meta = (BindWidget))
	UWidget* FindSesionIcon;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;


private:
	virtual bool Initialize() override;

	void UpdateChildren();

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void QuitPressed();
};
