// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Character/ALSBaseCharacter.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "ALSAIController.generated.h"
/**
 * 
 */
UCLASS()
class ALSV4_CPP_API AALSAIController : public AAIController
{
	GENERATED_BODY()

public:
	AALSAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> Behaviour = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> Behaviour2 = nullptr;

	AALSBaseCharacter* CharacterOwner;

	EAIType AITypeR;

	USplineComponent* LocalPath;

	int Targetpoint;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	//void Patrol();
	//void Patrol_C();

	virtual FVector GetFocalPointOnActor(const AActor *Actor) const override;
};
