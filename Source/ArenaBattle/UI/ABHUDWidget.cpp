// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"
#include "Interface/ABCharacterHUDInterface.h"
#include "ABHpBarWidget.h"
#include "ABCharacterStatWidget.h"
#include "ABCharacterSkillWidget.h"
#include "ABCharacterExpWidget.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UABHUDWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	FABCharacterStat TotalStat = BaseStat + ModifierStat;

	HpBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UABHUDWidget::UpdateLevel(float NewLevel)
{
	HpBar->UpdateLevel(NewLevel);
}

void UABHUDWidget::UpdateExp(float NewExp)
{
	CharacterExp->UpdateExpBar(NewExp);
}

void UABHUDWidget::UpdateSkill(float CooldownPercent)
{
	CharacterSkill->UpdateCooldown(CooldownPercent);
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UABCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	CharacterSkill = Cast<UABCharacterSkillWidget>(GetWidgetFromName(TEXT("WidgetCharacterSkill")));
	ensure(CharacterSkill);

	CharacterExp = Cast<UABCharacterExpWidget>(GetWidgetFromName(TEXT("WidgetCharacterExp")));
	ensure(CharacterExp);

	IABCharacterHUDInterface* HUDPawn = Cast<IABCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
