#pragma once
#include "CoreMinimal.h"
UENUM(Blueprintable)
enum class EAxeState : uint8
{
	Idle =0,
	Launched,
	LodgedInSomething,
	Returning
	
};
