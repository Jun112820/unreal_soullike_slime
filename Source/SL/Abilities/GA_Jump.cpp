#include "SL/Abilities/GA_Jump.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"

UGA_Jump::UGA_Jump(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ActivationPolicy = ESLAbilityActivationPolicy::InputTriggeredOnce;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    return Character && Character->CanJump();
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        Character->Jump();

        UAbilityTask_WaitMovementModeChange* WaitLandingTask = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, EMovementMode::MOVE_Walking);
        
        WaitLandingTask->OnChange.AddDynamic(this, &UGA_Jump::OnLandingTaskChanged); 
        WaitLandingTask->ReadyForActivation();
    }
}

void UGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    // 버튼을 떼면 가변 점프(살짝 누르면 낮게 점프) 처리를 위해 StopJumping 호출
    if (ActorInfo->IsLocallyControlled())
    {
        ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
        if (Character)
        {
            Character->StopJumping();
        }
    }
}

void UGA_Jump::OnLandingTaskChanged(EMovementMode Mode)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
