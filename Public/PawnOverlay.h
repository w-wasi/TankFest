// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PawnOverlay.generated.h"

/**
 * 
 */

class UProgressBar;

UCLASS()
class BATTLETANK_API UPawnOverlay : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

public:
	UProgressBar* GetHealthBar();
};
