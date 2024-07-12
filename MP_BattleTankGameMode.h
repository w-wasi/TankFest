// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPersonGameMode.h"
#include "MP_BattleTankGameMode.generated.h"

/**
 * 
 */



UCLASS()
class BATTLETANK_API AMP_BattleTankGameMode : public ATP_ThirdPersonGameMode
{
	GENERATED_BODY()

public:
	void DestroyTank(APawn* PawnToBeKilled);

	
protected:

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	int32 PlayerStartIndex = -1;


private:
	void EndGame(bool bIsPlayerWinner);
};
