// Fill out your copyright notice in the Description page of Project Settings.


#include "CPC_Combat.h"

#include "HP_Bar_Widget.h"
#include "DrawDebugHelpers.h" 
#include "LeviathanAxe.h"
#include "MathUtil.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/ALSBASECHARACTER.H"
#include "Character/Animation/Notify/AttachWeapon.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UCPC_Combat::UCPC_Combat(): bTraceOn(false), ParryTimer(0), bRightAttack(false), bIsEnemy(false), YawValue(0),
                            CombatMode(false),
                            bBlock(false), bIsDodge(false), ForwardDir(0), RightDir(0),
                            ActiveState(),
                            OverlayState(),
                            E_AttackType(),
                            FocusedEnemy(nullptr),
                            Sword_Equip(nullptr),
                            Sword_Holster(nullptr), CharacterBlock(nullptr), ParryKick(nullptr),
                            CurveFloat(nullptr),
                            InstepSpeed(0),
                            bCanTurn(true), StoredKey(), DodgeAngle(0), Timer(0),
                            HP_Bar(nullptr),
                            HP_Bar_Widget(nullptr),
                            HPWidget(nullptr),
                            HPBarWidget(nullptr),
                            PlayerController(nullptr),
                            hpbarR(nullptr),
                            tw(nullptr)
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;


    static ConstructorHelpers::FClassFinder<UHP_Bar_Widget> HPBarWidgetBPClass(
        TEXT("/Game/Path/To/Your/HP_Bar_Widget_BP"));
    HPBarWidgetClass = HPBarWidgetBPClass.Class;

    // ...
}


// Called when the game starts
void UCPC_Combat::BeginPlay()
{
	Super::BeginPlay();

    // Initialize your pointers here
    CharacterOwner = Cast<ACharacter>(GetOwner());
    AlsCharacter = Cast<AALSBaseCharacter>(GetOwner());
    PlayerController = UGameplayStatics::GetPlayerController(CharacterOwner, 0);
    if (CharacterOwner)
    {
        CharacterCapsule = CharacterOwner->GetCapsuleComponent();
        CharacterMovement = CharacterOwner->GetCharacterMovement();
        MeshComponent = CharacterOwner->GetMesh();
        AnimInstance = MeshComponent->GetAnimInstance();
    }

    InitialiseRotateToEnemyTimeline();
}





// Called every frame
void UCPC_Combat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    

    YourTimeline.TickTimeline(DeltaTime);

    if (CharacterOwner->IsPlayerControlled())
    {
        /*if (!bLocked)
        {
           
        }*/
        FindEnemy();
        CheckEndCombat();
    }

    TargetEnemy(DeltaTime);
    //WidgetPosition(hpbarR);
    
    


	// ...
}

void UCPC_Combat::ActionInput(const EAttackMove AttackMove, const EALSOverlayState Overlay)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("ActionInput"));
    StoredKey = AttackMove;
    OverlayState = Overlay;
    if (!IsAttacking) {
        QueueInput();
    }
}

void UCPC_Combat::ConsumedKey(const EAttackMove AttackMove)
{
    switch (AttackMove)
    {
    case EAttackMove::Null:
        break;
    case EAttackMove::LightAttack:
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Light Attack//"));
        LightAttack();
        break;
    case EAttackMove::HeavyAttack:
        break;
    default:
        break;
    }
}

