// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Library/ALSCharacterEnumLibrary.h"
#include "Library/ALSCharacterStructLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HP_Bar_Widget.h"
#include "CPC_Combat.generated.h"

class ACharacter;
class USkeletalMeshComponent;
class UAnimInstance;
class UCurveFloat;
class AALSBaseCharacter;
class UALSPlayerCameraBehavior;

UCLASS(Blueprintable, BlueprintType)//( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UCPC_Combat : public UActorComponent
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this component's properties
	UCPC_Combat();

	//Standard Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking = false;
	bool bPickA = true;
	bool bActionAlreadyPerformed = false;
	bool bTraceOn;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ParryTimer;//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRightAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsEnemy;
	float YawValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CombatMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBlock;	//block
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsDodge;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ForwardDir;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RightDir;

	//Enums
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActiveState ActiveState;

	EALSOverlayState OverlayState;
	EAttackType E_AttackType;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*> FistMontageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*> SwordMontageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* FocusedEnemy;
	TArray<AActor*> EnemyArray;
	TArray<AActor*> EnemyToHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<UAnimMontage*, FStrucHitReactions> Montages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Sword_Equip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Sword_Holster;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* CharacterBlock;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* ParryKick;

	
	UPROPERTY(EditAnywhere, Category = "Combat")
	UCurveFloat* CurveFloat;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values")
	EALSStance Stance = EALSStance::Standing;

	
	float InstepSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanTurn;

	EAttackMove StoredKey;

	FVector DodgeVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DodgeAngle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FVector, UAnimMontage*> Dodge_Montages;
	

private:

	UPROPERTY()
	FTimeline YourTimeline;


	FTimerHandle DelayedFunctionTimerHandle;
	FTimerHandle DelayedFunctionTimerHandleIG;

public:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* CharacterOwner = nullptr;  // Initialize to nullptr
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterMovementComponent* CharacterMovement = nullptr;  // Initialize to nullptr
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MeshComponent = nullptr;  // Initialize to nullptr
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimInstance* AnimInstance = nullptr;  // Initialize to nullptr
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CharacterCapsule = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* WidgetC = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* HP_Bar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "widget")
	UWidget* HP_Bar_Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Sword = nullptr;
	 UPROPERTY(BlueprintReadWrite)
	AALSBaseCharacter* AlsCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHP_Bar_Widget* HPWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UHP_Bar_Widget> HPBarWidgetClass;

	// Declare a variable to hold the created HP Bar Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* HPBarWidget;

	APlayerController* PlayerController;

	UUserWidget* hpbarR;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> wc;
	UPROPERTY(VisibleInstanceOnly)
	class UHP_Bar_Widget* tw;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

	UFUNCTION(BlueprintCallable)
	void ActionInput(EAttackMove AttackMove, EALSOverlayState Overlay);
	void ConsumedKey(EAttackMove AttackMove);
	UFUNCTION(BlueprintCallable)
	void EnterCombatMode();
	void CheckEndCombat();

	void FindEnemy();
	void CheckAutoFind();
	UFUNCTION(BlueprintCallable)
	void SelectFocusEnemy(AActor* Enemy);

	void LightAttack();

	void IgnoreRootMotion();
	void EnableRootMotion() const;
	void ResetState() const;
	UFUNCTION(BlueprintCallable)
	bool CanAttack() const;
	UFUNCTION(BlueprintCallable)
	void Initialise(UWidgetComponent* WidgetBar, UStaticMeshComponent* Sword_Stm, UUserWidget* HpBar);

	UFUNCTION(BlueprintCallable)
	void ShowBar(bool Show);


	UFUNCTION(BlueprintCallable)
	void TraceOn(bool RightAttack, bool Trace, EAttackType AttackType);

	bool SelectAttackTrace(TArray<FHitResult>& OutHits);

	UFUNCTION(BlueprintCallable)
	void PerformTrace();

	void GetHit(const ACharacter* Attacker, FStrucHitReactions HitReaction) const;
	

	void AttachWeapon(bool bAdd) const;

	void EquipWeapon(bool bEquip) const;


	UFUNCTION(BlueprintCallable)
	void RotateToEnemy(AActor* Enemy) const;

	UFUNCTION(BlueprintCallable)
	void RotateToEnemyTimelineUpdate(float Value) const;
	UFUNCTION()
	void OnTimelineEnd();


	void InitialiseRotateToEnemyTimeline();
	void TargetEnemy(float DeltaTime) const;

	void TurnToYaw(float TargetYaw, float DeltaTime) const;
	UFUNCTION(BlueprintCallable)
	void QueueInput();

	void WidgetPosition(UUserWidget* Widget) const;
	void Dodge();
	void CalculateDirection();
	void CalculateDodgeDirection();

		
};


