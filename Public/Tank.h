// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"


class UTankAimingComponent;
class UTankMovementComponent;



UENUM()
enum class EFiringStatus : uint8
{
	OutOfAmmo,		//OutOfAmmo		needs to be added	color: GREY
	OutOfRange,		//OutOfRange	needs to be renamed	color: RED
	Reloading,		//Reloading		needs to be renamed	color: YELLOW
	ValidTarget		//ValidTarget	needs to be renamed	color: GREEN	
};


UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/* Setting tank characteristics */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SetupMovementComponent();
	void SetFiringStatus(EFiringStatus FiringStatusToSet);
	
	/* Getters & Setters */
	float GetAmmunationRounds();
	void SetAmmunationRounds(float Rounds);
	
	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	/* Tank functionailities */
	bool AimAt(FVector GivenTarget);
	void TankFire();
	void Move(float axis);
	void Turn(float axis);
	bool IsTankDestroyed();

protected:

	/* Tank Data members */

	UTankMovementComponent* TankMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTankAimingComponent* TankAimingComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Reloading;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Setup)
	float AmmunationRounds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth; // Initialised in Begin Play


	UPROPERTY(EditAnywhere, Category = "Firing")
	float LaunchSpeed = 10000;

	UPROPERTY(EditAnywhere, Category = "TankMovement")
	float MoveSpeed = 100;

	UPROPERTY(EditAnywhere, Category = "TankMovement")
	float TurnRate = 45;

	/* Protected functions; might be needed in Bluprint */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void SetupAimingComponent();

	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void SetupHealthWidgetComponent();


};
