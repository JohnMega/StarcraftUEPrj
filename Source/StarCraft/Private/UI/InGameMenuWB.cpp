// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameMenuWB.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UnitSelectMenuWB.h"
#include "Player/SC_MainCamera.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "SCUnitTestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AI/SCAICharacter.h"
#include "AI/SCGoalActor.h"
#include "AI/SCAIController.h"
#include "GameStates/SCGameState.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Player/SC_UnitTestMainCamera.h"

bool UInGameMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (BackToInGameMenuButton)
	{
		BackToInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenuWB::OnBackToInGameMenuButtonClicked);
	}

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UInGameMenuWB::OnStartGameButtonClicked);
	}

	return InitRes;
}

void UInGameMenuWB::BeginPlay()
{
}

void UInGameMenuWB::OnBackToInGameMenuButtonClicked()
{
	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!SCGameState) return;

	if (SCGameState->GetGameState() == EGameState::INGAME_MENU)
	{
		Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnBackToInGameMenuButtonClicked_InGame_MulticastPart();
		Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnBackToInGameMenuButtonClicked_InGame_ServerPart();
	}
	else if (SCGameState->GetGameState() == EGameState::GAME)
	{
		Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnBackToInGameMenuButtonClicked_Game_MulticastPart();
		Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnBackToInGameMenuButtonClicked_Game_ServerPart();
	}
}

void UInGameMenuWB::OnBackToInGameMenuButtonClicked_InGame_ServerPartImplementation()
{
	TArray<AActor*> AllAICharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllAICharacters);
	for (int32 i = 0; i < AllAICharacters.Num(); ++i)
	{
		AllAICharacters[i]->Destroy();
	}
}

void UInGameMenuWB::OnBackToInGameMenuButtonClicked_InGame_MulticastPartImplementation()
{
	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	SCGameState->SetGameState(EGameState::INGAME_MENU);
}

void UInGameMenuWB::OnBackToInGameMenuButtonClicked_Game_ServerPartImplementation()
{
	TArray<AActor*> AllAICharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllAICharacters);
	for (int32 i = 0; i < AllAICharacters.Num(); ++i)
	{
		Cast<ASCAICharacter>(AllAICharacters[i])->DestroySCCharacter();
	}

	TArray<AActor*> AllAIControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAIController::StaticClass(), AllAIControllers);
	for (int32 i = 0; i < AllAIControllers.Num(); ++i)
	{
		auto AIController = Cast<ASCAIController>(AllAIControllers[i]);

		auto SCCharacter = GetWorld()->SpawnActor<ASCAICharacter>(AIController->GetCharacterStartData().CharacterClass, AIController->GetCharacterStartData().StartCharacterPosition
			, FRotator(0, AIController->GetCharacterStartData().IsCharacterFriendly ? 270 : 90, 0));
		if (!SCCharacter) return;
		SCCharacter->SetFriendly(AIController->GetCharacterStartData().IsCharacterFriendly);

		AIController->Possess(SCCharacter);
	}
}

void UInGameMenuWB::OnBackToInGameMenuButtonClicked_Game_MulticastPartImplementation()
{
	UnitSelectMenuWB->SetVisibility(ESlateVisibility::Visible);
	StartGameButtonText->SetText(FText::FromName(StartButtonText));
	BackToInGameMenuButtonText->SetText(FText::FromName(ClearButtonText));

	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	SCGameState->SetGameState(EGameState::INGAME_MENU);
}

void UInGameMenuWB::OnStartGameButtonClicked()
{
	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!SCGameState) return;

	if (SCGameState->GetGameState() == EGameState::INGAME_MENU)
	{
		Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnStartGameButtonClicked_InGame();
	}
	else if (SCGameState->GetGameState() == EGameState::GAME)
	{
		Cast<ASC_UnitTestMainCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Server_OnStartGameButtonClicked_Game();
	}
}

void UInGameMenuWB::OnStartGameButtonClicked_InGame_Implementation()
{
	StartGameButtonText->SetText(FText::FromName(AttackButtonText));
	BackToInGameMenuButtonText->SetText(FText::FromName(BackToMenuButtonText));
	UnitSelectMenuWB->SetVisibility(ESlateVisibility::Hidden);

	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	SCGameState->SetGameState(EGameState::GAME);
}

void UInGameMenuWB::OnStartGameButtonClicked_Game_Implementation()
{
	TArray<AActor*> AllAICharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllAICharacters);

	for (int32 i = 0; i < AllAICharacters.Num(); ++i)
	{
		auto Character = Cast<ASCAICharacter>(AllAICharacters[i]);
		if (!Character) continue;

		Character->SetCurrentGoal(GetWorld()->SpawnActor<ASCGoalActor>(ASCGoalActor::StaticClass(), FTransform(AttackPointLocation)));
		Character->OnAddNewGoalActor(EAICharacterState::ATTACK);
	}
}