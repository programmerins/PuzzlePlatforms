// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"


#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class)) return;

	MainMenuClass = MainMenuBPClass.Class;
}


void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Log, TEXT("Found Class: %s"), *MainMenuClass->GetName());
}


void UPuzzlePlatformsGameInstance::LoadMenu()
{
	UMainMenu* MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (MainMenu)
	{
		MainMenu->Setup();
		MainMenu->SetMainMenuInterface(this);
	}
}


void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Now Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	World->ServerTravel("/Game/Maps/Stage?listen");
}


void UPuzzlePlatformsGameInstance::Join(const FString& IPAddress)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining: %s"), *IPAddress));

	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
	}
}
