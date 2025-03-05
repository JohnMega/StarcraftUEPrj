// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/GameMenuWB.h"
#include "UI/Menu/UserSettingsWB.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Player/SC_MainCamera.h"
#include "GameStates/SCGameState.h"
#include "Player/SCPlayerController.h"

bool UGameMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (ContinueButton)
		ContinueButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnContinueButtonClicked);

	if (SettingsButton)
		SettingsButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnSettingsButtonClicked);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnExitButtonClicked);

	if(SettingsBackButton)
		SettingsBackButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnSettingsBackButtonClicked);

	return InitRes;
}

void UGameMenuWB::BeginPlay()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UGameMenuWB::OnGameMenuEnable()
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	SetVisibility(ESlateVisibility::Visible);
}

void UGameMenuWB::OnContinueButtonClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);

	Cast<ASC_MainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->SetupGameInputComponentBinding();
}

void UGameMenuWB::OnSettingsButtonClicked()
{
	GameMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	SettingsBackOverlay->SetVisibility(ESlateVisibility::Visible);

	UserSettingsWB->SetVisibility(ESlateVisibility::Visible);
	UserSettingsWB->SetColorAndOpacity({ 1,1,1,1 });
}

void UGameMenuWB::OnExitButtonClicked()
{
	if (GetWorld()->GetNetMode() != NM_Standalone)
	{
		Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Server_GameMenuOnExitButtonClicked();
		Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()))->DestroySession();
	}

	UGameplayStatics::OpenLevel(GetWorld(), ManuLevelName);
}

void UGameMenuWB::OnSettingsBackButtonClicked()
{
	GameMenuOverlay->SetVisibility(ESlateVisibility::Visible);
	SettingsBackOverlay->SetVisibility(ESlateVisibility::Hidden);

	UserSettingsWB->SetVisibility(ESlateVisibility::Hidden);
	UserSettingsWB->SetColorAndOpacity({ 1,1,1,0 });
}