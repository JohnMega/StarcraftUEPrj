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

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true;
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