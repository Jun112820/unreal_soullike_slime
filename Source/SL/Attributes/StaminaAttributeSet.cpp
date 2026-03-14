// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Attributes/StaminaAttributeSet.h"

#include "Net/UnrealNetwork.h"

UStaminaAttributeSet::UStaminaAttributeSet()
	: Stamina(100.f)
	, MaxStamina(100.f)
	, StaminaRegenRate(20.f)
{
}

void UStaminaAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UStaminaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}