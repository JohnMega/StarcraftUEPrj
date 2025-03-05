// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SCGameModeBase.generated.h"

UCLASS()
class STARCRAFT_API ASCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void Logout(AController* Exiting) override;

public:
	ASCGameModeBase();
};
