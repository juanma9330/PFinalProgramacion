// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/PF_GameMode.h"

#include "Framework/PF_PlayerState.h"

APF_GameMode::APF_GameMode()
{
	bDelayedStart = true;
}

void APF_GameMode::StartMatch()
{
	Super::StartMatch();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("comenzo la partida"));
}

void APF_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TiempoEsperaInicioTimerHandle, this, &APF_GameMode::StartMatch,MatchStartTimeDuration,false);
}

void APF_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	APF_PlayerState* PS = Cast<APF_PlayerState>(NewPlayer->PlayerState);
	if (PS)
	{
		ETeam team = (GetNumPlayers()%2 ==0) ? ETeam::Blue : ETeam::Red;
		PS->SetTeamColor(team);
	}
	
}

void APF_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

