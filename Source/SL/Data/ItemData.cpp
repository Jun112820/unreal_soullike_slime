// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Data/ItemData.h"

#include "Misc/DataValidation.h"

EDataValidationResult UItemData::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!ItemTag.IsValid())
	{
		Context.AddError(FText::FromString(TEXT("ItemTag 가 비어있습니다. 반드시 설정해야 합니다.")));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
