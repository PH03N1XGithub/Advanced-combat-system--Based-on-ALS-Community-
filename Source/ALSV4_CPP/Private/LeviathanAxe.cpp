


#include "LeviathanAxe.h"

#include "CPC_Combat.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ALeviathanAxe::ALeviathanAxe(): DistanceFromChar(0), AxeState(EAxeState::Idle),
								AxeSpinRate(2.5f),
								OptimalDistance(1400),
								ImpulseStrength(20000),
                                AxeSpinAxis_Offset(60),
                                AxeThrowSpeed(3500),
                                AxeReturnSpeed(1),
                                NumberOfReturnSpins(0),
                                MaxCalculationDistance(3000),
                                AxeReturn_Tilt(60),
                                ReturnSpinRate(0.35f),
                                Z_Adjustment(0),
                                bHitEnemy(false),
                                AxeThrowTraceDistance(120),
                                SurfaceType(),
                                AxeReturnRightVectorScale(1),
                                bIsHit(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PivotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	RootComponent = PivotPoint;
	LodgePoint = CreateDefaultSubobject<USceneComponent>(TEXT("LodgePoint"));
	LodgePoint->SetupAttachment(PivotPoint);
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(LodgePoint);
	Trace1 = CreateDefaultSubobject<USceneComponent>(TEXT("Trace1"));
	Trace1->SetupAttachment(SkeletalMesh);
	Trace2 = CreateDefaultSubobject<USceneComponent>(TEXT("Trace2"));
	Trace2->SetupAttachment(SkeletalMesh);
	P_AxeCatch = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_AxeCatch"));
	P_AxeCatch->SetupAttachment(SkeletalMesh);
	SwingParticles =CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwingParticles"));
	SwingParticles->SetupAttachment(SkeletalMesh);
	ThrowParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrowParticles"));
	ThrowParticles->SetupAttachment(SkeletalMesh);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0, 0, 0);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->Deactivate();

	AxeThrowTraceTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AxeThrowTraceTimeline"));
	AxeLodgePullTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AxeLodgePullTimeline"));
	AxeLodgeWiggleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AxeLodgeWiggleTimeline"));
	AxeReturnCurveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AxeReturnCurveTimeline"));
	SpinForTheWIn = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpinForTheWIn"));
	AxeTrace_Return = CreateDefaultSubobject<UTimelineComponent>(TEXT("AxeTrace_Return")); 
		
}

// Called when the game starts or when spawned
void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
	SetupTimeline();
}

// Called every frame
void ALeviathanAxe::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALeviathanAxe::Throw(const FRotator CameraRotator, const FVector ThrowDirectionVector, const FVector CameraLocation,
                          const float ThrowSpeed)
{
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,TEXT("Throw"));
	CameraStartRotation = CameraRotator;
	ThrowDirection = ThrowDirectionVector;
	CameraLocationAtThrow = CameraLocation;
	AxeThrowSpeed = ThrowSpeed;
	//CameraStartRotation.Roll = AxeSpinAxis_Offset;
	SnapAxeToStartPosition(CameraStartRotation + FRotator(0,0,AxeSpinAxis_Offset),ThrowDirection,CameraLocationAtThrow);
	//Launch
	ProjectileMovement->Velocity = ThrowDirection* AxeThrowSpeed*2;
	ProjectileMovement->Activate();
	
	StartAxeRotForward();
	
	AxeState = EAxeState::Launched;
	ThrowParticles->BeginTrails("BaseSocket","TipSocket",ETrailWidthMode_FromCentre,1);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	//Trace
	AxeThrowTraceTimeline->PlayFromStart();
	SetAlsState();
	
	
}

void ALeviathanAxe::SnapAxeToStartPosition(const FRotator StartRotation, FVector ThrowDirectionVector, FVector CameraLocation)
{
	if (Char_Ref)
	{
		const FVector SocketLocation = Char_Ref->GetMesh()->GetSocketLocation("hand_rSocket");
		SetActorLocationAndRotation(SocketLocation,StartRotation);
	}
	
}

void ALeviathanAxe::StartAxeRotForward() const
{
	RotatingMovement->RotationRate = FRotator(-1500, 0, 0);
	RotatingMovement->Activate();
}

