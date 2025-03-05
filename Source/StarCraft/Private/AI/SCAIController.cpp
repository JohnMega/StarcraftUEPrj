// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SCAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AI/SCAICharacter.h"
#include "Components/PathFollowingNoStopComponent.h"

ASCAIController::ASCAIController(const FObjectInitializer& Init)
	: Super(Init.SetDefaultSubobjectClass<UPathFollowingNoStopComponent>("PathFollowingComponent"))
{
	MyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	SetPerceptionComponent(*MyPerceptionComponent);
}

void ASCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	const auto STUCharacter = Cast<ASCAICharacter>(InPawn);
	if (STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
		CharacterStartData.StartCharacterPosition = STUCharacter->GetActorLocation();
		CharacterStartData.IsCharacterFriendly = STUCharacter->IsFriendly();
	}
}

void ASCAIController::OnUnPossess()
{
	const auto STUCharacter = Cast<ASCAICharacter>(GetPawn());
	if (STUCharacter)
	{
		STUCharacter->DeleteCurrentGoal();
	}

	Super::OnUnPossess();
}

const FCharacterStartData& ASCAIController::GetCharacterStartData() const
{
	return CharacterStartData;
}

void ASCAIController::SetCharacterClass(UClass* CharacterClass)
{
	CharacterStartData.CharacterClass = CharacterClass;
}

void ASCAIController::SetTeamAttitude(ETeamAttitude::Type NetTeamAttitude)
{
	TeamAttitude = NetTeamAttitude;
	SightConfig = PerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	SightConfig->DetectionByAffiliation.bDetectEnemies = TeamAttitude == ETeamAttitude::Hostile ? false : true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = TeamAttitude == ETeamAttitude::Friendly ? false : true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
}