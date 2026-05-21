// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InterfazInteraccion.h"
#include "ItemRobable.generated.h"
class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class PFINALPROGRAMACION_API AItemRobable : public AActor, public IInterfazInteraccion
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemRobable();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//overlap para ver si el character lo tengo en rango
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// llamado server rpc del character
	void EjecutarPickup(AActor* Interactor);
	
	// personaje cuando presiona E llamo a la interfaz
	virtual void Interact_Implementation(AActor* Interactor) override;
	//Interfaz necesita skeletalmesh
	virtual USkeletalMeshComponent* GetSkeletalMesh_Implementation() override { return nullptr; }

	//Valor que va a sumar el jugador al puntaje
	UFUNCTION(BlueprintPure, Category = "Item")
	int32 GetValor() const { return ValorItem; }
		
	UFUNCTION(BlueprintPure, Category = "Item")
	bool EstaEnRango() const { return bPersonajeEnRango; }
	
	UFUNCTION(BlueprintPure, Category = "Item")
	bool EstaRecogido() const { return bRecogido; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Item", meta=( AllowPrivateAccess = "true" ))
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Item", meta=( AllowPrivateAccess = "true" ))
	USphereComponent* SphereCollision;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Item", meta=( AllowPrivateAccess = "true" ))
	FName SocketName = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", Meta = (AllowPrivateAccess = "true"))
	int32 ValorItem = 100;
	
	void DesactivarColision(); 
	
	UPROPERTY(ReplicatedUsing = OnRep_Recogido)
	bool bRecogido = false;
	
	bool bPersonajeEnRango = false;
	
	UFUNCTION()
	void OnRep_Recogido();
	
};
