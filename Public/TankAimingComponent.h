// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel;
class UTankTurret;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();

	void AimTowardsCrosshair();

	//shoudl be renamed to: TraceUnderCrosshair(OutHitLocation)
	bool GetSightRayHitLocation(FVector& HitLocation) const; //Rename

	//shoudl be renamed to: CalculateLookDirection(ScreenLocation,OutLookDirection)
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection, FVector& LookPosition) const;

	//shoudl be renamed to: GetSightRayHitLocation(LookDirection,OutHitLocation)
	bool GetLookVectorHitLocation(FVector LookPosition, FVector LookDirection, FVector& HitLocation) const;

	void Fire();
	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire();

	void MoveAim(FVector Target);

	UFUNCTION(Server, Reliable)
	void ServerMoveAim(FVector Target);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastMoveAim(FVector Target);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool AimAt(FVector GivenTarget, float LaunchSpeed);

	void MoveBarrelTowards(FVector TargetAimDirection);

	FVector GetBarrelSocketLocation();

	void FireProjectile(float LaunchSpeed);

	/* Required in PLC*/

	FORCEINLINE bool Reloaded() { return bTankReloaded; };
	FORCEINLINE bool ValidRange() { return bCrosshairStatus; };
	FORCEINLINE float AvailableAmmo() { return AmmunationRounds; }
	FORCEINLINE void SetAllowedToFire(bool flag) { bAllowedToFire = flag; }

protected:

	

	// Called when the game starts
	virtual void BeginPlay() override;

	//Blueprint callable not implementable i.e, taking inputs from BP
	//but setting them in C++
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetBarrelAndTurret(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

private:

	float DefaultLaunchSpeed = 2700;

	bool bFireButtonPressed = false;

	UTankBarrel* Barrel = nullptr;

	UTankTurret* Turret = nullptr;

	
protected:


	
	

	UPROPERTY(EditDefaultsOnly)
	float MaxRange = 2700; // this working perfectly with launch speed of 2700 for player

	UPROPERTY(VisibleDefaultsOnly)
	float CrosshairXLocation = 0.5;

	UPROPERTY(VisibleDefaultsOnly)
	float CrosshairYLocation = 0.2;


protected:

	float ReloadTimeInSeconds = 1.8f;
	float LastFireTime = 0.0f;
	float TimeElapsed = 0.0f;
	bool bTankReloaded = false;
	bool bCrosshairStatus = false;
	float AmmunationRounds = 3;
	bool bAllowedToFire;
		
};
