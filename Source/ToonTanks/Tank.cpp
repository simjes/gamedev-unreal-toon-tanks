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

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
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

void ATank::Fire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Firing"));
}

void ATank::RotateCamera(const FInputActionValue& Value)
{
	const float Direction = Value.Get<float>();
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Direction * DeltaTime * TurnSpeed;
	SpringArm->AddWorldRotation(DeltaRotation);
	TurretMesh->AddWorldRotation(DeltaRotation);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATank::RotateCamera);
		// EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATank::Fire);
	}
}
