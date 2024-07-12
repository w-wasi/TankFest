// Fill out your copyright notice in the Description page of Project Settings.
// xyzPlayercontroller.h should be the first header included must

#include "TankPlayerController.h"
#include "Tank.h"
#include "TankHUD.h"
#include "PawnOverlay.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


ATank* ATankPlayerController::GetControlledTank() const {

	return  Cast<ATank>(GetPawn());

}


/* Engine Overridden Functions */

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<ATankHUD>(GetHUD());
	
	if (!GetControlledTank()) {
		return;
	}

	// Dummy status at start, re-written in Tick()
	GetControlledTank()->SetFiringStatus(EFiringStatus::OutOfAmmo);
	HUD->AddPawnOverlay();
	SetHUDHealth();

}


void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetControlledTank()) {
		return;
	}


	if (GetControlledTank()->GetAmmunationRounds() > 0) {

		if (GetControlledTank()->CanTarget()) {

			if (GetControlledTank()->CanFire()) {

				GetControlledTank()->SetFiringStatus(EFiringStatus::ValidTarget);
				ColorToSet = FLinearColor::Green;
			}
			else {

				GetControlledTank()->SetFiringStatus(EFiringStatus::Reloading);
				ColorToSet = FLinearColor::Yellow;
			}
		}
		else {
			GetControlledTank()->SetFiringStatus(EFiringStatus::OutOfRange);
			ColorToSet = FLinearColor::Red;
		}
	}
	else {
		GetControlledTank()->SetFiringStatus(EFiringStatus::OutOfAmmo);
		ColorToSet = FLinearColor::Gray;
	}

	SetHUDCrosshairs(DeltaTime);
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

/* 

	HUD Functions 


*/

void ATankPlayerController::SetHUDCrosshairs(float DeltaTime)
{
	HUD = HUD == nullptr ? Cast<ATankHUD >(GetHUD()) : HUD;
	if (HUD)
	{
		FHUDPackage HUDPackage;
		if (GetControlledTank())
		{
			HUDPackage.CrosshairsCenter = GetControlledTank()->CrosshairsCenter;
			HUDPackage.CrosshairsLeft = GetControlledTank()->CrosshairsLeft;
			HUDPackage.CrosshairsRight = GetControlledTank()->CrosshairsRight;
			HUDPackage.CrosshairsBottom = GetControlledTank()->CrosshairsBottom;
			HUDPackage.CrosshairsTop = GetControlledTank()->CrosshairsTop;
		}
		else
		{
			HUDPackage.CrosshairsCenter = nullptr;
			HUDPackage.CrosshairsLeft = nullptr;
			HUDPackage.CrosshairsRight = nullptr;
			HUDPackage.CrosshairsBottom = nullptr;
			HUDPackage.CrosshairsTop = nullptr;
		}
		HUD->SetHUDPackage(HUDPackage, ColorToSet);
	}
}

void ATankPlayerController::SetHUDHealth()
{
	HUD = HUD == nullptr ? Cast<ATankHUD >(GetHUD()) : HUD;

	bool bHUDValid = HUD &&
		HUD->GetPawnOverlay() &&
		HUD->GetPawnOverlay()->GetHealthBar();

	if (bHUDValid && GetControlledTank()) {
		const float HealthPercent = GetControlledTank()->GetHealth();
		HUD->GetPawnOverlay()->GetHealthBar()->SetPercent(HealthPercent);
	}

}

void ATankPlayerController::UpdateHUDHealth()
{
	SetHUDHealth();
}

/*

	Game Shutdown Functions


*/

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

		StartSpectatingOnly();
		
	}

	ShowButtonComponent();
}

void ATankPlayerController::EndGame()
{
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(GetWorld(), this, QuitPreference, true);
}







