// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ControlGroup/ControlGroupsViewWB.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/ControlGroup/ControlGroupWB.h"
#include "AI/SCAICharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HealthComponent.h"

void UControlGroupsViewWB::NativeConstruct()
{
	Super::NativeConstruct();

	ControlGroupsBox->ClearChildren();

	for (int32 i = 0; i < ControlGroupsMaxCount; ++i)
	{
		UControlGroupWB* ControlGroupWB = DuplicateObject(EmptyControlGroup, this);
		ControlGroupsBox->AddChild(ControlGroupWB);
		ControlGroupWB->SetVisibility(ESlateVisibility::Visible);
		
		TArray<ASCAICharacter*> EmptyControlGroupUnits;
		ControlGroupWB->OnUpdateControlGroupInfo(EmptyControlGroupUnits, i + 1, -1);
	}
}

UControlGroupWB* UControlGroupsViewWB::AddNewControlGroup(const TArray<ASCAICharacter*>& ControlGroupUnits, int32 BindButtonNum, int32 ControlGroupNum)
{
	TArray<UWidget*> AllControlGroups = ControlGroupsBox->GetAllChildren();
	UControlGroupWB* ControlGroupWB = Cast<UControlGroupWB>(AllControlGroups[BindButtonNum - 1]);
	ControlGroupWB->OnUpdateControlGroupInfo(ControlGroupUnits, BindButtonNum, ControlGroupNum);

	return ControlGroupWB;
}

void UControlGroupsViewWB::ClearAllControlGroups()
{
	TArray<UWidget*> AllControlGroups = ControlGroupsBox->GetAllChildren();
	for (int32 i = 0; i < AllControlGroups.Num(); ++i)
	{
		UControlGroupWB* ControlGroupWB = Cast<UControlGroupWB>(AllControlGroups[i]);
		TArray<ASCAICharacter*> EmptyControlGroupUnits;
		ControlGroupWB->OnUpdateControlGroupInfo(EmptyControlGroupUnits, i + 1, -1);
	}
}