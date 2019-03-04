// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"


#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!ensure(HostButton)) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton)) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(ConfirmJoinMenuButton)) { return false; }
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(CancelJoinMenuButton)) { return false; }
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}


void UMainMenu::HostServer()
{
	if (IsVaildInterface())
	{
		GetInterface()->Host();
	}
}


void UMainMenu::JoinServer()
{
	if (IsVaildInterface())
	{
		if (!ensure(IPAddressField)) return;

		const FString& Address = IPAddressField->GetText().ToString();
		GetInterface()->Join(Address);
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
