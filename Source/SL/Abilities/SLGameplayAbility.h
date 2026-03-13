// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SLGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ESLAbilityActivationPolicy : uint8
{
	InputTriggeredOnce,

	WhileInputActive,

	OnSpawn
};
/**
 * 
 */
UCLASS()
class SL_API USLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class USLAbilitySystemComponent;

public:
	ESLAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	TObjectPtr<class ASLCharacter> GetBSCharacterFromActorInfo() const;
	TObjectPtr<ACharacter> GetCharacterFromActorInfo() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	ESLAbilityActivationPolicy ActivationPolicy;
};
