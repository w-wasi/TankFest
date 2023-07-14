// Fill out your copyright notice in the Description page of Project Settings.
// xyzPlayercontroller.h should be the first header included must

#include "TankPlayerController.h"
#include "Tank.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"

ATank* ATankPlayerController::GetControlledTank() const {

	return  Cast<ATank>(GetPawn());
}

/* Engine Overridden Functions */

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetControlledTank()) {
		return;
	}

	// Dummy status at start, re-written in Tick()
	GetControlledTank()->SetFiringStatus(EFiringStatus::OutOfAmmo);
	LastFireTime = FPlatformTime::Seconds();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetControlledTank()) {
		return;
	}

	TimeElapsed = FPlatformTime::Seconds() - LastFireTime;
	bTankReloaded = TimeElapsed > ReloadTimeInSeconds;

	// Crosshair Status gives rutime value of cross-hair on player screen
	// and  returns true if player is looking at valid target
	bool bValidRange = bCrosshairStatus; 

	if (GetControlledTank()->GetAmmunationRounds() > 0) {
		if (bValidRange) {

			if (bTankReloaded) {

				GetControlledTank()->SetFiringStatus(EFiringStatus::ValidTarget);
			}
			else {

				GetControlledTank()->SetFiringStatus(EFiringStatus::Reloading);
			}
		}
		else {
			GetControlledTank()->SetFiringStatus(EFiringStatus::OutOfRange);
		}
	}
	else {
		GetControlledTank()->SetFiringStatus(EFiringStatus::OutOfAmmo);
	}

	// Aiming at crosshair everytick
	AimTowardsCrosshair();
}

void ATankPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner) {

		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}

	}
	else {

		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}

		
		//Quit game in 10 seconds if no choice is made by player
		FTimerHandle ChoiceHandle;
		GetWorldTimerManager().SetTimer(ChoiceHandle, this, &ATankPlayerController::EndGame, 10.0f, false);
		
		StartSpectatingOnly();
		
	}

	ShowButtonComponent();
}

void ATankPlayerController::EndGame()
{
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(GetWorld(), this, QuitPreference, true);
}


void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);

	InputComponent->BindAxis(TEXT("MoveForward"), this, &ATankPlayerController::Move);
	InputComponent->BindAxis(TEXT("Turn"), this, &ATankPlayerController::Turn);

}

/* Player input dependant functions */

void ATankPlayerController::Fire()
{
	if (!GetControlledTank()) {
		return;
	}

	if (bTankReloaded && GetControlledTank()->GetAmmunationRounds() > 0) {
			GetControlledTank()->TankFire();
			LastFireTime = FPlatformTime::Seconds();

			//set tank ammo prop here
			GetControlledTank()->SetAmmunationRounds(GetControlledTank()->GetAmmunationRounds() - 1);
	}

}

void ATankPlayerController::Move(float axis)
{
	if (!GetControlledTank()) {
		return;
	}
	GetControlledTank()->Move(axis);
}

void ATankPlayerController::Turn(float axis)
{
	if (!GetControlledTank()) {
		return;
	}
	GetControlledTank()->Turn(axis);
}

/* Crosshair logic function & sub-function(s) */

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) {
		return;
	}

	FVector HitLocation; // Out parameter
	bool bValidHitLocation = GetSightRayHitLocation(HitLocation);

	if (bValidHitLocation) {
		
		bCrosshairStatus = GetControlledTank()->AimAt(HitLocation);
	}
	
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY); //returns the pixles of screen like 600x800, 1080x1920 etc
	
	// Adjusting with the same ratio as anchor of the crosshair in BP by
	// multiplying (in reality dividing by 1/2 and 1/5 to get the actual crosshair loc on screen)
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	FVector LookDirection; //To be filled with a UNIT VECTOR in 3D space in the world at the crosshair's location.

	if (GetLookDirection(ScreenLocation, LookDirection)) {

		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return false;
}

/** converts 2D location(our viewport) to 3D(actual in - game)
	basically making crosshair workable by making it a vector along z axis in world from starting point
	from corrsshair by using DeprojectScreenPositionToWorld() method of Unreal
*/
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be discarded

	return  DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * MaxRange); //TODO max range needs to be checked
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Camera)
		) {
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false; // Line trace didn't succeed
}




