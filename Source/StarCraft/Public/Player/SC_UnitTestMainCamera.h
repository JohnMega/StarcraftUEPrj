// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SC_MainCamera.h"
#include "SC_UnitTestMainCamera.generated.h"

enum class EGameState;
class ASCAICharacter;

UCLASS()
class STARCRAFT_API ASC_UnitTestMainCamera : public ASC_MainCamera
{
	GENERATED_BODY()
	
private:
	AActor* StartViewTarget;
	FTimerHandle SetViewTargetWithBlendTimer;

// Input Vars
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping")
	UInputMappingContext* InGameMenuMappingContext;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View Target")
	TSubclassOf<AActor> StartViewTargetClass;

protected:
	virtual void BeginPlay() override;
	virtual void ChangeUnitSkillsKeys() override;

	void OnGameStateChange(EGameState NewGameState);
	void OnSetViewTargetWithBlendFinish();

public:
	ASC_UnitTestMainCamera(const FObjectInitializer& ObjectInitializer);

	void SetupInGameMenuInputComponentBinding(UInputComponent* PlayerInputComponent);
	void SetupGameInputComponentBinding(UInputComponent* PlayerInputComponent);

// AUnitSelectMenuNetHelper
public:
	UFUNCTION(Server, Reliable)
	void Server_OnMarineSpawnButtonClicked(int32 CurrTeamType);

	UFUNCTION(Server, Reliable)
	void Server_OnMinionSpawnButtonClicked(int32 CurrTeamType);

// AInGameMenuNetHelper
public:
	UFUNCTION(Server, Reliable)
	void Server_OnStartGameButtonClicked_InGame();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnStartGameButtonClicked_InGame();

	UFUNCTION(Server, Reliable)
	void Server_OnStartGameButtonClicked_Game();

	UFUNCTION(Server, Reliable)
	void Server_OnBackToInGameMenuButtonClicked_InGame_ServerPart();

	UFUNCTION(Server, Reliable)
	void Server_OnBackToInGameMenuButtonClicked_InGame_MulticastPart();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnBackToInGameMenuButtonClicked_InGame_MulticastPart();

	UFUNCTION(Server, Reliable)
	void Server_OnBackToInGameMenuButtonClicked_Game_ServerPart();

	UFUNCTION(Server, Reliable)
	void Server_OnBackToInGameMenuButtonClicked_Game_MulticastPart();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnBackToInGameMenuButtonClicked_Game_MulticastPart();

// UOTOPlayersAcceptWBNetHelper
public:
	UFUNCTION(Server, Reliable)
	void Server_PlayersAcceptSendAcceptance(bool bIsAccept);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayersAcceptSendAcceptance(int32 AcceptsNum);

// UOTOScoreWBNetHelper
public:
	UFUNCTION(Server, Reliable)
	void Server_OTOScoreSetNewScore();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OTOScoreSetNewScore(int32 NewBlueTeamScore, int32 NewRedTeamScore);
};