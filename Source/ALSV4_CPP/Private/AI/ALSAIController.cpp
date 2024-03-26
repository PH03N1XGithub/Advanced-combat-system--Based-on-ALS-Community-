// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#include "AI/ALSAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ALSBaseCharacter.h"

AALSAIController::AALSAIController()
{
}

void AALSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CharacterOwner = Cast<AALSBaseCharacter>(InPawn);
	AITypeR = CharacterOwner->AIType;
	switch (AITypeR)
	{
	case EAIType::ALS:
		if (Behaviour && InPawn)
		{
			RunBehaviorTree(Behaviour);
		}
		break;
	case EAIType::Custom:


		if (Behaviour2 && InPawn)
		{
			RunBehaviorTree(Behaviour2);
		}
		break;
	default:
		break;
	}
	
	
	
}

FVector AALSAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	if (Actor == nullptr)
	{
		return FAISystem::InvalidLocation;
	}
	const APawn* FocusPawn = Cast<APawn>(Actor);
	if (FocusPawn)
	{
		// Focus on pawn's eye view point
		return FocusPawn->GetPawnViewLocation();
	}
	return Actor->GetActorLocation();
}
