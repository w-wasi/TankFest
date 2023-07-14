// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UTankMovementComponent::SetSpeedAndTurn(float SpeedToSet, float TurnToSet)
{
	TankSpeed = SpeedToSet;
	TankTurnRate = TurnToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	IntendMoveForward(ForwardThrow);

	auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	IntendTurnRight(RightThrow);
}

void UTankMovementComponent::IntendMoveForward(float Axis)
{
	auto DeltaLocation = FVector(0);
	auto TankBase = Cast<APawn>(GetOwner());
	auto AdjustedAxis = FMath::Clamp<float>(Axis, -1, 1);

	DeltaLocation.X = AdjustedAxis * TankSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	TankBase->AddActorLocalOffset(DeltaLocation);
}

void UTankMovementComponent::IntendTurnRight(float Axis)
{
	auto DeltaRotation = FRotator::ZeroRotator;
	auto TankBase = Cast<APawn>(GetOwner());
	auto AdjustedAxis = FMath::Clamp<float>(Axis, -1, 1);

	DeltaRotation.Yaw = AdjustedAxis * TankTurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	TankBase->AddActorLocalRotation(DeltaRotation, true);
}