void ALeviathanAxe::ThrowTrace(const float Gravity)
{
	ProjectileMovement->ProjectileGravityScale = Gravity;

	//GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan,TEXT("Trace"));

	// Start and end points for the trace
	const FVector StartLocation = GetActorLocation();    // Starting point of the trace
	const FVector EndLocation = GetActorLocation() + (GetVelocity().GetSafeNormal() * AxeThrowTraceDistance);

	// Trace radius
	constexpr float TraceRadius = 10.0f;

	

	// Array to hold the hit results
	TArray<FHitResult> OutHits;

	// Set collision channel and trace parameters
	TArray<AActor*> ActorsToIgnore;  // Optionally, ignore certain actors
	ActorsToIgnore.Add(Char_Ref);
	constexpr bool bTraceComplex = true;
	constexpr EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	// Perform the multi-sphere trace

	if (UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),                    // World context
		StartLocation,                 // Start location
		EndLocation,                   // End location
		TraceRadius,                   // Radius of the sphere
		UEngineTypes::ConvertToTraceType(ECC_Camera), // Collision channel
		bTraceComplex,                 // Whether to perform complex collision
		ActorsToIgnore,                // Actors to ignore
		DrawDebugType,                 // Whether to draw debug trace (for testing)
		OutHits,                       // Output array of hits
		false,                          // Enable tracing against the entire world
		FLinearColor::Blue,           // Debug color (hit)
		FLinearColor::Red,             // Debug color (no hit)
		5.0f                           // Debug line duration
	))
	{

		for (const FHitResult& Hit : OutHits)
		{
			if (Hit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,TEXT("Hit"));
				InitializeHitValues(Hit);
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, HitBoneName.ToString());
				ThrowParticles->EndTrails();
				AxeThrowTraceTimeline->Stop();
				ProjectileMovement->Deactivate();
				RotatingMovement->Deactivate();
				
				if (Hit_AI_Ref)
				{
					GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,TEXT("enemy"));
					bHitEnemy = true;
					LodgeAxe();
					if (BodyImpactSound)
					{
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BodyImpactSound, ImpactLocation);
					}//Play Sound

					ApplyHitToEnemyAndAttach();
					ImpactParticles();
					
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,TEXT("wall"));
					bHitEnemy = false;
					LodgeAxe();
					if (ImpactSound)
					{
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ImpactSound, ImpactLocation);
					}
					ImpactParticles();
				}
			}
		}
	}
}

void ALeviathanAxe::InitializeHitValues(const FHitResult& Hit)
{
	ImpactLocation = Hit.ImpactPoint;
	ImpactNormal = Hit.ImpactNormal;
	HitBoneName = Hit.BoneName;
	Hit_Actor = Hit.GetActor();
	Hit_AI_Ref = Cast<ACharacter>(Hit_Actor);
	const UPhysicalMaterial* PhysMaterial = Hit.PhysMaterial.Get();
	SurfaceType = PhysMaterial->SurfaceType;
}

void ALeviathanAxe::LodgeAxe()
{
	StopAxeMoving();
	PivotPoint->SetRelativeRotation(FRotator::ZeroRotator);
	SetActorRotation(CameraStartRotation);
	FRotator Rotation;
	Rotation.Roll = FMath::RandRange(-8,70);
	const float RandomValue1 = FMath::RandRange(-30.0f, -55.0f);
	const float RandomValue2 = FMath::RandRange(-5.0f, -25.0f);
	Rotation.Pitch = AdjustAxeImpactPitch(ImpactNormal, RandomValue1, RandomValue2);
	Rotation.Yaw = 0;
	LodgePoint->SetRelativeRotation(Rotation);
	SetActorLocation(AdjustAxeImpactLocation(ImpactNormal,ImpactLocation));
	AxeState = EAxeState::LodgedInSomething;
}

void ALeviathanAxe::StopAxeMoving() const
{
	ProjectileMovement->Deactivate();
	RotatingMovement->Deactivate();
}

float ALeviathanAxe::AdjustAxeImpactPitch(const FVector& InImpactNormal, const double InclinedSurfaceRange,
                                          const double RegularSurfaceRange)
{
	const FRotator Rotation = UKismetMathLibrary::MakeRotationFromAxes
	(InImpactNormal,FVector::ZeroVector,FVector::ZeroVector);
	return Rotation.Pitch >= 0 ? (RegularSurfaceRange- Rotation.Pitch ):(InclinedSurfaceRange- Rotation.Pitch);
}

