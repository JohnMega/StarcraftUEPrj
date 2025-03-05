// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitSelectMenuWB.h"
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

bool UUnitSelectMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (MarineSpawnButton)
		MarineSpawnButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnMarineSpawnButtonClicked);

	if (MinionSpawnButton)
		MinionSpawnButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnMinionSpawnButtonClicked);

	if (BlueTeamMenuButton)
		BlueTeamMenuButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnBlueTeamMenuButtonClicked);

	if (RedTeamMenuButton)
		RedTeamMenuButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnRedTeamMenuButtonClicked);

	UnitSpawnButtonsTexts.Add(MarineSpawnButtonText);
	UnitSpawnButtonsTexts.Add(MinionSpawnButtonText);

	return InitRes;
}

void UUnitSelectMenuWB::OnBlueTeamMenuButtonClicked()
{
	UnitSelectMenuBackground->SetColorAndOpacity(BlueTeamBackgroundColor);
	for (int32 i = 0; i < UnitSpawnButtonsTexts.Num(); ++i) UnitSpawnButtonsTexts[i]->SetColorAndOpacity(FColor::Blue);
	TeamType = EUnitSelectMenuType::BLUE_TEAM_TYPE;
}

void UUnitSelectMenuWB::OnRedTeamMenuButtonClicked()
{
	UnitSelectMenuBackground->SetColorAndOpacity(RedTeamBackgroundColor);
	for (int32 i = 0; i < UnitSpawnButtonsTexts.Num(); ++i) UnitSpawnButtonsTexts[i]->SetColorAndOpacity(FColor::Red);
	TeamType = EUnitSelectMenuType::RED_TEAM_TYPE;
}

void UUnitSelectMenuWB::OnMarineSpawnButtonClicked()
{
	Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnMarineSpawnButtonClicked((int32)TeamType);
}

void UUnitSelectMenuWB::OnMinionSpawnButtonClicked()
{
	Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnMinionSpawnButtonClicked((int32)TeamType);
}

void UUnitSelectMenuWB::OnMarineSpawnButtonClicked_Impl(EUnitSelectMenuType CurrTeamType)
{
	UnitSpawn(MarineClass.Get(), CurrTeamType);
}

void UUnitSelectMenuWB::OnMinionSpawnButtonClicked_Impl(EUnitSelectMenuType CurrTeamType)
{
	UnitSpawn(MinionClass.Get(), CurrTeamType);
}

void UUnitSelectMenuWB::UnitSpawn(UClass* UnitClass, EUnitSelectMenuType CurrTeamType)
{
	auto SpawnPointClass = (CurrTeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE ? FriendUnitsSpawnPointClass.Get() : EnemyUnitsSpawnPointClass.Get());
	auto UnitsSpawnPoint = Cast<AFriendUnitsSpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), SpawnPointClass));
	if (!UnitsSpawnPoint) return;

	auto SCAIController = GetWorld()->SpawnActor<ASCAIController>(AIControllerClass.Get());
	auto SCUnit = GetWorld()->SpawnActor<ASCAICharacter>(UnitClass, UnitsSpawnPoint->GetBestSpawnPoint()
		, FRotator(0, CurrTeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE ? 0 : 180, 0));
	if (!SCUnit) return;
	SCUnit->SetFriendly(CurrTeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE ? true : false);

	SCAIController->Possess(SCUnit);
	SCAIController->SetCharacterClass(UnitClass);
	SCAIController->SetTeamAttitude(CurrTeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE ? ETeamAttitude::Friendly : ETeamAttitude::Hostile);

	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!SCGameState) return;

	if (CurrTeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE)
	{
		SCGameState->SetTotalSpawnedFriendCharactersCount(SCGameState->GetTotalSpawnedFriendCharactersCount() + 1);
	}
	else
	{
		SCGameState->SetTotalSpawnedEnemyCharactersCount(SCGameState->GetTotalSpawnedEnemyCharactersCount() + 1);
	}
}