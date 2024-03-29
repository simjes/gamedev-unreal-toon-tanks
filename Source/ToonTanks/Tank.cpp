// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<APlayerController>(GetController());

	if (TankPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::Move(const FInputActionValue& Value)
{
	const float Direction = Value.Get<float>();
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Direction * DeltaTime * Speed;

	// Enable sweep so we don't merge with walls
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(const FInputActionValue& Value)
{
	const float Direction = Value.Get<float>();
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Direction * DeltaTime * TurnSpeed;

	AddActorLocalRotation(DeltaRotation, true);
}

// Alternative implementation for rotating the turret (and the camera). Not reusable for non player turrets so not using it.
// void ATank::RotateCamera(const FInputActionValue& Value)
// {
// 	const float Direction = Value.Get<float>();
// 	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
//
// 	FRotator DeltaRotation = FRotator::ZeroRotator;
// 	DeltaRotation.Yaw = Direction * DeltaTime * TurnSpeed;
//
// 	// DrawLineTraces(this, ECC_Visibility, )
// 	// PlayerControllerRef->GetHitResultUnderCursor()
// 	SpringArm->AddWorldRotation(DeltaRotation);
// 	// TurretMesh->AddWorldRotation(DeltaRotation);
// }

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATank::Fire);
		// EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATank::RotateCamera);
		// EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATank::Fire);
	}
}
