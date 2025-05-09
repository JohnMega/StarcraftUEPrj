// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SCAIController.generated.h"

class UBehaviorTree;
class UAIPerceptionComponent;

USTRUCT(BlueprintType)
struct FCharacterStartData
{
	GENERATED_USTRUCT_BODY()

public:
	FVector StartCharacterPosition;
	bool IsCharacterFriendly;
	UClass* CharacterClass;
};

UCLASS()
class STARCRAFT_API ASCAIController : public AAIController
{
	GENERATED_BODY()

private:
	class UAISenseConfig_Sight* SightConfig;
	FCharacterStartData CharacterStartData;
	ETeamAttitude::Type TeamAttitude = ETeamAttitude::Hostile;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAIPerceptionComponent* MyPerceptionComponent;

protected:
	void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override { return TeamAttitude; }

public:
	ASCAIController(const FObjectInitializer& Init);

	void SetTeamAttitude(ETeamAttitude::Type NetTeamAttitude);

	UFUNCTION(BlueprintCallable)
	const FCharacterStartData& GetCharacterStartData() const;

	UFUNCTION(BlueprintCallable)
	void SetCharacterClass(UClass* CharacterClass);
};
