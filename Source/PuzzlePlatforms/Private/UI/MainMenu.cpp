// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"


#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/PuzzlePlatforms/UI/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class)) return;

	ServerRowClass = ServerRowBPClass.Class;
}


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!ensure(Success)) return false;

	/* Bind to Host Button (Main Menu)*/
	if (!ensure(HostButton)) {	return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnPressedHostMenuButton);

	/* Bind to Join Button (Main Menu) */
	if (!ensure(JoinButton)) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnPressedJoinMenuButton);

	/* Bind to Join Button (Join Menu) */
	if (!ensure(ConfirmJoinMenuButton)) { return false; }
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnPressedConfirmJoinMenuButton);
	
	/* Bind to Cancel button (Join Menu) */
	if (!ensure(CancelJoinMenuButton)) { return false; }
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnPressedCancelJoinMenuButton);

	/* Bind to Host Button (Host Menu) */
	if (!ensure(ConfirmHostMenuButton)) { return false; }
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnPressedConfirmHostMenuButton);

	/* Bind to Host Button (Host Menu) */
	if (!ensure(CancelHostMenuButton)) { return false; }
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnPressedCancelHostMenuButton);

	/* Bind to Game Quit Button */
	if (!ensure(QuitButton)) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnPressedQuitButton);

	//if (!ensure(ConfirmHostMenuButton)) { return false; }
	//CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}


void UMainMenu::OnPressedConfirmHostMenuButton()
{
	if (IsVaildInterface())
	{
		FString ServerName = ServerHostName->Text.ToString();
		GetInterface()->Host(ServerName);
	}
}


void UMainMenu::OnPressedCancelHostMenuButton()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}


void UMainMenu::OnPressedConfirmJoinMenuButton()
{
	if (SelectedIndex.IsSet() && IsVaildInterface())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %d"), SelectedIndex.GetValue())
		GetInterface()->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not index selected"))
	}
}


void UMainMenu::OnPressedCancelJoinMenuButton()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}


void UMainMenu::OnPressedHostMenuButton()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;

	MenuSwitcher->SetActiveWidget(HostMenu);
}


void UMainMenu::OnPressedJoinMenuButton()
{	
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;

	GetInterface()->RefreshServerList();

	ServerList->ClearChildren();
	MenuSwitcher->SetActiveWidget(JoinMenu);
	SetVisibilityFindSessionIcon(ESlateVisibility::Visible);
}


void UMainMenu::OnPressedQuitButton()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC != nullptr)) return;

	PC->ConsoleCommand("quit");
}


void UMainMenu::SetServerList(TArray<FServerData> ServerDatas)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World)) return;

	uint32 i = 0;

	for (const FServerData& ServerData : ServerDatas)
	{
		UServerRow* RowText = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(RowText)) return;

		RowText->ServerName->SetText(FText::FromString(ServerData.Name));
		RowText->HostUser->SetText(FText::FromString(ServerData.HostUserName));
		RowText->ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));
		RowText->Setup(this, i++);

		ServerList->AddChild(RowText);
	}
}


void UMainMenu::SetVisibilityFindSessionIcon(ESlateVisibility&& NewSlateVisibility)
{
	FindSesionIcon->SetVisibility(NewSlateVisibility);
}


void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}


void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));

		if (Row)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
