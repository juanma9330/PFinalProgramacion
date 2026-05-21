#pragma once
#include "CoreMinimal.h"
//#include "TeamType.h"


UENUM(BlueprintType)
enum class ETeam : uint8
{
	None UMETA(DisplayName="None"),
	Blue UMETA(DisplayName="Blue"),
	Red UMETA(DisplayName="Red"),
};
