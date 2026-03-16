// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Character/QuickSlotComponent.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "SL/Abilities/SLAbilitySystemComponent.h"
#include "SL/Data/ItemData.h"
#include "SL/Item/SLItem.h"
#include "SL/Util/SLLogChannels.h"

UQuickSlotComponent::UQuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UQuickSlotComponent::CanUseItem() const
{
	if (!CurrentItemData)
		return false;

	return true;
}

void UQuickSlotComponent::UseCurrentItem()
{
	if (!CurrentItemData)
	{
		UE_LOG(LogSL, Error, TEXT("현재 선택된 아이템이 없습니다."));
		return;
	}
	
	if (auto ASI = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (USLAbilitySystemComponent* MyASC = Cast<USLAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
		{
			if (CurrentItemData->ItemEffectClassArray.Num() > 0)
			{
				MyASC->ApplyItemEffects(CurrentItemData->ItemEffectClassArray, 1.0f, GetOwner());
				UE_LOG(LogSL, Warning, TEXT("아이템 사용 : %s"), *CurrentItemData->ItemName.ToString());
				CurrentItemData = nullptr;
			}
		}
	}
	else
	{
		UE_LOG(LogSL, Error, TEXT("캐릭터에게 ASI 가 없습니다."));
	}
}

void UQuickSlotComponent::AddItem(ASLItem* Item)
{
	if (!Item) return;

	UItemData* NewData = Item->GetItemData();
	if (!NewData) return;

	CurrentItemData = NewData;

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	FSLEquipItemMessage ChangeMessage;
	ChangeMessage.ItemData = CurrentItemData;
	ChangeMessage.EquipActor = GetOwner();
	ChangeMessage.bIsEquip = true;
	MessageSubsystem.BroadcastMessage(FGameplayTag::RequestGameplayTag("Message.EquipItem"), ChangeMessage);
	
	FSLPickUpItemMessage Message;
	Message.ItemData = CurrentItemData;
	Message.PickupActor = GetOwner();
	MessageSubsystem.BroadcastMessage(FGameplayTag::RequestGameplayTag("Message.PickupItem"), Message);

	Item->Destroy();

	UE_LOG(LogSL, Warning, TEXT("아이템 획득 : %s"), *CurrentItemData->ItemName.ToString());
}

void UQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem& MsgSubsystem = UGameplayMessageSubsystem::Get(this);
	MsgSubsystem.RegisterListener(
		FGameplayTag::RequestGameplayTag(FName("Message.ItemOverlap")),
		this,
		&UQuickSlotComponent::OnItemOverlapReceived
	);
}

void UQuickSlotComponent::OnItemOverlapReceived(FGameplayTag Channel, const FSLItemOverlapMessage& Payload)
{
	if (Payload.bIsOverlapped == true && Payload.ItemActor->IsValidLowLevel() && Payload.OverlappedActor == GetOwner())
	{
		CurrentOverlapItem = Payload.ItemActor;
	}

	else if (Payload.bIsOverlapped == false && Payload.OverlappedActor == GetOwner())
	{
		CurrentOverlapItem = nullptr;
	}
}