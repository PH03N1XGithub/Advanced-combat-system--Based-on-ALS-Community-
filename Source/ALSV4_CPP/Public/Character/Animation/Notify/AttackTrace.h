// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Library/ALSCharacterEnumLibrary.h"
#include "AttackTrace.generated.h"
/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UAttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType AttackType;

	


	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
