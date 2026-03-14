// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "SL/Interface/IDamageable.h"
#include "SLEnemy.generated.h"

class UHealthAttributeSet;

UCLASS()
class SL_API ASLEnemy : public ACharacter, public IAbilitySystemInterface, public IDamageable
{
	GENERATED_BODY()

public:
	ASLEnemy();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void Die();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class USLAbilitySystemComponent> SLAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UHealthAttributeSet> HealthSet;
    
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<class USLAbilitySet> LoadedAbilitySet;
};