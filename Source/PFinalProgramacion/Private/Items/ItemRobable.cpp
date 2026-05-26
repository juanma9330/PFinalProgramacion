// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemRobable.h"
#include "Components/SphereComponent.h"
#include "Interface/InterfazInteraccion.h"
#include "GameFramework/Character.h"
#include "Framework/PF_PlayerState.h"
#include "Character/PF_Character.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AItemRobable::AItemRobable()
{
	SetReplicates(true);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	SetRootComponent(ItemMesh);
	ItemMesh->SetIsReplicated(true);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetSphereRadius(350.0f);
	SphereCollision->SetupAttachment(ItemMesh);
	
}

// Called when the game starts or when spawned
void AItemRobable::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AItemRobable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AItemRobable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!OtherActor) return;
	if (bRecogido) return;
	if (OtherActor->Implements<UInterfazInteraccion>())
	{
		bPersonajeEnRango = true;
	}
}

void AItemRobable::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!OtherActor) return;
	if (OtherActor->Implements<UInterfazInteraccion>())
	{
		bPersonajeEnRango = false;
	}
}

void AItemRobable::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;
	if (bRecogido) return;
	
	DesactivarColision();
	
	if (APF_Character* Character = Cast<APF_Character>(Interactor))
	{
		Character->Server_PickupItem(this);
	}
}


void AItemRobable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItemRobable, bRecogido);
}


void AItemRobable::OnRep_Recogido()
{
	if (bRecogido)
	{
		DesactivarColision();
	}
}

void AItemRobable::EjecutarPickup(AActor* Interactor)
{
	if (!Interactor) return;
	if (bRecogido) return;
		
	bRecogido = true;
	
	USkeletalMeshComponent* SkeletalMeshComponent = IInterfazInteraccion::Execute_GetSkeletalMesh(Interactor);
	AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,SocketName);
	
	DesactivarColision();
	
	SetOwner(Interactor);
	if (ACharacter* Character = Cast<ACharacter>(Interactor))
	{
		if (APF_PlayerState* PS = Character->GetPlayerState<APF_PlayerState>())
			{
			//PS->AddLoot(ValorItem);
			}
	}
}

void AItemRobable::DesactivarColision()
{
	if (SphereCollision)
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereCollision->SetGenerateOverlapEvents(false);
	}
 
	if (ItemMesh)
	{
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

