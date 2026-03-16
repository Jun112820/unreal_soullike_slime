#include "SL/Abilities/GA_Focus.h"
#include "SL/Enemy/SLEnemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SL/Util/SLLogChannels.h"

UGA_Focus::UGA_Focus()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Focus::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    ACharacter* Hero = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    OwnerPC = ActorInfo->PlayerController.Get();

    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(4000.f); 
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Hero);

    GetWorld()->OverlapMultiByChannel(Overlaps, Hero->GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere, Params);
    //DrawDebugSphere(GetWorld(), Hero->GetActorLocation(), 4000.f,1,FColor::Red, false ,1.0f);

    ASLEnemy* BestTarget = nullptr;
    float MinDistance = MAX_FLT;

    for (const FOverlapResult& Overlap : Overlaps)
    {
        if (ASLEnemy* Enemy = Cast<ASLEnemy>(Overlap.GetActor()))
        {
            FVector2D ScreenPosition;
            bool bIsOnScreen = OwnerPC->ProjectWorldLocationToScreen(Enemy->GetActorLocation(), ScreenPosition);

            if (bIsOnScreen)
            {
                float CurrentDistance = FVector::Dist(Hero->GetActorLocation(), Enemy->GetActorLocation());

                if (CurrentDistance < MinDistance)
                {
                    FHitResult Hit;
                    MinDistance = CurrentDistance;
                    BestTarget = Enemy;
                }
            }
        }
    }

    if (BestTarget)
    {
        TargetActor = BestTarget;
        
        FSLTargetLockMessage Msg;
        Msg.TargetActor = TargetActor;
        Msg.bIsLockedOn = true;
        UGameplayMessageSubsystem::Get(this).BroadcastMessage(FGameplayTag::RequestGameplayTag(FName("Message.LockOn")), Msg);
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UGameplayMessageSubsystem& MsgSubsystem = UGameplayMessageSubsystem::Get(this);
    MsgSubsystem.RegisterListener(
        FGameplayTag::RequestGameplayTag(FName("Message.LockOn")),
        this,
        &UGA_Focus::OnTargetLockMessageReceived 
    );

    // 캐릭터 회전 설정 및 루프 시작...
    Hero->GetCharacterMovement()->bOrientRotationToMovement = false;
    Hero->bUseControllerRotationYaw = true;
    UpdateLockOn();
}

void UGA_Focus::UpdateLockOn()
{
    if (!IsActive() || !TargetActor || !OwnerPC) 
    {
        return; 
    }

    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (Avatar)
    {
        FVector TargetLocation = TargetActor->GetActorLocation();
        FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(Avatar->GetActorLocation(), TargetLocation);
        FRotator CurrentRot = OwnerPC->GetControlRotation();

        FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, GetWorld()->GetDeltaSeconds(), 12.0f);
        OwnerPC->SetControlRotation(FRotator(CurrentRot.Pitch, NewRot.Yaw, CurrentRot.Roll));
    }

    if (IsActive())
    {
        UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.01f);
        if (WaitTask)
        {
            WaitTask->OnFinish.AddDynamic(this, &UGA_Focus::UpdateLockOn);
            WaitTask->ReadyForActivation();
        }
    }
}

void UGA_Focus::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (TargetActor)
    {
        FSLTargetLockMessage Msg;
        Msg.TargetActor = TargetActor;
        Msg.bIsLockedOn = false;
                
        UGameplayMessageSubsystem::Get(this).BroadcastMessage(
            FGameplayTag::RequestGameplayTag(FName("Message.LockOn")), Msg);
    }

    if (ACharacter* Hero = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
    {
        Hero->GetCharacterMovement()->bOrientRotationToMovement = true;
        Hero->bUseControllerRotationYaw = false;
    }

    TargetActor = nullptr;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Focus::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    Super::InputPressed(Handle, ActorInfo, ActivationInfo);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Focus::OnTargetLockMessageReceived(FGameplayTag Channel, const FSLTargetLockMessage& Payload)
{
    if (!IsActive()) return;
    if (Payload.bIsLockedOn == false && Payload.TargetActor == nullptr)
    {
        K2_EndAbility(); 
    }
}