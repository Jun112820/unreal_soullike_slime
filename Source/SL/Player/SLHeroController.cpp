// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Player/SLHeroController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/AssetManager.h"
#include "GameFramework/Character.h"
#include "SL/Abilities/SLAbilitySystemComponent.h"
#include "SL/Character/SLHero.h"
#include "SL/Data/SLInputSet.h"

ASLHeroController::ASLHeroController()
{
}

void ASLHeroController::BeginPlay()
{
	Super::BeginPlay();
}

void ASLHeroController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!IsLocalPlayerController()) return;
	UAssetManager& AssetManager = UAssetManager::Get();
	FGameplayTag CharacterTag = FGameplayTag::RequestGameplayTag(FName("Character.Hero")); 
	FPrimaryAssetId AssetId("InputSet", CharacterTag.GetTagLeafName());

	AssetManager.LoadPrimaryAsset(AssetId);
	HeroInputSet = Cast<USLInputSet>(AssetManager.GetPrimaryAssetObject(AssetId));

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    if (Subsystem)
    {
        if (HeroInputSet)
        {
            for (const TObjectPtr<UInputMappingContext>& IMC : HeroInputSet->AdditionalIMCs)
            {
                if (IMC) Subsystem->AddMappingContext(IMC, 1);
            }
        }
    }

    if (EnhancedInputComponent)
    {
        if (HeroInputSet)
        {
        	for (const FSLInputAction& Action : HeroInputSet->NativeInputActions)
        	{
        		if (Action.InputAction && Action.InputTag.IsValid())
        		{
        			const FGameplayTag& Tag = Action.InputTag;

        			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input.Native.Move"))))
        			{
        				EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Triggered, this, &ASLHeroController::Move);
        			}
        			else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input.Native.Look"))))
        			{
        				EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Triggered, this, &ASLHeroController::Look);
        			}
        		}
        	}
        	
            for (const FSLInputAction& Action : HeroInputSet->AbilityInputActions)
            {
                if (Action.InputAction && Action.InputTag.IsValid())
                {
                    EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Started, this, &ASLHeroController::InputAbilityTagPressed, Action.InputTag);
                    EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, this, &ASLHeroController::InputAbilityTagReleased, Action.InputTag);
                }
            }
        }
    }
}

void ASLHeroController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (APawn* MyPawn = GetPawn<APawn>())
	{
		const ASLHero* Hero = Cast<ASLHero>(MyPawn);
		if (USLAbilitySystemComponent* ASC = Hero->GetSLAbilitySystemComponent())
		{
			ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
		}
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ASLHeroController::InputAbilityTagPressed(const FInputActionValue& InputActionValue, FGameplayTag GameplayTag)
{
	if (APawn* MyPawn = GetPawn<APawn>())
	{
		const ASLHero* Hero = Cast<ASLHero>(MyPawn);
		if (USLAbilitySystemComponent* ASC = Hero->GetSLAbilitySystemComponent())
		{
			ASC->AbilityInputTagPressed(GameplayTag);
		}
	}
}

void ASLHeroController::InputAbilityTagReleased(const FInputActionValue& InputActionValue, FGameplayTag GameplayTag)
{
	if (APawn* MyPawn = GetPawn<APawn>())
	{
		const ASLHero* Hero = Cast<ASLHero>(MyPawn);
		if (USLAbilitySystemComponent* ASC = Hero->GetSLAbilitySystemComponent())
		{
			ASC->AbilityInputTagPressed(GameplayTag);
		}
	}
}

void ASLHeroController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASLHeroController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		AddYawInput(LookAxisVector.X);
		AddPitchInput(LookAxisVector.Y);
	}
}