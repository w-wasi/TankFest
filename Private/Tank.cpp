
// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "../TankPlayerController.h"
#include "../MP_BattleTankGameMode.h"

/* Engine Overridden Functions */

// Sets default values
ATank::ATank()
{

	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true); 
	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 70.0f;

	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>("Tank Aiming Component");
	TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>("Tank Movement Component");

}

void ATank::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(ATank, CurrentHealth);
}

void ATank::OnRep_Health()
{
	if(PlayerController){
		PlayerController->UpdateHUDHealth();
	}

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	SetupMovementComponent();
	TankMovementComponent->SetAllowedToMove(true);

	SetupAimingComponent();
	TankAimingComponent->SetAllowedToFire(true);

	SetupHealthWidgetComponent();

	PlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ATank::ReceiveDamage);
	}
}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TankAimingComponent->AimTowardsCrosshair();
}


// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATank::FireButtonReleased);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
}

void ATank::SetupMovementComponent()
{
	TankMovementComponent->SetSpeedAndTurn(MoveSpeed, TurnRate);
}


void ATank::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(Damage);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	PlayerController->UpdateHUDHealth();

	if (IsTankDestroyed())
	{


		auto GameMode = GetWorld()->GetAuthGameMode<AMP_BattleTankGameMode>();

		if (GameMode != nullptr)
		{
			GameMode->DestroyTank(this);
		}
		TankMovementComponent->SetAllowedToMove(false);
		TankAimingComponent->SetAllowedToFire(false);
		DisableInput(PlayerController);


	}
}


/* Tank functionailities */


void ATank::Move(float axis)
{
	
	TankMovementComponent->MoveTankForward(axis);
}


void ATank::Turn(float axis)
{
	
	TankMovementComponent->TurnTankRight(axis);
}

void ATank::FireButtonPressed()
{

	if (TankAimingComponent) {
		TankAimingComponent->FireButtonPressed(true);
	}
}

void ATank::FireButtonReleased()
{
	if (TankAimingComponent) {
		TankAimingComponent->FireButtonPressed(false);
	}
}

bool ATank::IsTankDestroyed()
{
	return CurrentHealth <= 0;
}


/* Getters & Setters */

void ATank::SetFiringStatus(EFiringStatus FiringStatusToSet)
{
	FiringStatus = FiringStatusToSet;
}

float ATank::GetHealth() const {

	return (float)CurrentHealth / (float)StartingHealth;
}


float ATank::GetAmmunationRounds()
{
	return TankAimingComponent->AvailableAmmo();
}


bool ATank::CanFire()
{
	return TankAimingComponent->Reloaded();
}

bool ATank::CanTarget()
{
	return TankAimingComponent->ValidRange();
}
