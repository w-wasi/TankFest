
// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BattleTank_GameMode.h"

/* Engine Overridden Functions */

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//disabling tick as it is costly
	PrimaryActorTick.bCanEverTick = false;

	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>("Tank Aiming Component");
	TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>("Tank Movement Component");

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	SetupMovementComponent();

	SetupAimingComponent();

	SetupHealthWidgetComponent();
	
	CurrentHealth = StartingHealth;

	//UGameplayStatics::OpenLevel(this, "MainMenu");
}

/*
// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;

	//UE_LOG(LogTemp, Warning, TEXT("The dmg value is: %i"), DamageToApply);
	UE_LOG(LogTemp, Warning, TEXT("The current health is: %i"), CurrentHealth);

	if (IsTankDestroyed())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Tank Death ourside!"));
		//OnDeath.Broadcast();
		//auto GameMode = GetWorld()->GetAuthGameMode<ABattleTank_GameMode>();

		auto GameMode = Cast<ABattleTank_GameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode != nullptr)
		{
			//GameMode->PawnKilled(this);
			GameMode->DestroyTank(this);
			//UE_LOG(LogTemp, Warning, TEXT("Tank Death!"));
		}

		DetachFromControllerPendingDestroy();
		
	}
	return DamageToApply;
}

/* Tank functionailities */

bool ATank::AimAt(FVector GivenTarget)
{
	return TankAimingComponent->AimAt(GivenTarget, LaunchSpeed);
}

void ATank::TankFire()
{	
	TankAimingComponent->FireProjectile(LaunchSpeed);
}

void ATank::Move(float axis)
{
	TankMovementComponent->IntendMoveForward(axis);
}

void ATank::Turn(float axis)
{
	TankMovementComponent->IntendTurnRight(axis);
}

bool ATank::IsTankDestroyed()
{
	return CurrentHealth <= 0;
}

void ATank::SetupMovementComponent()
{
	TankMovementComponent->SetSpeedAndTurn(MoveSpeed,TurnRate);
}

/* Getters & Setters */

void ATank::SetFiringStatus(EFiringStatus FiringStatusToSet)
{
	FiringStatus = FiringStatusToSet;
}

void ATank::SetAmmunationRounds(float Rounds)
{
	AmmunationRounds = Rounds;
}

float ATank::GetAmmunationRounds()
{
	return AmmunationRounds;
}

float ATank::GetHealth() const {

	return (float)CurrentHealth / (float)StartingHealth;
}
