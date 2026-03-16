// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SLItem.generated.h"


class IAbilitySystemInterface;
class UItemData;

USTRUCT(BlueprintType)
struct FSLItemOverlapMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OverlappedActor = nullptr; // 들어온 액터 (Hero)

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ItemActor = nullptr; // 아이템 자신

	UPROPERTY(BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(BlueprintReadOnly)
	bool bIsOverlapped = false; // 들어오면 true, 나가면 false
};

UCLASS()
class SL_API ASLItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ASLItem();

public:
	UItemData* GetItemData();

protected:
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
protected:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void SendOverlapMessage(AActor* OtherActor, bool bIsOverlap);

protected:
	UPROPERTY(EditAnywhere, Category = "Item")
	FGameplayTag ItemTag;
	
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> ItemMesh;

	UPROPERTY()
	TObjectPtr<UItemData> ItemData;
};