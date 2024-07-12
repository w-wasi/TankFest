// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_BattleTankGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"




AActor* AMP_BattleTankGameMode::ChoosePlayerStart_Implementation(AController* Player)
{

	TArray<AActor*> PlayerStarts;
	TSubclassOf<APlayerStart> PlayerStart = APlayerStart::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStart, PlayerStarts);

	return PlayerStarts[++PlayerStartIndex];
}

void AMP_BattleTankGameMode::DestroyTank(APawn* PawnToBeKilled)
{
    APlayerController* PlayerController = Cast<APlayerController>(PawnToBeKilled->GetController());

    //lose condition 
    if (PlayerController != nullptr)
    {
        EndGame(false);
    }

    //win condition 
    EndGame(true);

}

void AMP_BattleTankGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);

    }
}
