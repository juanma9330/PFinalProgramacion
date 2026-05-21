// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfazInteraccion.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterfazInteraccion : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class PFINALPROGRAMACION_API IInterfazInteraccion
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
		
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, category = "Interfaz")
	USkeletalMeshComponent* GetSkeletalMesh();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaz")
	void Interact(AActor* Interactor);
};
	
