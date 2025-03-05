// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OTOScoreWB.generated.h"

class UTextBlock;

UCLASS()
class STARCRAFT_API UOTOScoreWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	FTimerHandle WinCheckTimer;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* BlueTeamScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* RedTeamScoreText;

	UPROPERTY(BlueprintReadWrite)
	int32 BlueTeamScore = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 RedTeamScore = 0;

private:
	void WinCheckTimerHandle();

protected:
	UFUNCTION(BlueprintCallable)
	void SetNewScoreFromServer();

	UFUNCTION(BlueprintCallable)
	void SetWinCheckTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void FinishRound();

public:
	virtual void NativeConstruct() override;

	void SetNewScoreFromServer_Impl(int32 NewBlueTeamScore, int32 NewRedTeamScore);
	void GetScores(int32& CurrBlueTeamScore, int32& CurrRedTeamScore);
};
