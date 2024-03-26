// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"	 
#include "SetAITask_SetState.generated.h"

/**
 * 
 */
UCLASS(Category = ALS, meta = (DisplayName = "changeTaskNode"))
class ALSV4_CPP_API USetAITask_SetState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USetAITask_SetState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
