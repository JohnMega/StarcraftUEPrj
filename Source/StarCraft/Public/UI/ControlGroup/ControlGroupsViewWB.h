// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "ControlGroupsViewWB.generated.h"

class UHorizontalBox;
class UControlGroupWB;
class ASCAICharacter;

UCLASS()
class STARCRAFT_API UControlGroupsViewWB : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ControlGroupsBox;

	UPROPERTY(meta = (BindWidget))
	UControlGroupWB* EmptyControlGroup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Init Vars")
	int32 ControlGroupsMaxCount = 3;

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void ClearAllControlGroups();

	UControlGroupWB* AddNewControlGroup(const TArray<ASCAICharacter*>& ControlGroupUnits, int32 BindButtonNum, int32 ControlGroupNum);
	TArray<UWidget*> GetControlGroupsBoxChilds() { return ControlGroupsBox->GetAllChildren(); }
	UHorizontalBox* GetControlGroupsBox() { return ControlGroupsBox; }
};
