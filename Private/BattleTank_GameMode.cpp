// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTank_GameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "TankAIController.h"
#include "Tank.h"

void ABattleTank_GameMode::DestroyTank(ATank* TankToBeKilled)
{
    APlayerController* PlayerController = Cast<APlayerController>(TankToBeKilled->GetController());

    //lose condition 
    if (PlayerController != nullptr)
    {
        // PlayerController->GameHasEnded(nullptr, false);
        //PlayerController->StartSpectatingOnly();
        EndGame(false);
    }

    for (auto Controller : TActorRange<ATankAIController>(GetWorld()))
    {
        if (!Controller->GetControlledTank()->IsTankDestroyed())
        {
            return;
        }
        else {
           // Controller->GetControlledTank()->DetachFromControllerPendingDestroy();
        }
    }

    EndGame(true);

	
}


void ABattleTank_GameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
        
    }
   
}