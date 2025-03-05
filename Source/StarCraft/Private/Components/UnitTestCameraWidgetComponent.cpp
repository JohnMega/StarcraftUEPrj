// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UnitTestCameraWidgetComponent.h"
#include "UI/InGameMenuWB.h"
#include "UI/OneToOne/PlayersAccept/OTOPlayersAcceptWB.h"

void UUnitTestCameraWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Cast<APawn>(GetOwner())->GetController()) return;

	/*InGameMenuWidget = CreateWidget<UInGameMenuWB>(GetWorld(), InGameMenuClass.Get());
	if (InGameMenuWidget)
	{
		CreatedWidgets.Add(InGameMenuWidget);
		InGameMenuWidget->AddToViewport();
	}*/

	PlayersAcceptWidget = CreateWidget<UOTOPlayersAcceptWB>(GetWorld(), PlayersAcceptClass.Get());
	if (PlayersAcceptWidget)
	{
		CreatedWidgets.Add(PlayersAcceptWidget);
		PlayersAcceptWidget->AddToViewport();
	}

	//InGameMenuWidget->BeginPlay();
}