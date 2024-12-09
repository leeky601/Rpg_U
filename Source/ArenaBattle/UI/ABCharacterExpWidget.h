// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterExpWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterExpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UABCharacterExpWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateExpBar(float NewExp, float MaxEXP);

protected:
	UPROPERTY()
	float CurrentExp;

	UPROPERTY()
	TObjectPtr<class UProgressBar> ExpProgressBar;
	
};
