// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"


#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "BaseMenuWidget.h"


const static FName SESSION_NAME = TEXT("My Session Game");


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/PuzzlePlatforms/UI/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class)) return;
	MainMenuClass = MainMenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/PuzzlePlatforms/UI/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class)) return;
	InGameMenuClass = InGameMenuBPClass.Class;
}


void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Log, TEXT("Found Class: %s"), *MainMenuClass->GetName())
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();

	if (OSS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OSS: %s"), *OSS->GetSubsystemName().ToString())
		SessionInterface = OSS->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	}
}


void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName().IsEqual("NULL") ? true : false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}


void UPuzzlePlatformsGameInstance::LoadMenu()
{
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (MainMenu)
	{
		MainMenu->Setup();
		MainMenu->SetInterFace(this);
	}
}


void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	UBaseMenuWidget* InGameMenu = CreateWidget<UBaseMenuWidget>(this, InGameMenuClass);
	if (InGameMenu)
	{
		InGameMenu->Setup();
		InGameMenu->SetInterFace(this);
	}
}


void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}


void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}


void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel("/Game/PuzzlePlatforms/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
	}
}


void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Log, TEXT("starting find session.."))
	}
}


void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"))
			return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Now Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Stage?listen");
}


void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}


void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Success)
{
	if (MainMenu)
	{
		MainMenu->SetVisibilityFindSessionIcon(ESlateVisibility::Hidden);
	}

	if (Success && SessionSearch.IsValid())
	{
		TArray<FServerData> ServerNames;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.CurrentPlayers = SearchResult.Session.NumOpenPublicConnections;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;

			ServerNames.Add(Data);
		}

		MainMenu->SetServerList(ServerNames);
	}

	UE_LOG(LogTemp, Warning, TEXT("Finished find session !"))
}


void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString IPAddress;
	if (!SessionInterface->GetResolvedConnectString(SessionName, OUT IPAddress))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."))
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining: %s"), *IPAddress));

	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
	}
}
