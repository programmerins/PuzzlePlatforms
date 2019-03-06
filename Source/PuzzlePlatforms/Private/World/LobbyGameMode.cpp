// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"


#include "Engine/Engine.h"
#include "TimerManager.h"


#include "PuzzlePlatformsGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 3)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_StartGame);
		GetWorldTimerManager().SetTimer(TimerHandle_StartGame, this, &ALobbyGameMode::StartGame, 5.0f, false);
	}
}


void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}


void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (!ensure(GameInstance)) return;

	GameInstance->StartSession();
	GameInstance->GetEngine()->AddOnScreenDebugMessage(0, 15, FColor::Green, TEXT("Reached 3 players!"));

	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/PuzzlePlatforms/Maps/Stage?listen");
}