void UCPC_Combat::EnterCombatMode()
{
    const FVector Start = GetOwner()->GetActorLocation();  // Set the start 
    const FVector End = GetOwner()->GetActorLocation();    // Set the end vector
    constexpr float Radius = 2000.0f;                // Set the radius

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(reinterpret_cast<AActor*>(this));
    //ActorsToIgnore.Add(GetOwner());// Ignore self


    if (TArray<FHitResult> OutHits; UKismetSystemLibrary::SphereTraceMultiForObjects(
        this,
        Start,
        End,
        Radius,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration,
        OutHits,  // Pass the TArray directly
        true
    ))
    {
        CombatMode = true;
        // ReSharper disable once CppIncompleteSwitchStatement
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch (OverlayState) {
        case EALSOverlayState::Default:
            CharacterCapsule->SetCapsuleRadius(45);
            break;
        case EALSOverlayState::Sword:
            CharacterCapsule->SetCapsuleRadius(72);
            break;
        }
        

        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Enterd Combat mode"));
        for (const FHitResult& HitResult : OutHits)
        {
            // Access individual HitResult properties here

            AActor* HitActor = HitResult.GetActor();

            bool const bIsItemInArray = EnemyArray.Contains(HitActor);

            if (const UCPC_Combat* CombatClass = HitActor->GetComponentByClass<UCPC_Combat>(); !bIsItemInArray && CombatClass && CombatClass->bIsEnemy)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("hit"));
                EnemyArray.Add(HitActor);
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("added enemy"));
            }
            else
            {
                
            }


            //HitResult
        }


        //(OutHit) 
        if (AlsCharacter) {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("valid"));
            AlsCharacter->SetDesiredGait(EALSGait::Walking);

        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("not valid"));
        }
            
    }
    else
    {
        CombatMode = false;
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Exit Combat Mode"));
        if (FocusedEnemy != nullptr)
        {
            if (UCPC_Combat* FocusedEnemyCpc = FocusedEnemy->GetComponentByClass<UCPC_Combat>(); FocusedEnemyCpc != nullptr)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Bar off"));
                FocusedEnemyCpc->ShowBar(false);
            }
        }
        AlsCharacter->SetDesiredGait(EALSGait::Running);
        AlsCharacter->SetRotationMode(EALSRotationMode::LookingDirection);
        EnemyArray.Empty();
    }


    //draw debug sphere 
    if (true)
    {
        DrawDebugSphere(
            GetWorld(),
            Start,
            Radius,
            12,
            FColor::Red,
            false,
            5.0f,
            0,
            1.0f
        );
    }
}

FVector lastmovement;
bool GBDoOnceFocusEnemy = true;
void UCPC_Combat::FindEnemy()
{
    if (ActiveState != EActiveState::Idle && IsAttacking)
        return;
    
    if (EnemyArray.Num() > 0)
    {
        AActor* TFoundEnemy = nullptr;
        float TempFoundDot = 0;

        CheckAutoFind();

        for (AActor* TEnemy : EnemyArray)
        {

            
            // Calculate the distance
            const float Distance = FVector::Dist(GetOwner()->GetActorLocation(), TEnemy->GetActorLocation());

        

             FVector Dif = TEnemy->GetActorLocation() - GetOwner()->GetActorLocation();
            Dif.Normalize();

            // Actor forward vec
            PlayerController = UGameplayStatics::GetPlayerController(this, 0);
            const APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
            FVector ForwardVector = PlayerCameraManager->GetActorForwardVector();
            if (PlayerCameraManager->GetActorForwardVector() != ForwardVector)
                break;

            // Last input vec
            const FVector LastInput = CharacterMovement->GetLastInputVector();

            // Vec select
            //bool bPickA = true;
            FVector Result = UKismetMathLibrary::SelectVector(ForwardVector, LastInput, bPickA);
            Result.Normalize();
            if (!bPickA){Timer++;}
            if (lastmovement != CharacterMovement->GetLastInputVector()){Timer = 0;}


            /*if (Result == LastInput) {
                return;
            }*/

           // FVector Combine = LastInput + ForwardVector;

            if(Distance <= 1000)
            {
                if (!bPickA && Timer <80)
                {
                    lastmovement = CharacterMovement->GetLastInputVector();
                    return;
                }else if (bPickA)  
                {
                    Timer = 0;
                }
                if (const float TemptDot = Dif.Dot(Result); TemptDot > TempFoundDot)
                {
                    GEngine->AddOnScreenDebugMessage(1,5,FColor::Black, TEXT("asdaasda")); 
                    TempFoundDot = TemptDot;
                    TFoundEnemy = TEnemy;
                }
                
                // if (bPickA)
                // {
                //     if (const float TemptDot = Dif.Dot(Combine); TemptDot > TempFoundDot)
                //     {
                //         TempFoundDot = TemptDot;
                //         TFoundEnemy = TEnemy;
                //     }
                // }
                // else
                // {
                //     if (const float TemptDot = Dif.Dot(Result); TemptDot > TempFoundDot)
                //     {
                //         TempFoundDot = TemptDot;
                //         TFoundEnemy = TEnemy;
                //     }
                //
                // }
                
            }
                
            
        }

        if (!TFoundEnemy)
            return;
        SelectFocusEnemy(TFoundEnemy);
         
        
        

        /*if (!TFoundEnemy && EnemyArray.Num() > 0)
        {
            float maxDistance = 0;

            for (AActor* enemy : EnemyArray)
            {
                float distance = enemy->GetDistanceTo(GetOwner());
                if (distance > maxDistance)
                {
                    maxDistance = distance;
                    TFoundEnemy = enemy;
                }
            }
        }*/

       

        
    }
    else
    {
        SelectFocusEnemy(nullptr);
    }
}
bool GBShouldDoOnce = true;
void UCPC_Combat::CheckAutoFind()
{
    PlayerController = UGameplayStatics::GetPlayerController(this, 0); // Use the appropriate player index
    const FRotator ControlRotation = PlayerController->GetControlRotation();

    if (FMath::Abs(ControlRotation.Yaw - YawValue) > 15)
    {
        // Rotation has changed
        bPickA = true;
        GBShouldDoOnce = true;
        // You can add any additional actions here

        // Update YawValue to the new control rotation
        YawValue = ControlRotation.Yaw;
    }
    else if (CharacterMovement->GetLastInputVector().SizeSquared() >= 0.64f) // Adjust the threshold as needed
    {
        // Movement input is significant
        bPickA = false;
        GBShouldDoOnce = false;
        // You can add any additional actions here
    }

    // if (CharacterMovement->GetLastInputVector().Length() >= 0.8f)
    // {
    //     
    //     if (GBShouldDoOnce)
    //     {
    //         YawValue = ControlRotation.Yaw;
    //         bPickA = false;
    //         GBShouldDoOnce = false;
    //         //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("vector input"));
    //     }
    // }
    // else
    // {
    //     if (ControlRotation.Yaw > YawValue + 15 || ControlRotation.Yaw < YawValue - 15)
    //     {
    //         //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("rot"));
    //         bPickA = true;
    //         GBShouldDoOnce = true;
    //     }
    // }

}


