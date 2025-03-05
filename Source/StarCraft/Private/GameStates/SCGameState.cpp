// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/SCGameState.h"
#include "Kismet/GameplayStatics.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "Player/SC_UnitTestMainCamera.h"
#include "Player/SCPlayerController.h"
#include "AI/SCAICharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "UI/UnitSelectMenuWB.h"

ASCGameState::ASCGameState()
{ }

void ASCGameState::SetGameState(EGameState NewGameState)
{
	CurrentGameState = NewGameState;
	OnGameStateChange.Broadcast(CurrentGameState);
}

EGameState ASCGameState::GetGameState() const
{
	return CurrentGameState;
}

int32 ASCGameState::GetTotalSpawnedFriendCharactersCount() const
{
	return TotalSpawnedFriendCharactersCount;
}

void ASCGameState::SetTotalSpawnedFriendCharactersCount(int32 NewCount)
{
	TotalSpawnedFriendCharactersCount = NewCount;
}

int32 ASCGameState::GetTotalSpawnedEnemyCharactersCount() const
{
	return TotalSpawnedEnemyCharactersCount;
}

void ASCGameState::SetTotalSpawnedEnemyCharactersCount(int32 NewCount)
{
	TotalSpawnedEnemyCharactersCount = NewCount;
}

// UAttackNotifyNetHelper

void ASCGameState::Multicast_AttackNotifySpawnBloodFX_Implementation(ASCAICharacter* AttackTargetCharacter)
{
	UNiagaraFunctionLibrary::SpawnSystemAttached(AttackTargetCharacter->GetNiagaraBlood(), AttackTargetCharacter->GetMesh(), NAME_None
		, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}