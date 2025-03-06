// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SC_UnitTestMainCamera.h"
#include "Kismet/GameplayStatics.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "SCUnitTestGameMode.h"
#include "Camera/CameraComponent.h"
#include "AI/SCGoalActor.h"
#include "Player/SCPlayerController.h"
#include "AI/SCAICharacter.h"
#include "AI/SCAIController.h"
#include "AI/Characters/Marine/SCMarine.h"
#include "AI/Characters/Minion/SCMinion.h"
#include "Components/UnitTestCameraWidgetComponent.h"
#include "Skills/MainSkillsActor.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "UObject/UObjectGlobals.h"
#include "GameStates/SCGameState.h"
#include "Components/HealthComponent.h"
#include "UI/InGameMenuWB.h"
#include "UI/UnitSelectMenuWB.h"
#include "UI/OneToOne/PlayersAccept/OTOPlayersAcceptWB.h"
#include "UI/OneToOne/Score/OTOScoreWB.h"
#include "OneToOne/WavesNPCSpawner.h"

ASC_UnitTestMainCamera::ASC_UnitTestMainCamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UUnitTestCameraWidgetComponent>("MainCameraWidgetComponent"))
{ }

void ASC_UnitTestMainCamera::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<APlayerController>(Controller))
	{
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSubsystem)
		{
			InputSubsystem->ClearAllMappings();
			InputSubsystem->AddMappingContext(GameMappingContext, 0);
		}
	}

	StartViewTarget = GetWorld()->SpawnActor<AActor>(StartViewTargetClass.Get(), GetActorTransform());
	StartViewTarget->SetActorRotation(FRotator(StartViewTarget->GetActorRotation().Pitch, 90, StartViewTarget->GetActorRotation().Roll));

	auto SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!SCGameState) return;

	SCGameState->OnGameStateChange.AddUObject(this, &ASC_UnitTestMainCamera::OnGameStateChange);
}

void ASC_UnitTestMainCamera::SetupInGameMenuInputComponentBinding(UInputComponent* PlayerInputComponent)
{
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InGameMenuMappingContext, 0);
}

void ASC_UnitTestMainCamera::OnGameStateChange(EGameState NewGameState)
{
	if (NewGameState == EGameState::GAME)
	{
		IsMoveOnMouseAvailable = true;

		auto PlayerController = Cast<ASCPlayerController>(GetOwner());
		if (!PlayerController) return;

		PlayerController->SetViewTargetWithBlend(StartViewTarget, 1.0f, VTBlend_Cubic);
		DisableInput(PlayerController);
		GetWorld()->GetTimerManager().SetTimer(SetViewTargetWithBlendTimer, this, &ASC_UnitTestMainCamera::OnSetViewTargetWithBlendFinish, 1.1f, false);
	}
	else if (NewGameState == EGameState::INGAME_MENU)
	{
		IsMoveOnMouseAvailable = false;

		SelectedUnits.Empty();
		OnClearSelectUnits.Broadcast();

		auto PlayerController = Cast<ASCPlayerController>(GetOwner());
		if (!PlayerController) return;
		if (PlayerController->GetLocalPlayer())
		{
			SetupInGameMenuInputComponentBinding(InputComponent);
		}
	}
}

void ASC_UnitTestMainCamera::OnSetViewTargetWithBlendFinish()
{
	auto PlayerController = Cast<ASCPlayerController>(GetOwner());
	if (!PlayerController) return;

	SetActorLocation(StartViewTarget->GetActorLocation());
	PlayerController->SetViewTargetWithBlend(this, 1.0f, VTBlend_Cubic);
	EnableInput(PlayerController);

	if (PlayerController->GetLocalPlayer())
	{
		SetupGameInputComponentBinding();
	}
}

void ASC_UnitTestMainCamera::ChangeUnitSkillsKeys()
{
	//if (Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld()))->GetGameState() != EGameState::GAME) return;

	ASC_MainCamera::ChangeUnitSkillsKeys();
}

// AUnitSelectMenuNetHelper
/////////////////////////////////////////////////////////////////////////////////

void ASC_UnitTestMainCamera::Server_OnMarineSpawnButtonClicked_Implementation(int32 CurrTeamType)
{
	auto UnitSelectMenuWB = USCFunctionLibrary::GetWidgetByClass<UUnitSelectMenuWB>(GetWorld());
	UnitSelectMenuWB->OnMarineSpawnButtonClicked_Impl((EUnitSelectMenuType)CurrTeamType);
}

void ASC_UnitTestMainCamera::Server_OnMinionSpawnButtonClicked_Implementation(int32 CurrTeamType)
{
	auto UnitSelectMenuWB = USCFunctionLibrary::GetWidgetByClass<UUnitSelectMenuWB>(GetWorld());
	UnitSelectMenuWB->OnMinionSpawnButtonClicked_Impl((EUnitSelectMenuType)CurrTeamType);
}

