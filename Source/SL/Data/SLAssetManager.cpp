// Fill out your copyright notice in the Description page of Project Settings.

#include "SLAssetManager.h"
#include "SL/Data/ItemData.h"
#include "SL/Data/SLAbilitySet.h"
#include "SL/Data/SLInputSet.h"
#include "AbilitySystemGlobals.h"
#include "SL/Util/SLLogChannels.h"

USLAssetManager::USLAssetManager()
{
}

USLAssetManager& USLAssetManager::Get()
{
	check(GEngine);

	if (USLAssetManager* Singleton = Cast<USLAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini. Must set SLAssetManager!"));
	return *static_cast<USLAssetManager*>(nullptr);
}

void USLAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

void USLAssetManager::PostInitialAssetScan()
{
	Super::PostInitialAssetScan();
	
	ScanAndCacheAssets<UItemData>(FPrimaryAssetType("ItemData"), ItemDataMap);
	ScanAndCacheAssets<USLAbilitySet>(FPrimaryAssetType("AbilitySet"), AbilitySetMap);
	ScanAndCacheAssets<USLInputSet>(FPrimaryAssetType("InputSet"), InputSetMap);

	UE_LOG(LogSL, Warning, TEXT("SLAssetManager: Initial Loading Complete. Items: %d, AbilitySets: %d, InputSets: %d"), 
		ItemDataMap.Num(), AbilitySetMap.Num(), InputSetMap.Num());
}

template <typename T>
void USLAssetManager::ScanAndCacheAssets(const FPrimaryAssetType& AssetType, TMap<FGameplayTag, TObjectPtr<T>>& OutMap)
{
	TArray<FAssetData> AssetList;
	GetPrimaryAssetDataList(AssetType, AssetList);

	for (const FAssetData& AssetData : AssetList)
	{
		// GetAsset()은 에셋을 동기적으로 로드합니다.
		if (T* LoadedAsset = Cast<T>(AssetData.GetAsset()))
		{
			FGameplayTag KeyTag;

			// 타입에 따른 태그 추출 (Compile-time 처리) 나중에 virtual 함수 하나만 호출하도록 변경
			if constexpr (std::is_same_v<T, UItemData>)
			{
				KeyTag = LoadedAsset->ItemTag;
			}
			else if constexpr (std::is_same_v<T, USLAbilitySet> || std::is_same_v<T, USLInputSet>)
			{
				KeyTag = LoadedAsset->CharacterTag;
			}

			if (KeyTag.IsValid())
			{
				OutMap.Add(KeyTag, LoadedAsset);
			}
		}
	}
}

UItemData* USLAssetManager::GetItemDataByTag(const FGameplayTag& ItemTag, bool bLogWarning)
{
	if (TObjectPtr<UItemData>* FoundData = ItemDataMap.Find(ItemTag))
	{
		return *FoundData;
	}

	if (bLogWarning)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemData for Tag [%s]"), *ItemTag.ToString());
	}
	return nullptr;
}

USLAbilitySet* USLAssetManager::GetAbilitySetByTag(const FGameplayTag& SetTag, bool bLogWarning)
{
	if (TObjectPtr<USLAbilitySet>* FoundSet = AbilitySetMap.Find(SetTag))
	{
		return *FoundSet;
	}
	return nullptr;
}

USLInputSet* USLAssetManager::GetInputSetByTag(const FGameplayTag& SetTag, bool bLogWarning)
{
	if (TObjectPtr<USLInputSet>* FoundSet = InputSetMap.Find(SetTag))
	{
		return *FoundSet;
	}
	return nullptr;
}