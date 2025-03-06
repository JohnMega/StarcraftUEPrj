// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UnitTestCameraWidgetComponent.h"
#include "UI/InGameMenuWB.h"
#include "UI/OneToOne/PlayersAccept/OTOPlayersAcceptWB.h"

void UUnitTestCameraWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Cast<APawn>(GetOwner())->GetController()) return;

	PlayersAcceptWidget = CreateWidget<UOTOPlayersAcceptWB>(GetWorld(), PlayersAcceptClass.Get());
	if (PlayersAcceptWidget)
	{
		CreatedWidgets.Add(PlayersAcceptWidget);
		PlayersAcceptWidget->AddToViewport();
	}
}