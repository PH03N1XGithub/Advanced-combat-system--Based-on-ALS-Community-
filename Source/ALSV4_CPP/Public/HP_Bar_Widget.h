// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HP_Bar_Widget.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UHP_Bar_Widget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHP(int hp);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	float GetHP();



	void SetHP_Implementation(int hp);
	float GetHP_Implementation();
	
};
