//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */

// UCLASS macro makes thing available in dropdwon components menu in BPs
// hidecategories flag hides and unhides the desired section

UCLASS(meta = (BlueprintSpawnableComponent)) //, hidecategories = ("Collision"))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	void Elevate(float RelativeSpeed);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxDegreesPerSecond = 15;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxElevationDegrees = 40;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MinElevationDegrees = 0;
	
};
