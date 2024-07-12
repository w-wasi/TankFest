// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:

	void SetSpeedAndTurn(float SpeedToSet, float TurnToSet);

	void IntendMoveForward(float Axis);

	void IntendTurnRight(float Axis);

	/* RPCs */

	
	void MoveTankForward(float Axis);

	void TurnTankRight(float Axis);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveTankForward(float Axis);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastMoveTankForward(float Axis);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTurnTankRight(float Axis);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTurnTankRight(float Axis);

	FORCEINLINE void SetAllowedToMove(bool flag) { bAllowedToMove = flag; };

private:

	
	float TankSpeed;

	float TankTurnRate;

	bool bAllowedToMove;
};
