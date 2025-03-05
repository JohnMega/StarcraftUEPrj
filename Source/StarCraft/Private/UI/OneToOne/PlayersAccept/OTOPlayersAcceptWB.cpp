// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OneToOne/PlayersAccept/OTOPlayersAcceptWB.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/SC_UnitTestMainCamera.h"

void UOTOPlayersAcceptWB::NativeConstruct()
{
	Super::NativeConstruct();

	AcceptButton->OnClicked.AddDynamic(this, &UOTOPlayersAcceptWB::OnAcceptButtonClicked);
}

void UOTOPlayersAcceptWB::OnAcceptButtonClicked()
{
	bIsAcceptLocal = !bIsAcceptLocal;

	Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_PlayersAcceptSendAcceptance(bIsAcceptLocal);
}

void UOTOPlayersAcceptWB::OnAcceptButtonClicked_Impl(bool bIsAccept)
{
	AcceptScore = bIsAccept ? AcceptScore + 1 : AcceptScore - 1;
	//AcceptScoreText->SetText(FText::FromString(FString::FromInt(AcceptScore)));
}

void UOTOPlayersAcceptWB::UpdateAcceptScore_Impl(int32 NewAcceptScore)
{
	AcceptScore = NewAcceptScore;
	AcceptScoreText->SetText(FText::FromString(FString::FromInt(AcceptScore)));

	if (AcceptScore == MAX_PLAYERS_NUM)
	{
		OnAllPlayersAccept();
	}
}