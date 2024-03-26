// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"



UBTTask_SetState::UBTTask_SetState() {
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}
