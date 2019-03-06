// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/PuzzlePlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ALobbyGameMode final : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()


public:
	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;


private:
	FTimerHandle TimerHandle_StartGame;

	uint32 NumberOfPlayers = 0;


private:
	void StartGame();
};
