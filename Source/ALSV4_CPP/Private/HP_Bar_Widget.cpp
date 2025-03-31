// Fill out your copyright notice in the Description page of Project Settings.


#include "HP_Bar_Widget.h"

void UHP_Bar_Widget::SetHP_Implementation(int hp)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "HP Bar Widget");
}

float UHP_Bar_Widget::GetHP_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "HP Bar Widget get");
	return 0;
}
