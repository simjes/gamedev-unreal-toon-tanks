// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction() override;

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TurnAction;
	void Turn(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	// UInputAction* RotateAction;
	// void RotateCamera(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, Category = "Pawn Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Pawn Components")
	float Speed = 600;

	UPROPERTY(EditAnywhere, Category = "Pawn Components")
	float TurnSpeed = 100;

	UPROPERTY()
	APlayerController* TankPlayerController;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
