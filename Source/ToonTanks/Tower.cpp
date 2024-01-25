// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	GetWorldTimerManager()
		.SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}


void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVector TankLocation; InFireRange(TankLocation))
	{
		RotateTurret(TankLocation);
	}
}

void ATower::CheckFireCondition()
{
	if (FVector TankLocation; InFireRange(TankLocation))
	{
		Fire();
	}
}

bool ATower::InFireRange(FVector& OutTankLocation) const
{
	if (Tank)
	{
		OutTankLocation = Tank->GetActorLocation();
		const float Distance = FVector::Dist(GetActorLocation(), OutTankLocation);
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	
	return false;
}
