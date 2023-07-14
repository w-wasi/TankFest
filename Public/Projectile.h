// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;
class USoundBase;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UStaticMeshComponent* CollisionMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UParticleSystemComponent* LaunchBlast = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UParticleSystemComponent* ImpactBlast = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	URadialForceComponent* ExplosionForce = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelay = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileDamage = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	USoundBase* LaunchSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	USoundBase* HitSound = nullptr;

public:	

	void LaunchProjectile(float Speed);

private:

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	void OnTimerExpire();

};