void UCPC_Combat::SelectFocusEnemy(AActor* Enemy)
{

    if (Enemy != FocusedEnemy && !IsAttacking)
    {
        if (FocusedEnemy != nullptr)
        {
            if (UCPC_Combat* FocusedEnemyCpc = FocusedEnemy->GetComponentByClass<UCPC_Combat>(); FocusedEnemyCpc != nullptr)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Bar off"));
                FocusedEnemyCpc->ShowBar(false);
            }
        }

        // not sure !!!!!
        if (!Enemy)
            FocusedEnemy = nullptr;

        FocusedEnemy = Enemy;
        
        
        if (FocusedEnemy != nullptr && CombatMode)
        {
            UCPC_Combat* FocusedEnemyCpc = FocusedEnemy->GetComponentByClass<UCPC_Combat>();
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Bar On gacı"));
            FocusedEnemyCpc->ShowBar(true); 
            if (CurveFloat) { 
                
                YourTimeline.PlayFromStart();
            }
            bCanTurn = false;
        }
    }   
}

void UCPC_Combat::LightAttack()
{
    if (!CharacterOwner || !CanAttack()) {

        if(!CanAttack())
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("cant attack"));
        if (!CharacterOwner)
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("2"));
        return;
    }
    ActiveState = EActiveState::Attack;
    IgnoreRootMotion();
    //Rotate to enemy
    if (CurveFloat) { YourTimeline.PlayFromStart();}
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Curv not exist"));
    }
        
    
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attacking"));

    switch (OverlayState)
    {
    case EALSOverlayState::Default:
        {
            const int NewIndex = FMath::RandRange(0,FistMontageArray.Num()-1);
            CharacterOwner->PlayAnimMontage(FistMontageArray[NewIndex],1.25f);
            break;
        }
    case EALSOverlayState::Sword:
        {
            //Sword
            int32 PreviousIndex = -1;
            [[maybe_unused]] int32 NewIndex;
            do
            {
                NewIndex = FMath::RandRange(0,FistMontageArray.Num()-1);
            }
            while (NewIndex == PreviousIndex);
            // ReSharper disable once CppAssignedValueIsNeverUsed
            PreviousIndex = NewIndex;
            CharacterOwner->PlayAnimMontage(SwordMontageArray[FMath::RandRange(0, SwordMontageArray.Num() - 1)], 1.25f);
        }
        break;
    case EALSOverlayState::Leviathan:
        {
            if (bIsAiming)
            {
                ActiveState = EActiveState::Idle;
            }
            else
            {
                //Leviathan
                int32 PreviousIndex = -1;
                int32 NewIndex;
                do
                {
                    NewIndex = FMath::RandRange(0,FistMontageArray.Num()-1);
                }
                while (NewIndex == PreviousIndex);
                // ReSharper disable once CppAssignedValueIsNeverUsed
                PreviousIndex = NewIndex;
                CharacterOwner->PlayAnimMontage(LeviathanMontageArray[FMath::RandRange(0, LeviathanMontageArray.Num() - 1)], 1.3f);   
            }
        }
        break;
    default:
        ActiveState = EActiveState::Idle;
        break;
    }

    

    
}

