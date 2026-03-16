// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "SLAssetManager.generated.h"

class USLInputSet;
class USLAbilitySet;
class UItemData;
/**
 * 
 */
UCLASS()
class SL_API USLAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	USLAssetManager();
	
	static USLAssetManager& Get();
	// 나중에 보스 페이즈 때는 필요한 것 로딩하는 방식으로 변경

public:
	UItemData* GetItemDataByTag(const FGameplayTag& ItemTag, bool bLogWarning = true);
	USLAbilitySet* GetAbilitySetByTag(const FGameplayTag& SetTag, bool bLogWarning = true);
	USLInputSet* GetInputSetByTag(const FGameplayTag& SetTag, bool bLogWarning = true);
	virtual void StartInitialLoading() override;

protected:
	virtual void PostInitialAssetScan() override;
	
private:
	template<typename T>
	void ScanAndCacheAssets(const FPrimaryAssetType& AssetType, TMap<FGameplayTag, TObjectPtr<T>>& OutMap);
	
private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UItemData>> ItemDataMap;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USLAbilitySet>> AbilitySetMap;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USLInputSet>> InputSetMap;
};