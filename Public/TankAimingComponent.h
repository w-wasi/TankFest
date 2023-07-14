// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool AimAt(FVector GivenTarget, float LaunchSpeed);

	void MoveBarrelTowards(FVector TargetAimDirection);

	FVector GetBarrelSocketLocation();

	void FireProjectile(float LaunchSpeed);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	//Blueprint callable not implementable i.e, taking inputs from BP
	//but setting them in C++
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetBarrelAndTurret(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

private:
	UTankBarrel* Barrel = nullptr;

	UTankTurret* Turret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;
		
};
