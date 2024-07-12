// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TankHUD.generated.h"

/**
 * 
 */

class UTexture2D;
class UPawnOverlay;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
};

UCLASS()
class BATTLETANK_API ATankHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	void SetHUDPackage(const FHUDPackage& Package, const FLinearColor& ColorToSet);

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> PawnOverlayClass;

	UPawnOverlay* GetPawnOverlay();

	void AddPawnOverlay();

protected:

	virtual void BeginPlay() override;

	UPawnOverlay* PawnOverlay=nullptr;

private:
	FHUDPackage HUDPackage;
	FLinearColor Color;

	float CrosshairXLocation = 0.5;

	float CrosshairYLocation = 0.2;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter);
};
