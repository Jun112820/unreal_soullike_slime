#pragma once
#include "IDamageable.generated.h"

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface { GENERATED_BODY() };

class SL_API IDamageable
{
	GENERATED_BODY()
	
public:
	virtual void HandleTakeDamage(AActor* Attacker) {}
};