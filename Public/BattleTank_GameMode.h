// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleTank_GameMode.generated.h"

/**
 * 
 */

class ATank;

UCLASS()
class BATTLETANK_API ABattleTank_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void DestroyTank(ATank* PawnToBeKilled);

private:
	void EndGame(bool bIsPlayerWinner);

};
