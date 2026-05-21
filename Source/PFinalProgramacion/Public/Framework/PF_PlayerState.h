// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Utils/TeamType.h"
#include "PF_PlayerState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLootValueChanged, const int32, NuevoTotalLoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemsCollectedChanged, const int32, NuevoItemsRecolectados);

UCLASS()
class PFINALPROGRAMACION_API APF_PlayerState : public APlayerState
{
	GENERATED_BODY()
protected:	

	UPROPERTY(ReplicatedUsing = OnRep_ItemsReclectados, BlueprintReadWrite, Category = "Puntaje")
	int32 ItemsReclectados = 0;
	
	UPROPERTY(ReplicatedUsing = OnRep_ValorTotalLoot, BlueprintReadOnly, Category = "Heist|Score")
	int32 ValorTotalLoot = 0;
	
	UFUNCTION()
	void OnRep_ValorTotalLoot();
 
	UFUNCTION()
	void OnRep_ItemsReclectados();
	
public:
	
	void SetTeamColor(ETeam Team);
	
	ETeam GetTeamColor() const;
	
	UPROPERTY(ReplicatedUsing=OnRep_TeamColor)
	ETeam TeamColor = ETeam::None;
	
	UFUNCTION()
	void OnRep_TeamColor();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category = "Puntaje")
	void AddLoot(int32 SumaLoot);
	
	//getters
	UFUNCTION(BlueprintPure, Category = "Puntaje")
	int32 GetItemsRecolectados() const { return ItemsReclectados; }
	
	UFUNCTION(BlueprintPure, Category = "Puntaje")
	int32 GetValorTotalLoot() const { return ValorTotalLoot; }
	
	//delegates para ui
	UPROPERTY(BlueprintAssignable, Category = "Puntaje")
	FOnLootValueChanged OnLootValueChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Puntaje")
	FOnItemsCollectedChanged OnItemsCollectedChanged;
	
};
