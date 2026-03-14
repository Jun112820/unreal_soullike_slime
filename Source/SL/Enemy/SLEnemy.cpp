// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Enemy/SLEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "SL/Abilities/SLAbilitySystemComponent.h"
#include "SL/Attributes/HealthAttributeSet.h"
#include "SL/Data/SLAbilitySet.h"
#include "SL/Util/SLLogChannels.h"

ASLEnemy::ASLEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	SLAbilitySystemComponent = CreateDefaultSubobject<USLAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* ASLEnemy::GetAbilitySystemComponent() const
{
	return SLAbilitySystemComponent;
}

void ASLEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (SLAbilitySystemComponent)
	{
		SLAbilitySystemComponent->InitAbilityActorInfo(this, this);

		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FPrimaryAssetId> AssetIds;
		
		FGameplayTag CharacterTag = FGameplayTag::RequestGameplayTag(FName("Character.Slime")); 
		FPrimaryAssetId AssetId("AbilitySet", CharacterTag.GetTagLeafName());

		AssetManager.LoadPrimaryAsset(AssetId);
		LoadedAbilitySet = Cast<USLAbilitySet>(AssetManager.GetPrimaryAssetObject(AssetId));
		
		if (LoadedAbilitySet)
		{
			SLAbilitySystemComponent->AddActorAbilities(this, *LoadedAbilitySet);
			HealthSet = SLAbilitySystemComponent->GetSet<UHealthAttributeSet>();

			SLAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute())
				.AddUObject(this, &ASLEnemy::HandleHealthChanged);
		}
	}
}

void ASLEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ASLEnemy::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == HealthSet->GetHealthAttribute())
	{
		if (Data.NewValue <= 0)
			Die();
	}
}

void ASLEnemy::Die()
{
	if (SLAbilitySystemComponent)
	{
		SLAbilitySystemComponent->CancelAllAbilities();
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// if (DeathMontage)
	// {
	// 	PlayAnimMontage(DeathMontage);
	// }
	//    
	// if (AAIController* AIC = Cast<AAIController>(GetController()))
	// {
	// 	AIC->StopMovement();
	// 	AIC->UnPossess(); 
	// }

	Destroy();
	//SetLifeSpan(0.0f);
}

