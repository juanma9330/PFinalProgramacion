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
}

void APF_GameState::SetGameOver(APF_PlayerState* Ganador)
{
}

void APF_GameState::OnRep_RemainingTime()
{
}

void APF_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APF_GameState, PuntajeEquipoAzul);
	DOREPLIFETIME(APF_GameState, PuntajeEquipoRojo);

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
