// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Character/SLHero.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SL/Abilities/SLAbilitySystemComponent.h"
#include "SL/Data/SLAbilitySet.h"
#include "SL/Player/SLPlayerState.h"

// Sets default values
ASLHero::ASLHero()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 카메라 붐 생성 (캐릭터 뒤에서 카메라를 당겨옴)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // 카메라와 캐릭터 사이의 거리
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러 입력에 따라 팔 회전

	// 팔로우 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라는 붐에 대해 상대적으로 회전하지 않음
}

void ASLHero::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASLHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASLPlayerState* PS = GetPlayerState<ASLPlayerState>();
	if (PS)
	{
		SLAbilitySystemComponent = Cast<USLAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		
		SLAbilitySystemComponent->RemoveActorAbilities();
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FPrimaryAssetId> AssetIds;
		
		FGameplayTag CharacterTag = FGameplayTag::RequestGameplayTag(FName("Character.Hero")); 
		FPrimaryAssetId AssetId("AbilitySet", CharacterTag.GetTagLeafName());

		AssetManager.LoadPrimaryAsset(AssetId);
		LoadedAbilitySet = Cast<USLAbilitySet>(AssetManager.GetPrimaryAssetObject(AssetId));
        
		if (LoadedAbilitySet && SLAbilitySystemComponent)
		{
			SLAbilitySystemComponent->AddActorAbilities(this, *LoadedAbilitySet);
		}
	}
}

void ASLHero::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (SLAbilitySystemComponent)
	{
		SLAbilitySystemComponent->RemoveActorAbilities();
	}
}

void ASLHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASLHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

TObjectPtr<USLAbilitySystemComponent> ASLHero::GetSLAbilitySystemComponent() const
{
	return SLAbilitySystemComponent;
}
