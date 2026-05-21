// Fill out your copyright notice in the Description page of Project Settings.
#include "Framework/PF_GameState.h"
#include "CoreMinimal.h"
#include "Framework/PF_GameMode.h"
#include "Framework/PF_PlayerState.h"

class APF_PlayerState;

void StartMatch(float Duracion);

void SetGameOver(APF_PlayerState* Ganador);

//UFUNCTION(BlueprintPure, Category = "Heist|State")
//APF_PlayerState* GetWinner() const { return Ganador; }




UPROPERTY(ReplicatedUsing = OnRep_RemainingTime)
float TiempoRestante = 0.f;