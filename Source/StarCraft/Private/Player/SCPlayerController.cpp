// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SCPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/SCGoalActor.h"
#include "AI/SCAICharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/HealthComponent.h"
#include "Engine/DamageEvents.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameStates/SCGameState.h"
#include "OneToOne/WavesNPCSpawner.h"
#include "OneToOne/OTOManager.h"
#include "UI/GameOver/GameOverWB.h"
#include "GameInstances/SCGameInstance.h"

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true;
}

void ASCPlayerController::PawnLeavingGame()
{
	auto OTORoundEndScreenWB = USCFunctionLibrary::GetWidgetByClass<UGameOverWB>(GetWorld());
	if (OTORoundEndScreenWB && OTORoundEndScreenWB->GetVisibility() == ESlateVisibility::Hidden)
	{
		Server_GameMenuOnExitButtonClicked();
	}

	Super::PawnLeavingGame();
}

// UAttackNotifyNetHelper

void ASCPlayerController::Server_AttackNotifyFXSpawn_Implementation(ASCAICharacter* AICharacter)
{
	if (!AICharacter->IsHaveAttackTarget() || AICharacter->IsDead()) return;

	auto HealthComponent = AICharacter->GetComponentByClass<UHealthComponent>();
	if (!HealthComponent) return;

	Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()))->Multicast_AttackNotifySpawnBloodFX(AICharacter->GetAttackTargetCharacter());
	AICharacter->GetAttackTargetCharacter()->TakeDamage(HealthComponent->GetDamage(), FDamageEvent(), Cast<AController>(AICharacter->GetOwner()), AICharacter);
}

// AWavesNPCSpawnerNetHelper

void ASCPlayerController::Server_UnitSpawn_Implementation(UClass* UnitClass, int32 CurrTeamType)
{
	auto WavesNPCSpawner = Cast<AWavesNPCSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AWavesNPCSpawner::StaticClass()));
	WavesNPCSpawner->UnitSpawn_Impl(UnitClass, (ETeamType)CurrTeamType);
}

// UGameMenuWBNetHealper

void ASCPlayerController::Server_GameMenuOnExitButtonClicked_Implementation()
{
	Cast<USCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DeleteSomeWidgetsWhileNetworkFailure();
	Cast<AOTOManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AOTOManager::StaticClass()))->OTOGameEndingHandle(ETeamType::BLUE_TEAM_TYPE);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}