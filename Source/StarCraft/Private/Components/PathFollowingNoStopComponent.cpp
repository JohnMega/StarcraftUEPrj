// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PathFollowingNoStopComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "AbstractNavData.h"
#include "AIConfig.h"
#include "AIController.h"
#include "AISystem.h"
#include "BrainComponent.h"
#include "Engine/Canvas.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "Navigation/MetaNavMeshPath.h"
#include "NavigationSystem.h"
#include "NavLinkCustomInterface.h"
#include "NavMesh/RecastNavMesh.h"
#include "TimerManager.h"
#include "UObject/Package.h"
#include "VisualLogger/VisualLogger.h"
#include "VisualLogger/VisualLoggerTypes.h"

void UPathFollowingNoStopComponent::OnPathFinished(const FPathFollowingResult& Result)
{
	UE_VLOG(GetOwner(), LogPathFollowing, Log, TEXT("OnPathFinished: %s"), *Result.ToString());

	INavLinkCustomInterface* CustomNavLink = Cast<INavLinkCustomInterface>(CurrentCustomLinkOb.Get());
	if (CustomNavLink)
	{
		CustomNavLink->OnLinkMoveFinished(this);
		CurrentCustomLinkOb.Reset();
	}

	// update meta path if needed
	if (bIsUsingMetaPath && Result.IsSuccess() && MovementComp)
	{
		FMetaNavMeshPath* MetaNavPath = Path->CastPath<FMetaNavMeshPath>();
		const bool bNeedPathUpdate = MetaNavPath && MetaNavPath->ConditionalMoveToNextSection(MovementComp->GetActorFeetLocation(), EMetaPathUpdateReason::PathFinished);
		if (bNeedPathUpdate)
		{
			// keep move request active
			return;
		}
	}

	// save move status
	bLastMoveReachedGoal = Result.IsSuccess() && !HasPartialPath();

	// save data required for observers before reseting temporary variables
	const FAIRequestID FinishedMoveId = GetCurrentRequestId();

	Reset();
	UpdateMoveFocus();

	/*if (ShouldStopMovementOnPathFinished())
	{
		MovementComp->StopMovementKeepPathing();
	}*/

	// notify observers after state was reset (they can request another move)
	OnRequestFinished.Broadcast(FinishedMoveId, Result);

	FAIMessage Msg(UBrainComponent::AIMessage_MoveFinished, this, FinishedMoveId, Result.IsSuccess());
	Msg.SetFlag(Result.Flags & 0xff);
	FAIMessage::Send(Cast<AController>(GetOwner()), Msg);
}