void UCPC_Combat::IgnoreRootMotion()
{
    const FVector LastInputVector = CharacterMovement->GetLastInputVector();

    // Check if the last input vector is not equal to the comparison vector
    if (const FVector ComparisonVector = FVector(0.0f, 0.0f, 0.0f); LastInputVector != ComparisonVector && !FocusedEnemy)
    {
        AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
        GetWorld()->GetTimerManager().SetTimer(DelayedFunctionTimerHandleIG, this, &UCPC_Combat::EnableRootMotion, 0.25f, false);
    }
}

void UCPC_Combat::EnableRootMotion() const
{
    AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Root motion enabled"));
}

void UCPC_Combat::ResetState() const
{


    //ActiveState = EActiveState::Null;
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Reseted"));
    if (ActiveState == EActiveState::Attack)
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("attack"));  
}

bool UCPC_Combat::CanAttack() const
{
    return ActiveState == EActiveState::Idle && !CharacterMovement->IsFalling();
}


void UCPC_Combat::Initialise(UWidgetComponent* WidgetBar, UStaticMeshComponent* swordSK, UUserWidget* HpBar)
{
    HP_Bar = WidgetBar;
    Sword = swordSK;
    hpbarR = HpBar;   
}

void UCPC_Combat::ShowBar(bool Show)
{
    if (!HP_Bar)
        return;
    HP_Bar->SetVisibility(Show);
}

void UCPC_Combat::RotateToEnemyTimelineUpdate(float Value) const
{
    RotateToEnemy(FocusedEnemy);
}

void UCPC_Combat::OnTimelineEnd()
{
    bCanTurn = true;
}

void UCPC_Combat::InitialiseRotateToEnemyTimeline()
{
    if (CurveFloat)
    {
        YourTimeline.SetPropertySetObject(this);
        YourTimeline.SetDirectionPropertyName(FName("TimelineDirection"));
        YourTimeline.SetLooping(false);
        YourTimeline.SetTimelineLength(0.5f);

        FOnTimelineFloatStatic TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("RotateToEnemyTimelineUpdate"));
        YourTimeline.AddInterpFloat(CurveFloat, TimelineCallback);

        FOnTimelineEventStatic TimelineEnd;
        TimelineEnd.BindUFunction(this, FName("OnTimelineEnd"));
        YourTimeline.SetTimelineFinishedFunc(TimelineEnd);
    }

}

void UCPC_Combat::TargetEnemy(const float DeltaTime) const
{
    if (FocusedEnemy && CombatMode && AlsCharacter && bCanTurn && CharacterOwner->IsPlayerControlled()) { 
        AlsCharacter->SetRotationMode(EALSRotationMode::Enemy);

        FVector DirectionToOtherActor = FocusedEnemy->GetActorLocation() - GetOwner()->GetActorLocation();
        DirectionToOtherActor.Normalize();

        // calculate the yaw from the direction vector
        const float TargetYaw = FMath::RadiansToDegrees(FMath::Atan2(DirectionToOtherActor.Y, DirectionToOtherActor.X));
        
        // Turn to the calculated yaw
        TurnToYaw(TargetYaw, DeltaTime);
    }else if (!bCanTurn)
    {
        //GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Black,TEXT("rotate"));
    }
}

void UCPC_Combat::TurnToYaw(const float TargetYaw, const float DeltaTime) const
{

    // Actor yaw
    const FRotator CurrentRotation = GetOwner()->GetActorRotation();

    // Set yaw to Rotator
    const FRotator NewRotation(0.0f, TargetYaw, 0.0f);

    // Interpolate rotation 
    const FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewRotation, DeltaTime, 100);

    // Set actor rotation
    GetOwner()->SetActorRotation(InterpolatedRotation);
    //○GEngine->AddOnScreenDebugMessage(-1,2,FColor::Black,TEXT("player"));
}

