// Fill out your copyright notice in the Description page of Project Settings.
#include "Framework/PF_GameState.h"
#include "CoreMinimal.h"
#include "Framework/PF_PlayerState.h"
#include "Net/UnrealNetwork.h"

class APF_PlayerState;



//UFUNCTION(BlueprintPure, Category = "Heist|State")
//APF_PlayerState* GetWinner() const { return Ganador; }


APF_GameState::APF_GameState()
{
	bReplicates = true;
}

void APF_GameState::StartMatch(float Duracion)
{
	if (!HasAuthority()) return;
	TiempoRestante = Duracion;
	OnRep_TiempoRestante();
	
	GetWorldTimerManager().SetTimer(RelojPartidaTimerHandle,this, &APF_GameState::ActualizarReloj, 1.0f,true);
	
}

void APF_GameState::SetGameOver(ETeam EquipoGanador)
{
	if (!HasAuthority()) return;
	GanadorPartida = EquipoGanador;
	OnRep_GanadorPartida();
}

void APF_GameState::SetTiempoInicial(float TiempoInicial)
{
	if (HasAuthority())
	{
		TiempoRestante = TiempoInicial;
		OnRep_TiempoRestante();
	}
}

void APF_GameState::OnRep_TiempoRestante()
{
	OnTiempoRestanteChanged.Broadcast(TiempoRestante);
}

void APF_GameState::ActualizarReloj()
{
	if (TiempoRestante > 0)
	{
		TiempoRestante --;
		OnTiempoRestanteChanged.Broadcast(TiempoRestante);
	}
}

void APF_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APF_GameState, PuntajeEquipoAzul);
	DOREPLIFETIME(APF_GameState, PuntajeEquipoRojo);
	DOREPLIFETIME(APF_GameState, TiempoRestante);
	DOREPLIFETIME(APF_GameState, GanadorPartida);
}

void APF_GameState::RecalcularPuntajeEquipo(ETeam Equipo)
{
	if (!HasAuthority()) return;
	
	int32 Total = 0;
	
	for (APlayerState* Estado : PlayerArray)
	{
		if (APF_PlayerState* PS = Cast<APF_PlayerState>(Estado))
		{
			if (PS->GetTeamColor() ==Equipo)
			{
				Total+=PS->GetValorTotalLoot();
			}
		}
	}
	if (Equipo == ETeam::Blue)
	{
		PuntajeEquipoAzul = Total;
		OnPuntajeEquipoChanged.Broadcast(ETeam::Blue, PuntajeEquipoAzul);
	}
	else if (Equipo == ETeam::Red)
	{
		PuntajeEquipoRojo = Total;
		OnPuntajeEquipoChanged.Broadcast(ETeam::Red, PuntajeEquipoRojo);
	}
}

int32 APF_GameState::GetPuntajeEquipo(ETeam Equipo) const
{
	if (Equipo == ETeam::Blue) return PuntajeEquipoAzul;
	if (Equipo == ETeam::Red)  return PuntajeEquipoRojo;
	return 0;
}

void APF_GameState::OnRep_PuntajeAzul()
{
	OnPuntajeEquipoChanged.Broadcast(ETeam::Blue, PuntajeEquipoAzul);
}

void APF_GameState::OnRep_PuntajeRojo()
{
	OnPuntajeEquipoChanged.Broadcast(ETeam::Red, PuntajeEquipoRojo);
}

void APF_GameState::OnRep_GanadorPartida()
{
	OnGanadorName.Broadcast(GanadorPartida);
	
	
}
