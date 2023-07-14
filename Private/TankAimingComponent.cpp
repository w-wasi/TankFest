// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...


}

void UTankAimingComponent::SetBarrelAndTurret(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

FVector UTankAimingComponent::GetBarrelSocketLocation()
{
	if (!Barrel) { return FVector(0); }
	return Barrel->GetSocketLocation("Projectile");
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

/*
// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
*/

bool UTankAimingComponent::AimAt(FVector GivenTarget, float LaunchSpeed)
{
	
	bool bValidAim = false;

	if (!Barrel) { return bValidAim; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation("Projectile");

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		GivenTarget,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace // Paramater must be present to prevent bug
	);


	
	/*
		NOTES:

		vector addition (A+B) is given by resultant vector from the tail of A to the head of B.

		In this case, we have StartLocation & GivenTarget in the form of Vectors not co-ordinates;
		meaning that they are essentially drawn from Origion (0,0,0) to some point p(x,y,z) respectively.

		Furthermore, in our case, StartLocation is A vector and the GivenTarget is actually (A+B) already.
		Hence, what we are intrested in actually is SUBTRACTION of vectors (B-A) becuase [A + (B-A) = B]

		This (B-A) vector is calculated by the said functiona and is captured in OutParameter OutLaunchVel.
		
		The said function takes other paramters to adjust the parabolic path (gravity etc) but we have assumed
		them to be zero for simplicity.

		LaunchSpeed paramter we didnt ignore. why? because, A vector by definition is Magnitude * UnitVector.
		The LaunchSpeed here is THE magnitude; hence, utilized in calculation of Vector.

	*/

	
	if (bHaveAimSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal(); 
		/*	
		
			NOTES:

			The reason we are not using this OutVeloityVec is that
			we are intrested to rotate our barrel to the desired angle
			to fire bomb. This raw vactor is of no use because it doesnt
			describe us degrees direction.

		*/
		
		MoveBarrelTowards(AimDirection);
		bValidAim = true;
	}
	else {

		bValidAim = false;
	}

	return bValidAim;
}

void UTankAimingComponent::MoveBarrelTowards(FVector TargetAimDirection)
{

	if (!Barrel) { return; }

	if (!Turret) { return; }

	// Work-out difference between current barrel roation (Elevation), and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();	//get x-direction vector's rotation
	auto AimAsRotator = TargetAimDirection.Rotation(); 
	auto DeltaRotator = AimAsRotator - BarrelRotator; // difference i.e., how much we need to rotate barrel
													  // to target desired location.
	Barrel->Elevate(DeltaRotator.Pitch);

	//moving turret

	if (FMath::Abs(DeltaRotator.Yaw) < 180) {

		Turret->Rotate(DeltaRotator.Yaw);
	}
	else {

		Turret->Rotate(-DeltaRotator.Yaw);
	}

}

void UTankAimingComponent::FireProjectile(float LaunchSpeed)
{
	if (!ProjectileBlueprint) { return; }

	if (!Barrel) { return; }

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))
	);


	Projectile->LaunchProjectile(LaunchSpeed);

}
