// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "HeroViewModel.generated.h"

/**
 * 
 */
UCLASS()
class SL_API UHeroViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeViewModel(class UAbilitySystemComponent* ASC);

public:
	void SetHealthPercent(float InValue) { HealthPercent = InValue; }
	float GetHealthPercent() const { return HealthPercent; }

	void SetStaminaPercent(float InValue) { StaminaPercent = InValue; }
	float GetStaminaPercent() const { return StaminaPercent; }

protected:
	void OnHealthChanged(const struct FOnAttributeChangeData& Data);
	void OnStaminaChanged(const struct FOnAttributeChangeData& Data);
	void RefreshHealth();
	void RefreshStamina();

protected:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category = "UI")
	float HealthPercent;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category = "UI")
	float StaminaPercent;

private:
	TWeakObjectPtr<const class UHealthAttributeSet> HealthSetPtr;
	TWeakObjectPtr<const class UStaminaAttributeSet> StaminaSetPtr;
};
