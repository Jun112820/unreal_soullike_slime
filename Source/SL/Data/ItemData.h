// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS()
class SL_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", ItemTag.GetTagLeafName());
	}
	
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly)
	FText ItemName;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> ItemEffectClassArray;
};