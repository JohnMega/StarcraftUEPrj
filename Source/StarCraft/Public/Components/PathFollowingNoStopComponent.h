// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "PathFollowingNoStopComponent.generated.h"

UCLASS()
class STARCRAFT_API UPathFollowingNoStopComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
	
public:
	virtual void OnPathFinished(const FPathFollowingResult& Result) override;
};
