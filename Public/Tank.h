// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"


class ATankPlayerController;
class UTankAimingComponent;
class UTankMovementComponent;
class UTexture2D;



UENUM()
enum class EFiringStatus : uint8
{
	OutOfAmmo,		//OutOfAmmo			color: GREY
	OutOfRange,		//OutOfRange		color: RED
	Reloading,		//Reloading			color: YELLOW
	ValidTarget		//ValidTarget		color: GREEN	
};


UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// C-tor
	ATank();

	/* Tank functionailities */
	void Move(float axis);
	void Turn(float axis);

	void FireButtonPressed();
	void FireButtonReleased();

	/* Setting tank characteristics */

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	
	bool IsTankDestroyed();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	/* Getters & Setters */
	float GetAmmunationRounds();
	
	void SetFiringStatus(EFiringStatus FiringStatusToSet);

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	bool CanFire();
	bool CanTarget();

	FORCEINLINE UTankMovementComponent* GetTankMoveComp() { return TankMovementComponent; }
	

protected:

	/* Tank Data members */

	ATankPlayerController* PlayerController = nullptr;

	UTankMovementComponent* TankMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTankAimingComponent* TankAimingComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Reloading;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	int32 StartingHealth = 150;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	int32 CurrentHealth = 150; // Initialised in Begin Play

	UFUNCTION()
	void OnRep_Health();



	UPROPERTY(EditAnywhere, Category = "Firing")
	float LaunchSpeed = 2700;

	UPROPERTY(EditAnywhere, Category = "TankMovement")
	float MoveSpeed = 100;

	UPROPERTY(EditAnywhere, Category = "TankMovement")
	float TurnRate = 45;

  
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void SetupMovementComponent();

	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void SetupAimingComponent();

	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void SetupHealthWidgetComponent();

public:
		/**
	* Textures for the weapon crosshairs
	*/

		UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsCenter;

		UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsLeft;

		UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsRight;

		UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsTop;

		UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsBottom;

};

