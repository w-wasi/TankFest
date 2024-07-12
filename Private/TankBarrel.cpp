// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"

void UTankBarrel::Elevate(float RelativeSpeed) {

	// Move the barrel the right amount this frame
	// Given a max elevation speed, and the frame time
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = GetRelativeRotation().Pitch + ElevationChange;
	auto Elevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	SetRelativeRotation(FRotator(Elevation, 0, 0));

}

void UTankBarrel::ShootTarget(float LaunchSpeed)
{
	if (!ProjectileBlueprint) { 
		return;
	}
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		GetSocketLocation(FName("Projectile")),
		GetSocketRotation(FName("Projectile"))
	);

	Projectile->LaunchProjectile(LaunchSpeed);
}