FVector ALeviathanAxe::AdjustAxeImpactLocation(FVector InImpactNormal, FVector InImpactLocation)
{
	const FRotator Rotator = UKismetMathLibrary::MakeRotationFromAxes(ImpactNormal,FVector::ZeroVector,FVector::ZeroVector);
		
	Z_Adjustment = ((Rotator.Pitch > 0 ?Rotator.Pitch:0)-90)/9;
	ImpactLocation.Z += Z_Adjustment;
	
	return ImpactLocation + (GetActorLocation()-LodgePoint->GetComponentLocation());
}

void ALeviathanAxe::ApplyHitToEnemyAndAttach()
//Add Physical anim 
{
	if (HitMontage)
	{
		Hit_AI_Ref->PlayAnimMontage(HitMontage);

		//Rotate To Character
		FVector CharacterLocation = Hit_Actor->GetActorLocation();
		FVector EnemyLocation = Char_Ref->GetActorLocation();
		FRotator FindLookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, EnemyLocation);
		const FRotator İnterpTo = FMath::RInterpTo(Hit_Actor->GetActorRotation(), FindLookAtRotation, 1, 2.5 + 4);
		Hit_Actor->SetActorRotation(İnterpTo);
	}

	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
	AttachToComponent(Hit_AI_Ref->GetMesh(), AttachRules, HitBoneName);
}

void ALeviathanAxe::ImpactParticles() const
{
	if (bHitEnemy)
	{
		UGameplayStatics::SpawnEmitterAttached(
			HitParticle,
			Hit_AI_Ref->GetMesh(),
			HitBoneName,
			LodgePoint->GetComponentLocation(),
			UKismetMathLibrary::MakeRotFromZ(ImpactNormal),
			FVector::OneVector * 2,
			EAttachLocation::KeepWorldPosition,
			true,
			EPSCPoolMethod::None,
			true
		);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			LodgePoint->GetComponentLocation(),
			UKismetMathLibrary::MakeRotFromZ(ImpactNormal),
			FVector::OneVector * 2,
			true,
			EPSCPoolMethod::None,
			true
		);
	}
}

void ALeviathanAxe::Recall()
{
	AxeThrowTraceTimeline->Stop();
	ReturnWhoosh = UGameplayStatics::SpawnSoundAttached(RecallSound, SkeletalMesh, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget, false,0,1,0,
		nullptr,nullptr,true);

	switch (AxeState) {
	case EAxeState::Idle:
		break;
	case EAxeState::Launched:
		Z_Adjustment = 0;
		AxeState = EAxeState::Returning;
		InitialiseReturnVariables();
		break;
	case EAxeState::LodgedInSomething:
		if (bHitEnemy)
		{
			LodgePointBaseRotation = LodgePoint->GetRelativeRotation();
			AxeLodgePullTimeline->SetPlayRate(3);
			AxeLodgePullTimeline->PlayFromStart();
		}
		else
		{
			LodgePointBaseRotation = LodgePoint->GetRelativeRotation();
			AxeLodgeWiggleTimeline->SetPlayRate(3.5f);
			AxeLodgeWiggleTimeline->PlayFromStart();
		}
		///HERRE
		break;
	case EAxeState::Returning:
		break;
	default:
		break;
	}
	
}

void ALeviathanAxe::AxeLodgePull(const float Value) const
{
	const FRotator Rotator = FRotator(LodgePointBaseRotation.Pitch + (Value * -40),LodgePointBaseRotation.Yaw,LodgePointBaseRotation.Roll);
	LodgePoint->SetRelativeRotation(Rotator);
}

void ALeviathanAxe::AxeLodgeWiggle(const float Value) const
{
	const FRotator Rotator = FRotator(LodgePointBaseRotation.Pitch,LodgePointBaseRotation.Yaw,LodgePointBaseRotation.Roll + (Value * 12));
	LodgePoint->SetRelativeRotation(Rotator);
	
}

void ALeviathanAxe::InitialiseReturnVariables()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(StartReturn);
	
	DistanceFromChar = GetClampedAxeDistanceFromChar(MaxCalculationDistance);
	AdjustAxeReturnLocation();
	InitialLoc = GetActorLocation();
	InitialRot = GetActorRotation();
	if (const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0))
	{
		CameraStartRotation = CameraManager->GetCameraRotation(); 
	}
	LodgePoint->SetRelativeRotation(FRotator::ZeroRotator);
	AxeReturnCurveTimeline->SetPlayRate(CalculateReturnTimelineSpeed(OptimalDistance,3));//asd
	bIsRotating = true;
	AxeReturnCurveTimeline->PlayFromStart();
	AxeReturnSpin(CalculateReturnTimelineSpeed(OptimalDistance,3));
	AxeLocationLastTick = ReturnTargetLocation;
	AxeTrace_Return->PlayFromStart();
}

