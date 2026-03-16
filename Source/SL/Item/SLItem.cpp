// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Item/SLItem.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Misc/DataValidation.h"
#include "SL/Abilities/SLAbilitySystemComponent.h"
#include "SL/Character/SLHero.h"
#include "SL/Data/ItemData.h"
#include "SL/Data/SLAssetManager.h"
#include "SL/Util/SLLogChannels.h"

ASLItem::ASLItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
    
	CapsuleComponent->SetCapsuleHalfHeight(60.f);
	CapsuleComponent->SetCapsuleRadius(40.f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Trigger")); 

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(CapsuleComponent);
    
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 하지만 플레이어 이동엔 방해 안 되게
	
}

UItemData* ASLItem::GetItemData()
{
	if (ItemData)
	{
		return ItemData;
	}
	if (ItemTag.IsValid())
	{
		ItemData = USLAssetManager::Get().GetItemDataByTag(ItemTag);
	}

	return ItemData;
}

void ASLItem::BeginPlay()
{
	Super::BeginPlay();

	USLAssetManager& AssetManager = USLAssetManager::Get();
	if (auto LoadedItemData = AssetManager.GetItemDataByTag(ItemTag))
	{
		ItemData = LoadedItemData;		
	}

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLItem::OnSphereBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ASLItem::OnSphereEndOverlap);
}

EDataValidationResult ASLItem::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
	if (!ItemTag.IsValid())
	{
		Context.AddError(FText::FromString(TEXT("ItemTag 가 비어있습니다. 반드시 설정해야 합니다.")));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}

void ASLItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASLHero>(OtherActor) && OtherActor != this)
	{
		SendOverlapMessage(OtherActor, true);
	}
}

void ASLItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ASLHero>(OtherActor) && OtherActor != this)
	{
		SendOverlapMessage(OtherActor, false);
	}
}

void ASLItem::SendOverlapMessage(AActor* OtherActor, bool bIsOverlap)
{
	FSLItemOverlapMessage Msg;
	Msg.OverlappedActor = OtherActor;
	Msg.ItemActor = this;
	Msg.bIsOverlapped = bIsOverlap;
	Msg.ItemName = ItemData->ItemName;

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(FGameplayTag::RequestGameplayTag(FName("Message.ItemOverlap")), Msg);
}