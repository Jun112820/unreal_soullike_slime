// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SLAttributeSet.h"
#include "HealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class SL_API UHealthAttributeSet : public USLAttributeSet
{
	GENERATED_BODY()

public:
	UHealthAttributeSet();

	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Damage);

protected:
	// Begin UAttributeSet~
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	// ~End UAttributeSet

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(BlueprintReadOnly,  Category = "Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly,  Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	UPROPERTY(BlueprintReadOnly, Category="Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Damage;

	float HealthBeforeAttributeChange;
	float MaxHealthBeforeAttributeChange;
};
