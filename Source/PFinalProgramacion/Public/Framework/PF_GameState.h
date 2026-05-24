// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Utils/TeamType.h"
#include "PF_GameState.generated.h"

class APF_PlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPuntajeEquipoChanged,ETeam,Equipo,int32,NuevoPuntaje);
/**
 * 
 */
UCLASS()
class PFINALPROGRAMACION_API APF_GameState : public AGameState
{
	GENERATED_BODY()
	
	
public:
	APF_GameState();
	
	void StartMatch(float Duracion);

	void SetGameOver(APF_PlayerState* Ganador);
	
private:	
	
	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime)
	float TiempoRestante = 0.f;
	
	UFUNCTION()
	void OnRep_RemainingTime();
	
	
public:
	
	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable,Category=Puntaje)
	void RecalcularPuntajeEquipo(ETeam Equipo);
	
	UFUNCTION(BlueprintCallable,Category=Puntaje)
	int32 GetPuntajeEquipo(ETeam Equipo) const;
	
	UPROPERTY(BlueprintAssignable,Category=Puntaje)
	FOnPuntajeEquipoChanged OnPuntajeEquipoChanged;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_PuntajeAzul)
	int32 PuntajeEquipoAzul = 0;
	
	UPROPERTY(ReplicatedUsing = OnRep_PuntajeRojo)
	int32 PuntajeEquipoRojo = 0;
	
	UFUNCTION()
	void OnRep_PuntajeAzul();
	
	UFUNCTION()
	void OnRep_PuntajeRojo();
};