/////////////////////////////////////////////////////////////////////////////////

// AInGameMenuNetHelper
/////////////////////////////////////////////////////////////////////////////////

void ASC_UnitTestMainCamera::Server_OnStartGameButtonClicked_InGame_Implementation()
{
	Multicast_OnStartGameButtonClicked_InGame();
}

void ASC_UnitTestMainCamera::Multicast_OnStartGameButtonClicked_InGame_Implementation()
{
	auto GameMenuWB = USCFunctionLibrary::GetWidgetByClass<UInGameMenuWB>(GetWorld());
	GameMenuWB->OnStartGameButtonClicked_InGame_Implementation();
}

void ASC_UnitTestMainCamera::Server_OnStartGameButtonClicked_Game_Implementation()
{
	auto GameMenuWB = USCFunctionLibrary::GetWidgetByClass<UInGameMenuWB>(GetWorld());
	GameMenuWB->OnStartGameButtonClicked_Game_Implementation();
}

void ASC_UnitTestMainCamera::Server_OnBackToInGameMenuButtonClicked_InGame_ServerPart_Implementation()
{
	auto GameMenuWB = USCFunctionLibrary::GetWidgetByClass<UInGameMenuWB>(GetWorld());
	GameMenuWB->OnBackToInGameMenuButtonClicked_InGame_ServerPartImplementation();
}

void ASC_UnitTestMainCamera::Server_OnBackToInGameMenuButtonClicked_InGame_MulticastPart_Implementation()
{
	Multicast_OnBackToInGameMenuButtonClicked_InGame_MulticastPart();
}

void ASC_UnitTestMainCamera::Multicast_OnBackToInGameMenuButtonClicked_InGame_MulticastPart_Implementation()
{
	auto GameMenuWB = USCFunctionLibrary::GetWidgetByClass<UInGameMenuWB>(GetWorld());
	GameMenuWB->OnBackToInGameMenuButtonClicked_InGame_MulticastPartImplementation();
}

void ASC_UnitTestMainCamera::Server_OnBackToInGameMenuButtonClicked_Game_ServerPart_Implementation()
{
	auto GameMenuWB = USCFunctionLibrary::GetWidgetByClass<UInGameMenuWB>(GetWorld());
	GameMenuWB->OnBackToInGameMenuButtonClicked_Game_ServerPartImplementation();
}

void ASC_UnitTestMainCamera::Server_OnBackToInGameMenuButtonClicked_Game_MulticastPart_Implementation()
{
	Multicast_OnBackToInGameMenuButtonClicked_Game_MulticastPart();
}

void ASC_UnitTestMainCamera::Multicast_OnBackToInGameMenuButtonClicked_Game_MulticastPart_Implementation()
{
	auto GameMenuWB = USCFunctionLibrary::GetWidgetByClass<UInGameMenuWB>(GetWorld());
	GameMenuWB->OnBackToInGameMenuButtonClicked_Game_MulticastPartImplementation();
}

/////////////////////////////////////////////////////////////////////////////////

// UOTOPlayersAcceptWBNetHelper

void ASC_UnitTestMainCamera::Server_PlayersAcceptSendAcceptance_Implementation(bool bIsAccept)
{
	auto PlayersAcceptWB = USCFunctionLibrary::GetWidgetByClass<UOTOPlayersAcceptWB>(GetWorld());
	PlayersAcceptWB->OnAcceptButtonClicked_Impl(bIsAccept);
	Multicast_PlayersAcceptSendAcceptance(PlayersAcceptWB->GetAcceptsNum());
}

void ASC_UnitTestMainCamera::Multicast_PlayersAcceptSendAcceptance_Implementation(int32 AcceptsNum)
{
	auto PlayersAcceptWB = USCFunctionLibrary::GetWidgetByClass<UOTOPlayersAcceptWB>(GetWorld());
	PlayersAcceptWB->UpdateAcceptScore_Impl(AcceptsNum);
}

// UOTOScoreWBNetHelper

void ASC_UnitTestMainCamera::Server_OTOScoreSetNewScore_Implementation()
{
	auto OTOScoreWB = USCFunctionLibrary::GetWidgetByClass<UOTOScoreWB>(GetWorld());

	int32 CurrBlueTeamScore;
	int32 CurrRedTeamScore;
	OTOScoreWB->GetScores(CurrBlueTeamScore, CurrRedTeamScore);
	Multicast_OTOScoreSetNewScore(CurrBlueTeamScore, CurrRedTeamScore);
}

void ASC_UnitTestMainCamera::Multicast_OTOScoreSetNewScore_Implementation(int32 NewBlueTeamScore, int32 NewRedTeamScore)
{
	auto OTOScoreWB = USCFunctionLibrary::GetWidgetByClass<UOTOScoreWB>(GetWorld());
	OTOScoreWB->SetNewScoreFromServer_Impl(NewBlueTeamScore, NewRedTeamScore);
}