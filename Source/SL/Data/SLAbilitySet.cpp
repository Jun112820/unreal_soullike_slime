// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Data/SLAbilitySet.h"

#include "Misc/DataValidation.h"

#if WITH_EDITOR
inline EDataValidationResult USLAbilitySet::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!CharacterTag.IsValid())
	{
		Context.AddError(FText::FromString(TEXT("CharacterTag가 비어있습니다. 반드시 설정해야 합니다.")));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif