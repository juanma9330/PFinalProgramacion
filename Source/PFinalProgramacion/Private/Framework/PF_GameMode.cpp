// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/PF_GameMode.h"

#include "Framework/PF_GameState.h"
#include "Framework/PF_PlayerState.h"

APF_GameMode::APF_GameMode()
{
	bDelayedStart = true;
	MatchStartTimeDuration = 5.0f;
	DuracionPartida = 60.0f;
	TiempoPostPartida = 5.0f;
}

void APF_GameMode::StartMatch()
{
	Super::StartMatch();
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("comenzo la partida"));
	if (APF_GameState* GS = GetGameState<APF_GameState>())
	{
		GS->StartMatch(DuracionPartida);
	}
	GetWorldTimerManager().SetTimer(DuracionPartidaHandle,this, &APF_GameMode::TerminotiempoPartida, DuracionPartida, false);
}

void APF_GameMode::BeginPlay()
{
	Super::BeginPlay();
	if (APF_GameState* GS = GetGameState<APF_GameState>())
	{
		GS->SetTiempoInicial(DuracionPartida);
	}
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

void APF_GameMode::CheckWinCondition()
{
}

void APF_GameMode::DeclareWinner()
{
	APF_GameState* GS = GetGameState<APF_GameState>();
	if (!GS) return;
	
	const  int32 PuntajeAzul = GS->GetPuntajeEquipo(ETeam::Blue);
	const  int32 PuntajeRojo = GS->GetPuntajeEquipo(ETeam::Red);
	
	ETeam EquipoGanador = ETeam::None;

	if (PuntajeAzul > PuntajeRojo)
	{
		EquipoGanador = ETeam::Blue;
	}
	else if (PuntajeAzul < PuntajeRojo)
	{
		EquipoGanador = ETeam::Red;
	}
	
	GS->SetGameOver(EquipoGanador);
	
	GetWorldTimerManager().SetTimer(ExpulsarJugadoresTimerHandle,this,&APF_GameMode::ExpulsarJugadores, TiempoPostPartida,false);
}



void APF_GameMode::TerminotiempoPartida()
{
	DeclareWinner();
}

void APF_GameMode::ExpulsarJugadores()
{ 
	//PROVISORIO HASTA TENER UN MAPA DE LOBBY Y PODER RETORNAR AL MAPA
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel("?Restart",false);
	}
	
}


