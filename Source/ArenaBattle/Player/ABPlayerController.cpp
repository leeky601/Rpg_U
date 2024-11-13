// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"
#include "UI/ABInventoryWidget.h"
#include <EnhancedInputComponent.h>

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInvenRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Inven.IA_Inven'"));
	if (nullptr != InputActionInvenRef.Object)
	{
		InvenAction = InputActionInvenRef.Object;
	}


	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UABInventoryWidget> ABInventoryWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_Inven.WBP_Inven_C"));
	if (ABInventoryWidgetRef.Class)
	{
		ABInventoryWidgetClass = ABInventoryWidgetRef.Class;
	}
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	if (ABHUDWidget)
	{
		ABHUDWidget->AddToViewport();
	}

	ABInventoryWidget = CreateWidget<UABInventoryWidget>(this, ABInventoryWidgetClass);
	if (ABInventoryWidget)
	{
		ABInventoryWidget->AddToViewport();
		ABInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(InvenAction, ETriggerEvent::Triggered, this, &AABPlayerController::ToggleInventory);
	}
}

void AABPlayerController::ToggleInventory()
{
	if (ABInventoryWidget)
	{
		bIsInventoryOpen = !bIsInventoryOpen;

		if (bIsInventoryOpen)
		{
			ABInventoryWidget->SetVisibility(ESlateVisibility::Visible);
			SetInputMode(FInputModeGameAndUI());
			bShowMouseCursor = true;              
		}
		else
		{
			ABInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			SetInputMode(FInputModeGameOnly());    
			bShowMouseCursor = false;              
		}
	}
}


