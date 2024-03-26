// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/IsAttackingState.h"
#include "CPC_Combat.h"


void UIsAttackingState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UCPC_Combat* CombatComponent = MeshComp->GetOwner()->GetComponentByClass<UCPC_Combat>();
	if (!CombatComponent)
		return;
	CombatComponent->IsAttacking = true;
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("Is Attacking Notify Begin"));

}

void UIsAttackingState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void UIsAttackingState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UCPC_Combat* CombatComponent = MeshComp->GetOwner()->GetComponentByClass<UCPC_Combat>();
	if (!CombatComponent)
		return;
	CombatComponent->IsAttacking = false;
	CombatComponent->ActiveState = EActiveState::Idle;
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("Is Attacking Notify End"));
}
