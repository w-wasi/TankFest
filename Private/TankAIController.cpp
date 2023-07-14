// Fill out your copyright notice in the Description page of Project Settings.
// xyzAIcontroller.h should be the first header included must

#include "TankAIController.h"
#include "Tank.h"

	/* Getters for Player and AI tanks */

ATank* ATankAIController::GetControlledTank() const {

	return  Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	//needs to be checked because game can exist without player
	// i.e, in losing scenario.
	if (PlayerPawn == nullptr) {
		return nullptr;
	}

	return  Cast<ATank>(PlayerPawn);

}

	/* Engine Overridden */

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = FPlatformTime::Seconds();

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* In every tick */

	// move to player
	if (GetPlayerTank() != nullptr) {

		MoveToActor(GetPlayerTank(), AcceptanceRadius); //Function given by Unreal
	}

	//aim at player
	AimTowardsPlayer();

}

void ATankAIController::AimTowardsPlayer()
{
	if (!GetControlledTank()) {
		return;
	}

	if (!GetPlayerTank()) {
		return;
	}
	
	//Aim at player
	bool bValidTareget = GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

	if((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds && GetControlledTank()->GetAmmunationRounds() > 0) {

		// Only fire if player is in sight; else no need to waste Ammo
		if (bValidTareget) {

			GetControlledTank()->TankFire();

			LastFireTime = FPlatformTime::Seconds();

			//Decrement tank Ammo on succesful fire
			GetControlledTank()->SetAmmunationRounds(GetControlledTank()->GetAmmunationRounds() - 1 );
		}
			
	}
	
}


