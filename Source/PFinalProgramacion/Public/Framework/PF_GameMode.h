// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PF_GameMode.generated.h"

class AZonaAnotacion;
/**
 * 
 */
UCLASS()
class PFINALPROGRAMACION_API APF_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:	
	APF_GameMode();
	virtual void StartMatch() override;
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
		
	void CheckWinCondition();
	void DeclareWinner();
	

	//----------TIEMPOS----------------
	//tiempo de espra para iniciar partida
	FTimerHandle TiempoEsperaInicioTimerHandle;
	
	UPROPERTY(EditDefaultsOnly,Category= "GameMode")
	float MatchStartTimeDuration;
	
	//todavia no sé cuanto tiempo de partida le doy
	FTimerHandle DuracionPartidaHandle;

	UPROPERTY(EditDefaultsOnly,Category= "GameMode")
	float DuracionPartida;
	
		// timer DuracionPartidaHandle de la partida llega a cero
//UFUNCTION()
//	void TerminotiempoPartida();
	

	
//	void ZonaPuntuacion();
	
	//UPROPERTY(BlueprintReadOnly,Category= "GameMode")
//	TArray<TObjectPtr<AZonaAnotacion>> ZonaParaPuntuar;

	
	

	
	
	
	
};
