// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Framework/PF_PlayerState.h"
#include "Interface/InterfazInteraccion.h"
#include "PF_Character.generated.h"

class AItemRobable;
class UInputMappingContext;
class TeamType;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class PFINALPROGRAMACION_API APF_Character : public ACharacter, public IInterfazInteraccion
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputMappingContext* IMC_PlayerController;
	
	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* Movimiento;
	
	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* Salto;
	
	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* Interactuar;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Mirar;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
public:
	// Sets default values for this character's properties
	APF_Character();
	
	//INTERFAZ
	virtual USkeletalMeshComponent* GetSkeletalMesh_Implementation() override;
	
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	//---------------------------------------------------------------------------------------------------------------
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoInteract();
	
	
	UFUNCTION(Server, Reliable)
	void Server_PickupItem(AItemRobable* Item);
	
	void UpdateTeamMateraial(ETeam Team);
	void OnRep_PlayerState() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
