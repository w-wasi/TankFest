// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */

class ATank;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 800;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTimeInSeconds = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	double LastFireTime = 0;

	/* Engine Overridden */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	/* Getters for Player and AI tanks */

	ATank* GetControlledTank() const;

	ATank* GetPlayerTank() const;

	// Function to target player tank
	void AimTowardsPlayer();
};
