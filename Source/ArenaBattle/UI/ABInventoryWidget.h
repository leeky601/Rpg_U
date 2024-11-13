// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ABItemData.h"
#include "ABInventoryWidget.generated.h"


/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:
    

protected:
    virtual void NativeConstruct() override;
 

public:
    void UpdateInventory(const TArray<TObjectPtr<UABItemData>>& Items);

protected:
    UFUNCTION()
    void OnItemClicked();

protected:
    UPROPERTY()
    TObjectPtr<class UUniformGridPanel> InventoryGrid;


    UPROPERTY()
    TMap<TObjectPtr<class UButton>, TObjectPtr<class UABItemData>> ButtonItemMap;
};