void ALeviathanAxe::AxeReturnPosition()
{
	const float TimelineTime = AxeReturnCurveTimeline->GetPlaybackPosition();
	
	if (const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0))
	{
		ReturnTargetLocation = FMath::Lerp(InitialLoc, Char_Ref->GetMesh()->GetSocketLocation("hand_rSocket")+
			(CameraManager->GetRootComponent()->GetRightVector()*
				(RightVectorCurve->GetFloatValue(TimelineTime)*(DistanceFromChar/AxeReturnRightVectorScale))), AxeReturnSpeedCurve->GetFloatValue(TimelineTime));
	}

	const FRotator Rotator = UKismetMathLibrary::RLerp(
		InitialRot,
		Char_Ref->GetMesh()->GetSocketRotation("hand_rSocket"),
		AxeReturnRotationCurve2->GetFloatValue(TimelineTime),true);


	if (bIsRotating)
	{
		SetActorLocation(ReturnTargetLocation);
	}
	else
	{
		SetActorLocationAndRotation(ReturnTargetLocation,FRotator(Rotator.Pitch ,Rotator.Yaw , Rotator.Roll));
	}
	
	if (ReturnWhoosh)
	{
		ReturnWhoosh->SetVolumeMultiplier(AxeReturnSoundCurve->GetFloatValue(TimelineTime));
	}

	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,TEXT("Return"));
	
}

void ALeviathanAxe::AxeReturnSpin(const float ReturnTimelinePlayRate)
{
	if (Return_Sound)
	{	
		UGameplayStatics::SpawnSoundAttached(Return_Sound, SkeletalMesh, NAME_None, FVector::ZeroVector, EAttachLocation::KeepRelativeOffset,
		true,1,1,0.87f - (1/ReturnTimelinePlayRate))
		->FadeIn(0.1f,1,0.87f - (1/ReturnTimelinePlayRate));
	}
	RotatingMovement->Deactivate();
	NumberOfReturnSpins = UKismetMathLibrary::Round((1/ReturnTimelinePlayRate)/ReturnSpinRate);
	UE_LOG(LogTemp, Warning, TEXT("Return Spins"));
	float a = 1/ReturnTimelinePlayRate;
	a -= 0.055f;
	a = a/UKismetMathLibrary::Round((1/ReturnTimelinePlayRate)/ReturnSpinRate);
	a = 1/a; 
	SpinForTheWIn->SetPlayRate(a);
	SpinForTheWIn->PlayFromStart();
	RotatingMovement->RotationRate = FRotator(FMath::IsFinite(a/NumberOfReturnSpins) && a/NumberOfReturnSpins !=0 ? a/NumberOfReturnSpins*1080 : 0, 0, 0);
	RotatingMovement->Activate();
}

void ALeviathanAxe::AxeReturnSpinTimeline()
{
	
}


double ALeviathanAxe::GetClampedAxeDistanceFromChar(const double MaxDistance) const
{
	return FMath::Clamp((GetActorLocation()-Char_Ref->GetMesh()->GetSocketLocation(FName("AxeSocket"))).Length(),0,MaxDistance);
}

void ALeviathanAxe::AdjustAxeReturnLocation()
{
	FVector Location = GetActorLocation()+FVector(0, 0, (((Z_Adjustment/10)-1)*30)+20);
	SetActorLocation(Location);
}

void ALeviathanAxe::AxeReturnTrace()
{

	// Start and end points for the trace
	const FVector StartLocation = ReturnTargetLocation;    // Starting point of the trace
	const FVector EndLocation = AxeLocationLastTick;

	// Trace radius
	constexpr float TraceRadius = 50.0f;

	// Array to hold the hit results
	FHitResult OutHits;

	// Set collision channel and trace parameters
	TArray<AActor*> ActorsToIgnore;  // Optionally, ignore certain actors
	ActorsToIgnore.Add(Char_Ref);
	constexpr bool bTraceComplex = false;
	constexpr EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;

	// Perform the multi-sphere trace

	if (UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),                    // World context
		StartLocation,                 // Start location
		EndLocation,                   // End location
		TraceRadius,                   // Radius of the sphere
		UEngineTypes::ConvertToTraceType(ECC_Visibility), // Collision channel
		bTraceComplex,                 // Whether to perform complex collision
		ActorsToIgnore,                // Actors to ignore
		DrawDebugType,                 // Whether to draw debug trace (for testing)
		OutHits,                       // Output array of hits
		false,                          // Enable tracing against the entire world
		FLinearColor::Red,           // Debug color (hit)
		FLinearColor::Green,             // Debug color (no hit)
		1.0f                           // Debug line duration
	))
	{
		// Return HiT
	}
	AxeLocationLastTick = ReturnTargetLocation;
	
}

