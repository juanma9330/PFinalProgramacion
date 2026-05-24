// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/TeamType.h"
#include "ZonaAnotacion.generated.h"

class UBoxComponent;
class APF_Character;
class APF_PlayerState;

UCLASS()
class PFINALPROGRAMACION_API AZonaAnotacion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZonaAnotacion();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void CheckPuntuar();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zona")
	bool bZonaExclusiva = false;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zona")
	ETeam EquipoAsignado = ETeam::None;
	
	
	
//	UPROPERTY(ReplicatedUsing=OnRep_OwnerTeam)
//	ETeam OwnerTeam = ETeam::None;
	
//	UFUNCTION()
//	void OnRep_OwnerTeam();
	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,blueprintReadWrite,Category="Zona", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent>BoxComponent;
	
	UPROPERTY(Replicated)
	TArray<APF_Character*> JugadoresEnZona;
	
	UFUNCTION(Server, Reliable)
	void Server_Puntuar(APF_Character* Character);
	
	bool PuedeEntregarLoot (APF_Character* Character) const;
};
