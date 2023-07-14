// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */

class ATank;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	
	GENERATED_BODY()

protected:
	
	float ReloadTimeInSeconds = 3.f;
	double LastFireTime = 0;
	double TimeElapsed = 0;
	bool bTankReloaded = false;
	bool bCrosshairStatus = false;

	UPROPERTY(EditDefaultsOnly)
	float MaxRange = 10000; // this working perfectly with launch speed of 2700 for player

	UPROPERTY(VisibleDefaultsOnly)
	float CrosshairXLocation = 0.5;

	UPROPERTY(VisibleDefaultsOnly)
	float CrosshairYLocation = 0.2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	/* Engine Overriden Functions */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupInputComponent() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	/* Custom Functions */

	UFUNCTION(BlueprintCallable)
	ATank* GetControlledTank() const;

	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(FVector& HitLocation) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	void EndGame();


	void Fire();
	void Move(float axis);
	void Turn(float axis);

	UFUNCTION(BlueprintImplementableEvent, Category = "End")
	void ShowButtonComponent();

};
