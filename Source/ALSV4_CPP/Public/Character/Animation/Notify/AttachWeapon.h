// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttachWeapon.generated.h"

UENUM()
enum class EALSWaeponChoiseState : uint8
{
	Sword,
	Leviathan,
};

enum class EALSWaeponChoiseState : uint8;
/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UAttachWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bAdd;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EALSWaeponChoiseState WeaponState;
	
	
};
