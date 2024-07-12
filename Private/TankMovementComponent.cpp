// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UTankMovementComponent::SetSpeedAndTurn(float SpeedToSet, float TurnToSet)
{
	TankSpeed = SpeedToSet;
	TankTurnRate = TurnToSet;
}

void UTankMovementComponent::MoveTankForward(float Axis)
{
	ServerMoveTankForward(Axis);
}

void UTankMovementComponent::ServerMoveTankForward_Implementation(float Axis)
{
	if (bAllowedToMove) {
		MulticastMoveTankForward(Axis);
	}
}

bool UTankMovementComponent::ServerMoveTankForward_Validate(float Axis)
{
	if (Axis >= -1 && Axis <= 1) {
		return true;
	}
	else {
		return false;
	}
}

void UTankMovementComponent::MulticastMoveTankForward_Implementation(float Axis)
{
	IntendMoveForward(Axis);
}

void UTankMovementComponent::IntendMoveForward(float Axis)
{
	auto DeltaLocation = FVector(0);
	auto TankBase = Cast<APawn>(GetOwner());
	auto AdjustedAxis = FMath::Clamp<float>(Axis, -1, 1);

	DeltaLocation.X = AdjustedAxis * TankSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	TankBase->AddActorLocalOffset(DeltaLocation);

}

void UTankMovementComponent::TurnTankRight(float Axis)
{
	ServerTurnTankRight(Axis);
}

void UTankMovementComponent::ServerTurnTankRight_Implementation(float Axis)
{
	if (bAllowedToMove) {
		MulticastTurnTankRight(Axis);
	}
}

bool UTankMovementComponent::ServerTurnTankRight_Validate(float Axis)
{
	if (Axis >= -1 && Axis <= 1) {
		return true;
	}
	else {
		return false;
	}
}

void UTankMovementComponent::MulticastTurnTankRight_Implementation(float Axis)
{
	IntendTurnRight(Axis);
}

void UTankMovementComponent::IntendTurnRight(float Axis)
{
	auto DeltaRotation = FRotator::ZeroRotator;
	auto TankBase = Cast<APawn>(GetOwner());
	auto AdjustedAxis = FMath::Clamp<float>(Axis, -1, 1);

	DeltaRotation.Yaw = AdjustedAxis * TankTurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	TankBase->AddActorLocalRotation(DeltaRotation, true);
	
}