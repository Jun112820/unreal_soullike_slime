// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SLAttributeSet.h"
#include "SL/Attributes/SLAttributeSet.h"
#include "StaminaAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class SL_API UStaminaAttributeSet : public USLAttributeSet
{
	GENERATED_BODY()

public:
	UStaminaAttributeSet();

	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, StaminaRegenRate); 

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

private:
	UPROPERTY(BlueprintReadOnly,  Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly,  Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly,  Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StaminaRegenRate;
};
