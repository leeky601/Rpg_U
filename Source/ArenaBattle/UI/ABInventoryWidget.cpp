// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABInventoryWidget.h"
#include "Interface/ABInventoryWidgetInterface.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Components/SizeBox.h"
#include "Interface/ABControllerUIInterface.h"
#include <Components/CanvasPanelSlot.h>

void UABInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    for (int32 i = 1; i <= 30; ++i)
    {
        FString ButtonName = FString::Printf(TEXT("Button%d"), i);
        TObjectPtr<UButton> Button = Cast<UButton>(GetWidgetFromName(*ButtonName));
        if (Button)
        {
            ButtonItemMap.Add(Button, nullptr);  
            Button->OnClicked.AddDynamic(this, &UABInventoryWidget::OnItemClicked);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Button with name %s not found"), *ButtonName);
        }
    }

    if (UButton* CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton"))))
    {
        CloseButton->OnClicked.AddDynamic(this, &UABInventoryWidget::OnCloseButtonClicked);
    }


    IABInventoryWidgetInterface* InvenPawn = Cast<IABInventoryWidgetInterface>(GetOwningPlayerPawn());
    if (InvenPawn)
    {
        InvenPawn->SetupInvenWidget(this);
    }
}

void UABInventoryWidget::OnItemClicked()
{
    for (const auto& Pair : ButtonItemMap)
    {
        TObjectPtr<UButton> Button = Pair.Key;
        if (Button->IsHovered())
        {
            TObjectPtr<UABItemData> ClickedItem = Pair.Value;

            IABInventoryWidgetInterface* InventoryPawn = Cast<IABInventoryWidgetInterface>(GetOwningPlayerPawn());
            if (InventoryPawn && ClickedItem)
            {
                InventoryPawn->TakeItem(ClickedItem);
            }
            break;
        }
    }
}

void UABInventoryWidget::OnCloseButtonClicked()
{
    IABControllerUIInterface* PawnControler = Cast<IABControllerUIInterface>(GetOwningPlayer());
    if (PawnControler)
    {
        PawnControler->ToggleInventory();
    }
}

void UABInventoryWidget::UpdateInventory(const TArray<TObjectPtr<UABItemData>>& Items)
{
    int32 ItemIndex = 0;

    for (auto& Pair : ButtonItemMap)
    {
        TObjectPtr<UButton> Button = Pair.Key;

        if (ItemIndex < Items.Num())
        {
            TObjectPtr<UABItemData> ItemData = Items[ItemIndex];
            if (ItemData)
            {
                if (ItemData->ItemTexture.IsPending())
                {
                    ItemData->ItemTexture.LoadSynchronous();
                }

                if (ItemData->ItemTexture.IsValid() && Pair.Value != ItemData)
                {
                    Button->ClearChildren();
                    UTexture2D* LoadedTexture = ItemData->ItemTexture.Get();


                    USizeBox* ItemSizeBox = NewObject<USizeBox>(Button);
                    ItemSizeBox->SetWidthOverride(48.0f);
                    ItemSizeBox->SetHeightOverride(48.0f);
                    Button->AddChild(ItemSizeBox);


                    UImage* ItemImage = NewObject<UImage>(ItemSizeBox);
                    ItemImage->SetBrushFromTexture(LoadedTexture);
                    ItemImage->SetBrushSize(FVector2D(48.0f, 48.0f));
                    ItemSizeBox->AddChild(ItemImage);

                    Pair.Value = ItemData;
                }
            }
            ++ItemIndex;
        }
        else
        {
            Button->ClearChildren();
            Pair.Value = nullptr;
        }
    }
}



