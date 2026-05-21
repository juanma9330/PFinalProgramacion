// Copyright Epic Games, Inc. All Rights Reserved.

#include "PFinalProgramacionCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PFinalProgramacion.h"
#include "Framework/PF_PlayerState.h"

APFinalProgramacionCharacter::APFinalProgramacionCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}
//interfaz funcion MIGRAR
USkeletalMeshComponent* APFinalProgramacionCharacter::GetSkeletalMesh_Implementation()
{
	return GetMesh();
}

void APFinalProgramacionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APFinalProgramacionCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APFinalProgramacionCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APFinalProgramacionCharacter::Look);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APFinalProgramacionCharacter::DoInteract);
	}
	else
	{
		UE_LOG(LogPFinalProgramacion, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APFinalProgramacionCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void APFinalProgramacionCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void APFinalProgramacionCharacter::DoMove(float Right, float Forward)
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

void APFinalProgramacionCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APFinalProgramacionCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void APFinalProgramacionCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void APFinalProgramacionCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	APF_PlayerState* PSNuevo = Cast<APF_PlayerState>(NewController->PlayerState);
	if (PSNuevo)
	{
		ETeam team = PSNuevo->GetTeamColor();
		UpdateTeamMateraial(team);
		
	}
	
}

void APFinalProgramacionCharacter::DoInteract()
{
	FVector Start = GetActorLocation();

	FVector End =
		Start + GetActorForwardVector() * 200.f;

	FHitResult Hit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility);

	if (bHit && Hit.GetActor())
	{
		AActor* HitActor = Hit.GetActor();

		if (HitActor->Implements<UInterfazInteraccion>())
		{
			IInterfazInteraccion::Execute_Interact(HitActor, this);
		}
	}
}

void APFinalProgramacionCharacter::UpdateTeamMateraial(ETeam Team)
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

void APFinalProgramacionCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	APF_PlayerState* PSNuevo = Cast<APF_PlayerState>(GetPlayerState());
	if (PSNuevo)
	{
		ETeam team = PSNuevo->GetTeamColor();
		UpdateTeamMateraial(team);
	}
	
}

