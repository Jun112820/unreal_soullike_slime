// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "SLHeroController.generated.h"

class USLInputSet;
/**
 * 
 */
UCLASS()
class SL_API ASLHeroController : public APlayerController
{
	GENERATED_BODY()

public:
	ASLHeroController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	void InputAbilityTagPressed(const FInputActionValue& InputActionValue, FGameplayTag GameplayTag);
	void InputAbilityTagReleased(const FInputActionValue& InputActionValue, FGameplayTag GameplayTag);

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	UPROPERTY()
	TObjectPtr<USLInputSet> HeroInputSet;
};