void UCPC_Combat::QueueInput()
{
    //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Que input"));
    ConsumedKey(StoredKey);
    StoredKey = EAttackMove::Null;
}

void UCPC_Combat::WidgetPosition(UUserWidget* Widget) const
{
    if (FocusedEnemy && Widget)
    {
        Widget->SetVisibility(ESlateVisibility::Visible);
        Widget->SetAlignmentInViewport(FVector2D(0.5, 0.5));

        FVector2D WorldPos;
        FVector2D ScreenPos;

        FVector Enemy_Location = FocusedEnemy->GetActorLocation();
        Enemy_Location.Z += 100;

        UGameplayStatics::ProjectWorldToScreen(PlayerController, Enemy_Location, ScreenPos);

        int32 ViewportSizeX;
        int32 ViewportSizeY;
        PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
        const FVector2D ViewportSize(ViewportSizeX, ViewportSizeY);


        const FVector2D WidgetSize = Widget->GetDesiredSize()/2;
        const FVector2D Subtract = ViewportSize - WidgetSize;

        WorldPos.X = FMath::Clamp(ScreenPos.X, 125, Subtract.X);
        WorldPos.Y = FMath::Clamp(ScreenPos.Y, 10, Subtract.Y);

        Widget->SetPositionInViewport(WorldPos, true);

    }else if (Widget && !FocusedEnemy)
    {
        Widget->SetVisibility(ESlateVisibility::Hidden);
    }
}




void UCPC_Combat::Dodge()
{
    if(ActiveState != EActiveState::Idle )
        return;
    CalculateDirection();
    CalculateDodgeDirection();
    bCanTurn = false;
    AnimInstance->Montage_Play(Dodge_Montages.FindRef(DodgeVector));
    if (CurveFloat) { YourTimeline.PlayFromStart();}
}



void UCPC_Combat::CalculateDirection()
{
    const AActor* OwnerActor = GetOwner();
    //const FVector Velocity = OwnerActor->GetVelocity();
    const FVector Velocity = CharacterMovement->GetCurrentAcceleration();
    const FVector ForwardVector = OwnerActor->GetActorForwardVector();

    const float DotProduct = FVector::DotProduct(Velocity.GetSafeNormal(), ForwardVector.GetSafeNormal());
    const FVector CrossProduct = FVector::CrossProduct(Velocity.GetSafeNormal(), ForwardVector.GetSafeNormal());
    
    float AngleRadians = FMath::Acos(DotProduct);
    if (CrossProduct.Z < 0)
    {
        AngleRadians = 2 * PI - AngleRadians;
    }
    
    DodgeAngle = AngleRadians;
}

void UCPC_Combat::CalculateDodgeDirection()
{
    for (int32 Index = 0; Index <= 8; Index++)
    {
        
        if (FMath::RadiansToDegrees(DodgeAngle) > Index * 45 + 22.5)
        {
            continue;
        }
        else
        {
            if (FMath::RadiansToDegrees(DodgeAngle) != 90)
            {
            #pragma region Dodge Direciton
                switch (Index)
                {
                case 0:
                    DodgeVector = FVector(1,0,0);
                    break;
                case 1:
                    DodgeVector = FVector(1,-1,0);
                    break;
                case 2:
                    DodgeVector = FVector(0,-1,0);
                    break;
                case 3:
                    DodgeVector = FVector(-1,-1,0);
                    break;
                case 4:
                    DodgeVector = FVector(-1,0,0);
                    break;
                case 5:
                    DodgeVector = FVector(-1,1,0);
                    break;
                case 6:
                    DodgeVector = FVector(0,1,0);
                    break;
                case 7:
                    DodgeVector = FVector(1,1,0);
                    break;
                case 8:
                    DodgeVector = FVector(1,0,0);
                    break;
                default:;
                    break;
                }
                break;
            #pragma endregion
            }
 
            else
            {
                DodgeVector = FVector(-1,0,0);
            }
        }
    }
}



void UCPC_Combat::RotateToEnemy(AActor* Enemy) const
{
    if (!Enemy) 
        return;
    if (IsAttacking)
        return;
    const FVector CharacterLocation = CharacterOwner->GetActorLocation();
    const FVector EnemyLocation = Enemy->GetActorLocation();
    const FRotator FindLookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, EnemyLocation);

    // Interpolate actor rotation towards the target rotation using FMath::RInterpTo
    const FRotator İnterpTo = FMath::RInterpTo(GetOwner()->GetActorRotation(), FindLookAtRotation, 0.1 , 1);

    // Set the actor's rotation
    GetOwner()->SetActorRotation(İnterpTo);
    
}

