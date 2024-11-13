// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ABControllerUIInterface.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController, public IABControllerUIInterface
{
	GENERATED_BODY()
	
public:
	AABPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

public:
	virtual void ToggleInventory() override;

	
	// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidget;

	// Inventory Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InvenAction;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TSubclassOf<class UABInventoryWidget> ABInventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TObjectPtr<class UABInventoryWidget> ABInventoryWidget;

private:
	bool bIsInventoryOpen = false;

};
