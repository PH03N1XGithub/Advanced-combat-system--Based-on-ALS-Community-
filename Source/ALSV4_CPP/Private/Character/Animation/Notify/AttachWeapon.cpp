// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/AttachWeapon.h"
#include "CPC_Combat.h"

void UAttachWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (const UCPC_Combat* CombatComp = MeshComp->GetOwner()->GetComponentByClass<UCPC_Combat>())
	{
		CombatComp->AttachWeapon(bAdd);
	}


}
