// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OneToOne/Score/OTOScoreWB.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"
#include "Player/SC_UnitTestMainCamera.h"
#include "AI/SCAICharacter.h"
#include "OneToOne/WavesNPCSpawner.h"

void UOTOScoreWB::NativeConstruct()
{
	Super::NativeConstruct();

	SetWinCheckTimer();
}

void UOTOScoreWB::SetWinCheckTimer()
{
	if (GetWorld()->GetNetMode() == NM_ListenServer)
	{
		GetWorld()->GetTimerManager().SetTimer(WinCheckTimer, this, &UOTOScoreWB::WinCheckTimerHandle, 1.0f, true);
	}
}

void UOTOScoreWB::WinCheckTimerHandle()
{
	int32 FriendsUnitsCount = 0;
	int32 EnemyUnitsCount = 0;

	TArray<AActor*> AllNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllNPCs);
	for (int32 i = 0; i < AllNPCs.Num(); ++i)
	{
		auto NPC = Cast<ASCAICharacter>(AllNPCs[i]);
		FriendsUnitsCount = NPC->IsFriendly() && !NPC->IsDead() ? FriendsUnitsCount + 1 : FriendsUnitsCount;
		EnemyUnitsCount = !NPC->IsFriendly() && !NPC->IsDead() ? EnemyUnitsCount + 1 : EnemyUnitsCount;
	}

	BlueTeamScore = EnemyUnitsCount == 0 ? BlueTeamScore + 1 : BlueTeamScore;
	RedTeamScore = FriendsUnitsCount == 0 ? RedTeamScore + 1 : RedTeamScore;

	if (EnemyUnitsCount == 0 || FriendsUnitsCount == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(WinCheckTimer);
		SetNewScoreFromServer();
	}
}

void UOTOScoreWB::SetNewScoreFromServer()
{
	Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OTOScoreSetNewScore();
}

void UOTOScoreWB::SetNewScoreFromServer_Impl(int32 NewBlueTeamScore, int32 NewRedTeamScore)
{
	BlueTeamScore = NewBlueTeamScore;
	RedTeamScore = NewRedTeamScore;

	BlueTeamScoreText->SetText(FText::FromString(FString::FromInt(BlueTeamScore)));
	RedTeamScoreText->SetText(FText::FromString(FString::FromInt(RedTeamScore)));

	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FinishRound();
}

void UOTOScoreWB::GetScores(int32& CurrBlueTeamScore, int32& CurrRedTeamScore)
{
	CurrBlueTeamScore = BlueTeamScore;
	CurrRedTeamScore = RedTeamScore;
}

bool UOTOScoreWB::IsOTOGameEnded()
{
	auto WavesNPCSpawner = Cast<AWavesNPCSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AWavesNPCSpawner::StaticClass()));
	return WavesNPCSpawner->GetCurrentWave() > WavesNPCSpawner->GetWavesCount();
}