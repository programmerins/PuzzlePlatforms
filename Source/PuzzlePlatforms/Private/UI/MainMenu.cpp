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
	if (!Success) { return false; }

	if (!ensure(HostButton)) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton)) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitButton)) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

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


void UMainMenu::OpenJoinMenu()
{	
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;

	GetInterface()->RefreshServerList();

	ServerList->ClearChildren();
	MenuSwitcher->SetActiveWidget(JoinMenu);
	SetVisibilityFindSessionIcon(ESlateVisibility::Visible);
}


void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}


void UMainMenu::QuitPressed()
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
		RowText->HostUser->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));
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
