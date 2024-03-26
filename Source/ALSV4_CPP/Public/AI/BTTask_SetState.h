// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetState.generated.h"

/**
 * 
 */
UCLASS(Category = ALS, meta = (DisplayName = "AttacTaskNode"))
class ALSV4_CPP_API UBTTask_SetState : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_SetState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
