// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterInvenComponent.h"

// Sets default values for this component's properties
UABCharacterInvenComponent::UABCharacterInvenComponent()
{
	
}

void UABCharacterInvenComponent::AddItem(UABItemData* NewItem)
{
    if (NewItem)
    {
        Items.Add(NewItem);
        OnInventoryUpdated.Broadcast(Items); 
    }
}

void UABCharacterInvenComponent::RemoveItem(UABItemData* ItemToRemove)
{
    if (Items.Remove(ItemToRemove) > 0)
    {
        OnInventoryUpdated.Broadcast(Items);
    }
}




