// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Mvvm/HeroViewModel.h"

#include "SL/Attributes/HealthAttributeSet.h"
#include "SL/Attributes/StaminaAttributeSet.h"

void UHeroViewModel::InitializeViewModel(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;
	
	if (auto HealthSet = ASC->GetSet<UHealthAttributeSet>())
	{
		HealthSetPtr = HealthSet;
			
		ASC->GetGameplayAttributeValueChangeDelegate(HealthSetPtr->GetHealthAttribute())
			.AddUObject(this, &UHeroViewModel::OnHealthChanged);
       
		ASC->GetGameplayAttributeValueChangeDelegate(HealthSetPtr->GetMaxHealthAttribute())
			.AddUObject(this, &UHeroViewModel::OnHealthChanged);

		RefreshHealth();
	}

	if (auto StaminaSet = ASC->GetSet<UStaminaAttributeSet>())
	{
		StaminaSetPtr = StaminaSet;
		ASC->GetGameplayAttributeValueChangeDelegate(StaminaSet->GetStaminaAttribute())
		   .AddUObject(this, &UHeroViewModel::OnStaminaChanged);
        
		RefreshStamina();
	}
}

void UHeroViewModel::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HealthSetPtr.IsValid())
	{
		RefreshHealth();
	}
}

void UHeroViewModel::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (StaminaSetPtr.IsValid())
	{
		RefreshStamina();
	}
}

void UHeroViewModel::RefreshHealth()
{
	if (HealthSetPtr.IsValid())
	{
		const float CurrentHealth = HealthSetPtr->GetHealth();
		const float MaxHealth = HealthSetPtr->GetMaxHealth();
        
		const float NewPercent = (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
        
		UE_MVVM_SET_PROPERTY_VALUE(HealthPercent, NewPercent);
	}
}

void UHeroViewModel::RefreshStamina()
{
	if (StaminaSetPtr.IsValid())
	{
		const float CurrentStamina = StaminaSetPtr->GetStamina();
		const float MaxStamina = StaminaSetPtr->GetMaxStamina();

		const float NewPercent = (MaxStamina > 0.f) ? (CurrentStamina / MaxStamina) : 0.f;

		UE_MVVM_SET_PROPERTY_VALUE(StaminaPercent, NewPercent);
	}
}