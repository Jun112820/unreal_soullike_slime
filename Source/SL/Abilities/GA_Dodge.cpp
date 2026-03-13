#include "SL/Abilities/GA_Dodge.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "SL/Util/SLLogChannels.h"

UGA_Dodge::UGA_Dodge(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ActivationPolicy = ESLAbilityActivationPolicy::InputTriggeredOnce;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
                                const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
    {
        FVector InputVector = Character->GetLastMovementInputVector();

        // 만약 입력이 있다면 (가만히 서서 구르는 게 아니라면)
        if (!InputVector.IsNearlyZero())
        {
            FRotator TargetRotation = InputVector.Rotation();
            
            TargetRotation.Pitch = 0.f;
            TargetRotation.Roll = 0.f;

            Character->SetActorRotation(TargetRotation);
        }
    }

    if (DodgeMontage)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            NAME_None,
            DodgeMontage,
            1.0f,
            NAME_None,
            false,
            1.0f
        );

        MontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
        MontageTask->OnBlendOut.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
        MontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageInterrupted);
        MontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageInterrupted);

        MontageTask->ReadyForActivation();
    }
    else
    {
        UE_LOG(LogSL, Error, TEXT("DodgeMontage가 설정되지 않았습니다!"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }
}

void UGA_Dodge::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Dodge::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}