// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actores/ZonaAnotacion.h"

#include "Components/BoxComponent.h"


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

// Called every frame
void AZonaAnotacion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZonaAnotacion::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void AZonaAnotacion::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
}

