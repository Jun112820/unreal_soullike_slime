#include "SLPlayerState.h"
#include "AbilitySystemComponent.h"
#include "SL/Abilities/SLAbilitySystemComponent.h"

ASLPlayerState::ASLPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USLAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void ASLPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

UAbilitySystemComponent* ASLPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
