// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utils/TeamType.h"
#include "CharacterHud.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;
class APF_GameState;
class APFinalProgramacionPlayerController;
/**
 * 
 */
UCLASS()
class PFINALPROGRAMACION_API UCharacterHud : public UUserWidget
{
	GENERATED_BODY()
protected:	
	virtual void NativeOnInitialized() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetScoreAzulText(int32 ScoreAzul);
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetScoreRojoText(int32 ScoreRojo);
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetTiempo(int32 TiempoPartida);
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetEquipoGanador(FString EquipoGanador);
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void MostrarPromptItem(bool bMostrar, FString NombreItem = "");
	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_PickupItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_PickupName;
	
	UPROPERTY()
	TObjectPtr<ACharacter> CharacterOwning;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TiempoText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GanadorText;
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ScoreAzul;
 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ScoreRojo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Dominio;
;
	
	
	UFUNCTION()
	void OnPuntajeEquipoChanged(ETeam Equipo, int32 NuevoPuntaje);
	
	void ActualizarBarraDominio(int32 PuntajeAzul, int32 PuntajeRojo);
	
};
