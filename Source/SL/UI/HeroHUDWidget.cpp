// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/UI/HeroHUDWidget.h"

#include "MVVMSubsystem.h"

void UHeroHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>();
	if (MVVMSubsystem->GetViewFromUserWidget(this) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MVVM View is STILL Null in NativeConstruct! Check WBP Bindings."));
	}
}
