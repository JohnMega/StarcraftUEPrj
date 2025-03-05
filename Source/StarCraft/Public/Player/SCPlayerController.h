// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController.generated.h"

class ASCGoalActor;
class ASCAICharacter;

UCLASS()
class STARCRAFT_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PawnLeavingGame() override;

// UAttackNotifyNetHelper
public:
	UFUNCTION(Server, Unreliable)
	void Server_AttackNotifyFXSpawn(ASCAICharacter* AICharacter);

// AWavesNPCSpawnerNetHelper
public:
	UFUNCTION(Server, Reliable)
	void Server_UnitSpawn(UClass* UnitClass, int32 CurrTeamType);

// UGameMenuWBNetHealper
public:
	UFUNCTION(Server, Reliable)
	void Server_GameMenuOnExitButtonClicked();
};
