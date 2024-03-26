// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/AttackTrace.h"
#include "Library/ALSCharacterEnumLibrary.h"
#include "CPC_Combat.h"






void UAttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	UCPC_Combat* CPCREF = MeshComp->GetOwner()->GetComponentByClass<UCPC_Combat>();

	if (!CPCREF)
		return;

	

	

	CPCREF->TraceOn(bRightHand, true, AttackType);
}

void UAttackTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	if (!MeshComp || !MeshComp->GetOwner())
		return;
	UCPC_Combat* CPCREF = MeshComp->GetOwner()->GetComponentByClass<UCPC_Combat>();

	if (!CPCREF)
		return;

	CPCREF->PerformTrace();
}

void UAttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (!MeshComp || !MeshComp->GetOwner())
		return;
	UCPC_Combat* CPCREF = MeshComp->GetOwner()->GetComponentByClass<UCPC_Combat>();

	if (!CPCREF)
		return;

	

	CPCREF->TraceOn(bRightHand, false, AttackType);
	
}
