// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actores/ZonaAnotacion.h"
#include "Framework/PF_PlayerState.h"
#include "Character/PF_Character.h"
#include "Components/BoxComponent.h"
#include "Items/ItemRobable.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AZonaAnotacion::AZonaAnotacion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("AreaPuntuacion");
	BoxComponent->SetBoxExtent(FVector(300, 300, 300));
	BoxComponent->SetHiddenInGame(false);
	
	
}

// Called when the game starts or when spawned
void AZonaAnotacion::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->ShapeColor = FColor::White;
	BoxComponent->MarkRenderStateDirty();
}

bool AZonaAnotacion::PuedeEntregarLoot(APF_Character* Character) const
{
	if (!bZonaExclusiva) return true;
	
	if (const APF_PlayerState* PS = Character->GetPlayerState<APF_PlayerState>())
	{
		return PS->GetTeamColor() == EquipoAsignado;
	}
	return false;
}


// Called every frame
void AZonaAnotacion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZonaAnotacion::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!OtherActor) return;
	if (!HasAuthority()) return;
	
	APF_Character* Character = Cast<APF_Character>(OtherActor);
	if (!Character) return;
	
	JugadoresEnZona.AddUnique(Character);
	
	Server_Puntuar(Character);
	
}

void AZonaAnotacion::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!OtherActor) return;
	if (!HasAuthority()) return;
	
	APF_Character* Character = Cast<APF_Character>(OtherActor);
	if (!Character) return;
	
	JugadoresEnZona.Remove(Character);
	
}

void AZonaAnotacion::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZonaAnotacion, JugadoresEnZona);
}



void AZonaAnotacion::Server_Puntuar_Implementation(APF_Character* Character)
{
	if (!Character) return;
	if (!PuedeEntregarLoot(Character)) return;
	
	APF_PlayerState* PS=Character->GetPlayerState<APF_PlayerState>();
	if (!PS) return;
	
	TArray<AActor*> AttachedActors;
	Character->GetAttachedActors(AttachedActors);
	
	int32 LootEntregado = 0;
	
	for (AActor* Actor : AttachedActors)
	{
		if (AItemRobable* Item =Cast<AItemRobable>(Actor))
		{
			LootEntregado+=Item->GetValor();
			Item ->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Item ->Destroy();
		
			PS->AddLoot(LootEntregado);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,FString::Printf(TEXT("%s entrego: %d | Total: %d"),
				*PS->GetPlayerName(),LootEntregado,PS->GetValorTotalLoot()));
		}
	}
}