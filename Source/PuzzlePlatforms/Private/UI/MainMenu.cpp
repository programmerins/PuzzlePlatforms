// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"


#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!ensure(HostButton)) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton)) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinMenuButton)) { return false; }
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}


void UMainMenu::HostServer()
{
	if (MainMenuInterface)
	{
		MainMenuInterface->Host();
	}
}


void UMainMenu::OpenJoinMenu()
{	
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}


void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}


void UMainMenu::SetMainMenuInterface(IMainMenuInterface* MainMenuInterface)
{
	this->MainMenuInterface = MainMenuInterface;
}


void UMainMenu::Setup()
{
	AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}


void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}