void UCPC_Combat::TraceOn(const bool RightAttack, const bool Trace, const EAttackType AttackType)
{
    E_AttackType = AttackType;

    if (E_AttackType == EAttackType::Fist)
    {
        bRightAttack = RightAttack;
    }

    if (Trace) {
        bTraceOn = true;               
    }
    else
    {
        bTraceOn = false;
        EnemyToHit.Empty();
    }
}

void UCPC_Combat::PerformTrace()
{
    if (!MeshComponent)
        return;

    TArray<FHitResult> OutHits;

    if (!SelectAttackTrace(OutHits))
        return;
    
    CombatMode = true;

    if (E_AttackType == EAttackType::Leviathan)
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            // Restore Custom Time Dilation to 1.0 (normal behavior)
            CharacterOwner->CustomTimeDilation = 1;
        }, 0.08f, false);
        CharacterOwner->CustomTimeDilation = 0;   
    }
    

    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hitting"));
    for (const FHitResult& HitResult : OutHits)
    {
        AActor* HitActor = HitResult.GetActor();
        
        FString ActorName = HitActor->GetName();
        UE_LOG(LogTemp, Warning, TEXT("Actors name is : %s"), *ActorName);

        if (EnemyToHit.Contains(HitActor))
            return;
    
        EnemyToHit.Add(HitActor);

        UCPC_Combat* CombatClass = HitActor->GetComponentByClass<UCPC_Combat>();
         
        if (!HitActor)
            return;
        if (!CombatClass)
            return;

        //Rotate To Character
        FVector CharacterLocation = HitActor->GetActorLocation();
        FVector EnemyLocation = GetOwner()->GetActorLocation();
        FRotator FindLookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, EnemyLocation);
        const FRotator İnterpTo = FMath::RInterpTo(HitActor->GetActorRotation(), FindLookAtRotation, 1, 2.5 + 4);
        HitActor->SetActorRotation(İnterpTo);
        CombatClass->GetHit(CharacterOwner, Montages.FindRef(AnimInstance->GetCurrentActiveMontage()));  
    }
}

void UCPC_Combat::GetHit(const ACharacter* Attacker, const FStrucHitReactions HitReaction) 
{
    UHP_Bar_Widget* Temptbar = nullptr;
    if (HP_Bar)
        Temptbar = Cast<UHP_Bar_Widget>(HP_Bar->GetWidget());
    if ((!bBlock) || ActiveState == EActiveState::Stunt)
    {
        AnimInstance->Montage_Play(HitReaction.GetHit,1.25f);//anim
        if (Temptbar)
        {
            Temptbar->SetHP(-10);
            if (Temptbar->GetHP() < 0)
            {
                AlsCharacter->RagdollStart();
                CharacterOwner->AIControllerClass = nullptr;
                bIsEnemy = false;
            }
        }
    }
    if (bBlock && ParryTimer < 20 && CharacterOwner->IsPlayerControlled())
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Parry"));
        AnimInstance->Montage_Play(ParryKick);
        Attacker->GetMesh()->GetAnimInstance()->Montage_Stop(0.01f);
        Attacker->GetMesh()->GetAnimInstance()->Montage_Play(HitReaction.Parry);
    }else if (bBlock && !CharacterOwner->IsPlayerControlled())
    {
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Black,TEXT("blocked"));
        AnimInstance->StopAllMontages(0);
        AnimInstance->Montage_Play(HitReaction.Parry);
    }else if (bBlock && CharacterOwner->IsPlayerControlled())
    {
        
        AnimInstance->Montage_Play(CharacterBlock);
    }

}

void UCPC_Combat::CheckEndCombat()
{
    if (!FocusedEnemy) 
    {
        if (CombatMode) {
            AlsCharacter->SetDesiredGait(EALSGait::Running);
            EnterCombatMode();
            CombatMode = false;
            //GBDoOnceFocusEnemy = true;
            CharacterCapsule->SetCapsuleRadius(35);
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CombatMode = false there is no enemy"));
        }
        return;
    }

    if (const float Distance = GetOwner()->GetDistanceTo(FocusedEnemy); Distance > 2000)
    {
        
            //EnemyArray.Empty();
            EnterCombatMode();
          
        
    }
}

