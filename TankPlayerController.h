// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */

class ATank;
class ATankHUD;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	
	GENERATED_BODY()


protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	ATankHUD* HUD;

	FLinearColor ColorToSet = FLinearColor::White;

	/* Engine Overriden Functions */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupInputComponent() override;

	void SetHUDCrosshairs(float DeltaTime);

	void SetHUDHealth();


public:

	UFUNCTION(BlueprintCallable)
	ATank* GetControlledTank() const;

	void UpdateHUDHealth();

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	void EndGame(); 

	UFUNCTION(BlueprintImplementableEvent, Category = "End")
	void ShowButtonComponent();

};
