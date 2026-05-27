// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Utils/TeamType.h"
#include "PF_GameState.generated.h"

class APF_PlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPuntajeEquipoChanged,ETeam,Equipo,int32,NuevoPuntaje);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTiempoRestanteChanged,int32,TiempoRestante);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, ETeam, EquipoGanador);
/**
 * 
 */
UCLASS()
class PFINALPROGRAMACION_API APF_GameState : public AGameState
{
	GENERATED_BODY()
	
	
public:
	APF_GameState();
	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartMatch(float Duracion);

	void SetGameOver(ETeam EquipoGanador);
	
	void SetTiempoInicial(float TiempoInicial);
	
	UFUNCTION(BlueprintCallable,Category=Puntaje)
	void RecalcularPuntajeEquipo(ETeam Equipo);
	
	UFUNCTION(BlueprintCallable,Category=Puntaje)
	int32 GetPuntajeEquipo(ETeam Equipo) const;
	
	UFUNCTION(BlueprintPure, Category = "Puntaje")
	int32 GetTiempoRestante() const { return TiempoRestante; }
	
	//Delegates para HUD
	UPROPERTY(BlueprintAssignable,Category=Puntaje)
	FOnPuntajeEquipoChanged OnPuntajeEquipoChanged;
	
	UPROPERTY(BlueprintAssignable,Category=Puntaje)
	FOnTiempoRestanteChanged OnTiempoRestanteChanged;
	
	UPROPERTY(BlueprintAssignable,Category=Puntaje)
	FOnGameOver OnGanadorName;
	
private:	
	
	UPROPERTY(ReplicatedUsing = OnRep_TiempoRestante)
	float TiempoRestante = 0.f;
	
	UFUNCTION()
	void OnRep_TiempoRestante();
	
	FTimerHandle RelojPartidaTimerHandle;
	void ActualizarReloj();
	
	UPROPERTY(ReplicatedUsing = OnRep_PuntajeAzul)
	int32 PuntajeEquipoAzul = 0;
	
	UPROPERTY(ReplicatedUsing = OnRep_PuntajeRojo)
	int32 PuntajeEquipoRojo = 0;
		
	UFUNCTION()
	void OnRep_PuntajeAzul();
	
	UFUNCTION()
	void OnRep_PuntajeRojo();
	
	UPROPERTY(ReplicatedUsing = OnRep_GanadorPartida)
	ETeam GanadorPartida = ETeam::None;
	
	UFUNCTION() 
	void OnRep_GanadorPartida();

	bool BPartidaActiva = false;
	
};
