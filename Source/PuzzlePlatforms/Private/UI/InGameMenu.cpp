// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"


#include "Components/Button.h"


bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!ensure(QuitButton)) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::OnQuitButtonPressed);

	if (!ensure(CancelButton)) { return false; }
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::OnCancelButtonPressed);

	return true;
}


void UInGameMenu::OnQuitButtonPressed()
{
	if (IsVaildInterface())
	{
		GetInterface()->LoadMainMenu();
	}
}


void UInGameMenu::OnCancelButtonPressed()
{
	Teardown();
}
