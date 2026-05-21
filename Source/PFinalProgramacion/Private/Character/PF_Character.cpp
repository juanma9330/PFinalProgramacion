// Fill out your copyright notice in the Description page of Project Settings.
 
#include "Character/PF_Character.h"
#include "Items/ItemRobable.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/InterfazInteraccion.h"


// Sets default values
APF_Character::APF_Character()
{
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	//CAMARA
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

USkeletalMeshComponent* APF_Character::GetSkeletalMesh_Implementation()
{
	return GetMesh();
}

void APF_Character::Interact_Implementation(AActor* Interactor)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, TEXT("Player esta interactuando"));
}

void APF_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(Salto, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(Salto, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(Movimiento, ETriggerEvent::Triggered, this, &APF_Character::Move);
		EnhancedInputComponent->BindAction(Mirar, ETriggerEvent::Triggered, this, &APF_Character::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APF_Character::Look);

		EnhancedInputComponent->BindAction(Interactuar, ETriggerEvent::Started, this, &APF_Character::DoInteract);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APF_Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void APF_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void APF_Character::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}
void APF_Character::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}
void APF_Character::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void APF_Character::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void APF_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	APF_PlayerState* PSNuevo = Cast<APF_PlayerState>(NewController->PlayerState);
	if (PSNuevo)
	{
		ETeam team = PSNuevo->GetTeamColor();
		UpdateTeamMateraial(team);
		
	}
}

void APF_Character::DoInteract()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->Implements<UInterfazInteraccion>())
		{
			IInterfazInteraccion::Execute_Interact(Actor, this);
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, TEXT("Personaje Interactuando"));
			break;
		}
		
	}
}


// Called every frame
void APF_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APF_Character::UpdateTeamMateraial(ETeam Team)
{
	switch (Team) 
	{
	case ETeam::None:
		break;
	case ETeam::Blue:
		GetMesh()->SetColorParameterValueOnMaterials(FName("Paint Tint"),FLinearColor(FColor::Blue));
		break;
	case ETeam::Red:
		GetMesh()->SetColorParameterValueOnMaterials(FName("Paint Tint"),FLinearColor(FColor::Red));
		break;
	}
}

void APF_Character::Server_PickupItem_Implementation(AItemRobable* Item)
{
	if (!Item || Item->EstaRecogido()) return;
 
	const float RangoMaximo = 400.f;
	if (FVector::Dist(GetActorLocation(), Item->GetActorLocation()) > RangoMaximo) return;
 
	Item->EjecutarPickup(this);
}

void APF_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	APF_PlayerState* PSNuevo = Cast<APF_PlayerState>(GetPlayerState());
	if (PSNuevo)
	{
		ETeam team = PSNuevo->GetTeamColor();
		UpdateTeamMateraial(team);
	}
	
}
void APF_Character::BeginPlay()
{
	Super::BeginPlay();
}