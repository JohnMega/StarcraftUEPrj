// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOver/GameOverWB.h"
#include "Kismet/GameplayStatics.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "Components/Button.h"
#include "GameStates/SCGameState.h"
#include "Player/SCPlayerController.h"
#include "OneToOne/OTOManager.h"
#include "UI/GameOver/GameOverWB.h"

bool UGameOverWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (RetryButton)
		RetryButton->OnClicked.AddDynamic(this, &UGameOverWB::OnRetryButtonClicked);

	if (BackToMenuButton)
		BackToMenuButton->OnClicked.AddDynamic(this, &UGameOverWB::OnBackToMenuButtonClicked);

	return InitRes;
}

void UGameOverWB::OnRetryButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), ThisLevelName);
}

void UGameOverWB::OnBackToMenuButtonClicked()
{
	if (GetWorld()->GetNetMode() != NM_Standalone)
	{
		auto OTORoundEndScreenWB = USCFunctionLibrary::GetWidgetByClass<UGameOverWB>(GetWorld());
		if (OTORoundEndScreenWB && OTORoundEndScreenWB->GetVisibility() == ESlateVisibility::Hidden)
		{
			Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Server_GameMenuOnExitButtonClicked();
		}
		Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()))->DestroySession();
	}

	UGameplayStatics::OpenLevel(GetWorld(), MenuLevelName);
}