void ALeviathanAxe::CatchAxe()
{
	StopAxeMoving();
	CharacterCatchAxe();
	ProjectileMovement->Deactivate();
	P_AxeCatch->Activate();
	ThrowParticles->EndTrails();
}

double ALeviathanAxe::CalculateReturnTimelineSpeed(const double InOptimalDistance, const double InAxeReturnSpeed) const
{
	return FMath::Clamp((InOptimalDistance*InAxeReturnSpeed)/DistanceFromChar,0.4f,7);
}

void ALeviathanAxe::SetupTimeline()
{
	
	if (AxeThrowCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("ThrowTrace"));
		AxeThrowTraceTimeline->AddInterpFloat(AxeThrowCurve, ProgressFunction);
		AxeThrowTraceTimeline->SetLooping(true);
	}

	if (AxeLodgePullCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("AxeLodgePull"));
		AxeLodgePullTimeline->AddInterpFloat(AxeLodgePullCurve, ProgressFunction);
		AxeLodgePullTimeline->SetLooping(false);
		AxeLodgePullTimeline->SetTimelineLength(0.1f);

		AxeLodgePullTimeline->SetTimelineFinishedFunc(FSimpleDelegate::CreateLambda([this]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Axe Lodge Pull Timeline Finished with Lambda!"));
			ThrowParticles->BeginTrails("BaseSocket","TipSocket",ETrailWidthMode_FromCentre,1.3f);
			AxeState = EAxeState::Returning;
			InitialiseReturnVariables();
		}));
	}

	if (AxeLodgeWiggleCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("AxeLodgeWiggle"));
		AxeLodgeWiggleTimeline->AddInterpFloat(AxeLodgeWiggleCurve, ProgressFunction);
		AxeLodgeWiggleTimeline->SetLooping(false);
		AxeLodgeWiggleTimeline->SetTimelineLength(0.1f);

		AxeLodgeWiggleTimeline->SetTimelineFinishedFunc(FSimpleDelegate::CreateLambda([this]()
	   {
			UE_LOG(LogTemp, Warning, TEXT("Axe Lodge Wiggle Timeline Finished with Lambda!"));
			ThrowParticles->BeginTrails("BaseSocket","TipSocket",ETrailWidthMode_FromCentre,1.3f);
			AxeState = EAxeState::Returning;
			InitialiseReturnVariables();
	   }));
	}


	if (AxeReturnFlatCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("AxeReturnPosition"));
		AxeReturnCurveTimeline->AddInterpFloat(AxeReturnFlatCurve, ProgressFunction);

		AxeReturnCurveTimeline->SetTimelineFinishedFunc(FSimpleDelegate::CreateLambda([this]()
	   {
			AxeTrace_Return->Stop();
			UE_LOG(LogTemp, Warning, TEXT("Return Timeline Finished with Lambdas!"));
			ReturnWhoosh->FadeOut(0.4f, 0,EAudioFaderCurve::Linear);
			CatchAxe();
			//UGameplayStatics::SetGamePaused(GetWorld(), true);
			
	   }));
	}

	if (SpinForTheWInCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("AxeReturnSpinTimeline"));
		SpinForTheWIn->AddInterpFloat(SpinForTheWInCurve, ProgressFunction);

		SpinForTheWIn->SetTimelineFinishedFunc(FSimpleDelegate::CreateLambda([this]()
		{
			NumberOfReturnSpins--;
			if (NumberOfReturnSpins == 0)
			{
				RotatingMovement->Deactivate();
				bIsRotating = false;
				//UGameplayStatics::SetGamePaused(GetWorld(), true);
			}
			else
			{
				SpinForTheWIn->PlayFromStart();
			}
		}));
	}

	if (AxeReturnFlatCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("AxeReturnTrace"));
		AxeTrace_Return->AddInterpFloat(AxeReturnFlatCurve, ProgressFunction);
		AxeTrace_Return->SetTimelineLength(0.05f);
		AxeTrace_Return->SetLooping(true);
	}
}

