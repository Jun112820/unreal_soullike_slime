// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SL/Abilities/SLGameplayAbility.h"
#include "GA_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SL_API UGA_MeleeAttack : public USLGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_MeleeAttack(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData Payload);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeTrace();

protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadius = 50.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	TObjectPtr<class UAnimMontage> AttackMontage;
};