void UCPC_Combat::AttachWeapon(const bool bAdd, const EALSWaeponChoiseState State) const
{
    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Attach"));
    if (!Sword || !MeshComponent)
    {
        // Handle null pointers (e.g., log an error)
        return;
    }
    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);

    FName SocketName;

    // Switch based on the weapon state
    
    switch (State)
    {
    case EALSWaeponChoiseState::Sword:
        // Decide which socket to attach based on bAdd
            SocketName = bAdd ? TEXT("SwordSocket") : TEXT("SheatSocket");
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("SwordAttach"));  

        // Attach the sword to the mesh
        Sword->AttachToComponent(MeshComponent, AttachmentRules, SocketName);
        break;

    case EALSWaeponChoiseState::Leviathan:
        // Decide which socket to attach based on bAdd
        SocketName = bAdd ? TEXT("hand_rSocket") : TEXT("spine_03Socket");
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("LeviathanAttach"));
        
        // Attach the Leviathan axe to the mesh
        LeviathanAxe->AttachToComponent(MeshComponent, AttachmentRules, SocketName);
        break;

    default:
        LeviathanAxe->AttachToComponent(MeshComponent, AttachmentRules, TEXT("spine_03Socket"));
        Sword->AttachToComponent(MeshComponent, AttachmentRules, TEXT("SheatSocket"));
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("Unexpected Weapon State"));
        break;
    }
}

void UCPC_Combat::EquipWeapon(const bool bEquip, const EALSOverlayState State)
{
    //ActiveState = EActiveState::Interaction;
    
    AnimRef = nullptr;
    
    switch (State) {
    case EALSOverlayState::Sword:
        AnimRef = bEquip ? Sword_Equip : Sword_Holster;
        break;
    case EALSOverlayState::Leviathan:
        if (!bAxeThrown)
        {
            AnimRef = bEquip ? Axe_Equip : Axe_Holster;
        }
        break;
    default: ;
    }

    CharacterOwner->PlayAnimMontage(AnimRef);
    

}

bool UCPC_Combat::SelectAttackTrace(TArray<FHitResult>& OutHits)
{
    //Fist Socket
    const FVector SocketLocation = bRightAttack
                                       ? MeshComponent->GetSocketLocation("middle_01_r")
                                       : MeshComponent->GetSocketLocation("middle_01_l");

    const FVector Fist_Start = SocketLocation;
    const FVector Fist_End = SocketLocation;
    
    //Sword Socket
    const FVector Sword_Start = Sword->GetSocketLocation("Socket1");
    const FVector Sword_End = Sword->GetSocketLocation("Socket2");
    const FRotator Orientation = Sword->GetSocketRotation("Socket1");
    const FVector HalfSize = FVector(5,5,0);

    FVector Leviathan_Start;
    FVector Leviathan_End;
    FRotator LeviathanOrientation;
    FVector LeviathanHalfSize;
    //Leviathan Socket
    if (LeviathanAxe)
    {
        Leviathan_Start = LeviathanAxe->SkeletalMesh->GetSocketLocation("Socket1");
         Leviathan_End = LeviathanAxe->SkeletalMesh->GetSocketLocation("Socket2");
         LeviathanOrientation = LeviathanAxe->SkeletalMesh->GetSocketRotation("Socket1");
         LeviathanHalfSize = FVector(10,12,10);
    }
    

    

    //MultiTrace Variables
    constexpr float Radius = 15.0f;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(reinterpret_cast<AActor*>(this));
    ActorsToIgnore.Add(GetOwner()); 

    bool bHit = false;

    switch (E_AttackType)
    {
    case EAttackType::Fist:
#pragma region Fist
        bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
            this,
            Fist_Start,
            Fist_End,
            Radius,
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::ForOneFrame,
            OutHits,
            true
        );
#pragma endregion    Fist
        break;
    case EAttackType::Sword:
#pragma region Sword
        
        bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
            this,
            Sword_Start,
            Sword_End,
            HalfSize,  
            Orientation,
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::ForOneFrame,
            OutHits,
            true
        );  
#pragma endregion   Sword  
        break;
    case EAttackType::Leviathan:
#pragma region Leviathan
        bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
            this,
            Leviathan_Start,
            Leviathan_End,
            LeviathanHalfSize,  
            LeviathanOrientation,
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::ForOneFrame,
            OutHits,
            true
        ); 
#pragma endregion Leviathan
        break;
    default:
        break;
    }
    
    return bHit;
}








