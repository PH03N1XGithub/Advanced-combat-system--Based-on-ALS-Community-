// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "CPP_BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS(Category = CombatAI, meta = (DisplayName = "CPP_BTT_Attack"))
class ALSV4_CPP_API UCPP_BTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UCPP_BTTask_Attack();

protected:

	void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);
	
};
