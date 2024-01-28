// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"

void AToonTanksPlayerController::SetPlayerEnabledState(const bool bPlayerEnabled)
{
	bShowMouseCursor = bPlayerEnabled;
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
		return;
	}

	GetPawn()->DisableInput(this);
}
