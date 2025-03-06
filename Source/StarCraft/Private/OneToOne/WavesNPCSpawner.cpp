// Fill out your copyright notice in the Description page of Project Settings.


#include "OneToOne/WavesNPCSpawner.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "UI/InGameMenuWB.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AI/SpawnPoint/FriendUnitsSpawnPoint.h"
#include "AI/SpawnPoint/EnemyUnitsSpawnPoint.h"
#include "AI/SCAIController.h"
#include "AI/Characters/Marine/SCMarine.h"
#include "AI/Characters/Minion/SCMinion.h"
#include "Kismet/GameplayStatics.h"
#include "SCUnitTestGameMode.h"
#include "GameStates/SCGameState.h"
#include "Player/SC_UnitTestMainCamera.h"
#include "Player/SCPlayerController.h"

AWavesNPCSpawner::AWavesNPCSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWavesNPCSpawner::SpawnNPCWaves(int32 Wave)
{
	for (int32 i = 0; i < NPCWaves[Wave].NPCClasses.Num(); ++i)
	{
		for (int32 j = 0; j < NPCWaves[Wave].NPCCounts[i]; ++j)
		{
			UnitSpawn(NPCWaves[Wave].NPCClasses[i], ETeamType::BLUE_TEAM_TYPE);
			UnitSpawn(NPCWaves[Wave].NPCClasses[i], ETeamType::RED_TEAM_TYPE);
		}
	}
	CurrentWave = Wave + 1;
}

void AWavesNPCSpawner::UnitSpawn(UClass* UnitClass, ETeamType CurrTeamType)
{
	Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Server_UnitSpawn(UnitClass, (int32)CurrTeamType);
}

void AWavesNPCSpawner::UnitSpawn_Impl(UClass* UnitClass, ETeamType CurrTeamType)
{
	auto SpawnPointClass = (CurrTeamType == ETeamType::BLUE_TEAM_TYPE ? FriendUnitsSpawnPointClass.Get() : EnemyUnitsSpawnPointClass.Get());
	auto UnitsSpawnPoint = Cast<AFriendUnitsSpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), SpawnPointClass));
	if (!UnitsSpawnPoint) return;

	auto SCAIController = GetWorld()->SpawnActor<ASCAIController>(AIControllerClass.Get());
	auto SCUnit = GetWorld()->SpawnActor<ASCAICharacter>(UnitClass, UnitsSpawnPoint->GetBestSpawnPoint()
		, FRotator(0, CurrTeamType == ETeamType::BLUE_TEAM_TYPE ? 0 : 180, 0));
	if (!SCUnit) return;
	SCUnit->SetFriendly(CurrTeamType == ETeamType::BLUE_TEAM_TYPE ? true : false);
	SetTeamOverlayMaterial(SCUnit, CurrTeamType);

	SCAIController->Possess(SCUnit);
	SCAIController->SetCharacterClass(UnitClass);
	SCAIController->SetTeamAttitude(CurrTeamType == ETeamType::BLUE_TEAM_TYPE ? ETeamAttitude::Friendly : ETeamAttitude::Hostile);

	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!SCGameState) return;

	if (CurrTeamType == ETeamType::BLUE_TEAM_TYPE)
	{
		SCGameState->SetTotalSpawnedFriendCharactersCount(SCGameState->GetTotalSpawnedFriendCharactersCount() + 1);
	}
	else
	{
		SCGameState->SetTotalSpawnedEnemyCharactersCount(SCGameState->GetTotalSpawnedEnemyCharactersCount() + 1);
	}
}

void AWavesNPCSpawner::SetTeamOverlayMaterial(ASCAICharacter* SCUnit, ETeamType CurrTeamType)
{
	SCUnit->GetMesh()->SetOverlayMaterial(CurrTeamType == ETeamType::BLUE_TEAM_TYPE ? BlueTeamOverlayColor : RedTeamOverlayColor);
}