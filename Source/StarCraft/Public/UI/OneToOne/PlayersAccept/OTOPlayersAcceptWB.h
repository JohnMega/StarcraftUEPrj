// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OTOPlayersAcceptWB.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class STARCRAFT_API UOTOPlayersAcceptWB : public UUserWidget
{
	GENERATED_BODY()

	static const int32 MAX_PLAYERS_NUM = 2;

private:
	int32 AcceptScore = 0;
	bool bIsAcceptLocal = false;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* AcceptButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AcceptScoreText;

private:
	UFUNCTION()
	void OnAcceptButtonClicked();

public:
	virtual void NativeConstruct() override;

	void OnAcceptButtonClicked_Impl(bool bIsAccept);
	void UpdateAcceptScore_Impl(int32 NewAcceptScore);
	int32 GetAcceptsNum() { return AcceptScore; }
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAllPlayersAccept();
};
