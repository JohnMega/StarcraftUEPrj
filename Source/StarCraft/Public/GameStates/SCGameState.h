// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SCGameState.generated.h"

class ASCAICharacter;

enum class EGameState
{
	INGAME_MENU,
	GAME
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangeSignature, EGameState)

UCLASS()
class STARCRAFT_API ASCGameState : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	int32 TotalSpawnedFriendCharactersCount = 0;
	int32 TotalSpawnedEnemyCharactersCount = 0;

	EGameState CurrentGameState = EGameState::INGAME_MENU;

public:
	FOnGameStateChangeSignature OnGameStateChange;

public:
	ASCGameState();

	UFUNCTION(BlueprintImplementableEvent)
	void DestroySession();

	int32 GetTotalSpawnedFriendCharactersCount() const;
	void SetTotalSpawnedFriendCharactersCount(int32 NewCount);

	int32 GetTotalSpawnedEnemyCharactersCount() const;
	void SetTotalSpawnedEnemyCharactersCount(int32 NewCount);

	void SetGameState(EGameState NewGameState);
	EGameState GetGameState() const;

// UAttackNotifyNetHelper
public:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_AttackNotifySpawnBloodFX(ASCAICharacter* AttackTargetCharacter);
};
