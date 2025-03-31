

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "EAxeState.h"
#include "LeviathanAxe.generated.h"

UCLASS()
class ALSV4_CPP_API ALeviathanAxe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeviathanAxe();

	

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> LodgePoint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> PivotPoint;

	/*/** Please add a variable description #1#
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;*/

		/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> Trace2;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> Trace1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<URotatingMovementComponent> RotatingMovement;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UParticleSystemComponent> P_AxeCatch;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UParticleSystemComponent> SwingParticles;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UParticleSystemComponent> ThrowParticles;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UTimelineComponent> AxeTrace_Return;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UTimelineComponent> AxeLodgePullTimeline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeLodgePullCurve;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UTimelineComponent> SpinForTheWIn;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> SpinForTheWInCurve;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UTimelineComponent> AxeThrowTraceTimeline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeThrowCurve;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UTimelineComponent> AxeLodgeWiggleTimeline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeLodgeWiggleCurve;

	

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UTimelineComponent> AxeRot;

	

	/** Axe Return Curve Timeline */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UTimelineComponent> AxeReturnCurveTimeline;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeReturnFlatCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeReturnRotationCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> RightVectorCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeReturnSpeedCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeReturnRotationCurve2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile_Leviathon")
	TObjectPtr<UCurveFloat> AxeReturnSoundCurve;



	
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double DistanceFromChar;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Ref")
	TObjectPtr<ACharacter> Char_Ref;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector InitialLoc;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FRotator InitialRot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector TargetLocation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	EAxeState AxeState;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector ImpactLocation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double AxeSpinRate = 2.5f;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector ThrowDirection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector CameraLocationAtThrow;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FRotator CameraStartRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double OptimalDistance = 1400;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FRotator LodgePointBaseRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double ImpulseStrength = 20000;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FVector AxeLocationLastTick;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double AxeSpinAxis_Offset = 60;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double AxeThrowSpeed = 3500;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double AxeReturnSpeed = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FVector ImpactNormal;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FName HitBoneName;
	

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FVector ReturnTargetLocation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	int NumberOfReturnSpins;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double MaxCalculationDistance = 3000;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double AxeReturn_Tilt = 60;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TObjectPtr<UAudioComponent> ReturnWhoosh;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double ReturnSpinRate = 0.35f;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double Z_Adjustment;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool bHitEnemy;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double AxeThrowTraceDistance = 120;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double AxeReturnRightVectorScale = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Ref", meta=(MultiLine="true"))
	TObjectPtr<ACharacter> Hit_AI_Ref;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool bIsHit;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<AActor> Hit_Actor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TObjectPtr<USoundBase> BodyImpactSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	UAnimMontage* HitMontage;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Default")
	UParticleSystem* HitParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	USoundBase* RecallSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	USoundBase* Return_Sound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	bool bIsRotating = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Throw(FRotator CameraRotator, FVector ThrowDirectionVector, FVector CameraLocation, float ThrowSpeed);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SnapAxeToStartPosition(FRotator StartRotation, FVector ThrowDirectionVector, FVector CameraLocation);
	
	void StartAxeRotForward() const;
	void InitializeHitValues(const FHitResult& Hit);
	void ApplyHitToEnemyAndAttach();
	void ImpactParticles() const;

	
	UFUNCTION()
	void AxeLodgePull(float Value) const;
	
	UFUNCTION()
	void AxeLodgeWiggle(float Value) const;

	
	void InitialiseReturnVariables();

	UFUNCTION()
	void AxeReturnPosition();
	
	void AxeReturnSpin(float ReturnTimelinePlayRate);

	UFUNCTION()
	static void AxeReturnSpinTimeline();


	UFUNCTION(BlueprintCallable)
	void Recall();
	
	UFUNCTION()
	void ThrowTrace(float Gravity);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void StopAxeMoving() const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	FVector AdjustAxeImpactLocation(FVector InImpactNormal, FVector InImpactLocation);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static float AdjustAxeImpactPitch(const FVector& InImpactNormal, double InclinedSurfaceRange, double RegularSurfaceRange);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	double GetClampedAxeDistanceFromChar(double MaxDistance) const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void AdjustAxeReturnLocation();

	UFUNCTION()
	void AxeReturnTrace();


	void CatchAxe();

	UFUNCTION(BlueprintCallable ,BlueprintImplementableEvent)
	void CharacterCatchAxe();

	UFUNCTION(BlueprintCallable ,BlueprintImplementableEvent)
	void SetAlsState();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	double CalculateReturnTimelineSpeed(double InOptimalDistance, double InAxeReturnSpeed) const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void LodgeAxe();
	void SetupTimeline();
};
