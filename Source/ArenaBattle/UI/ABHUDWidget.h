// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHUDWidget.generated.h"


/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UABHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	void UpdateLevel(float NewLevel);
	void UpdateExp(float NewExp, float MaxEXP);
	void UpdateSkill(float CooldownPercent);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UABHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UABCharacterStatWidget> CharacterStat;

	UPROPERTY()
	TObjectPtr<class UABCharacterSkillWidget> CharacterSkill;

	UPROPERTY()
	TObjectPtr<class UABCharacterExpWidget> CharacterExp;
};
