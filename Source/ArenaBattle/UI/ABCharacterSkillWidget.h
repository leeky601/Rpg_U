// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterSkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY()
	TObjectPtr<class UProgressBar> SkillCooldownBar;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UpdateCooldown(float CooldownPercent);


};
