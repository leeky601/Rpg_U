// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterInvenComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<TObjectPtr<class UABItemData>>& /*InItem*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterInvenComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UABCharacterInvenComponent();

public:
    FOnInventoryUpdated OnInventoryUpdated;

    FORCEINLINE const TArray<TObjectPtr<UABItemData>>& GetItems() const { return Items; }

    void AddItem(UABItemData* InItem);
    void RemoveItem(UABItemData* ItemToRemove);


protected:
    UPROPERTY(Transient, VisibleInstanceOnly, Category = Inven, Meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<UABItemData>> Items;
    	
};
