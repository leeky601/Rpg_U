// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterSkillWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

void UABCharacterSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillCooldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbQSkill")));
	ensure(SkillCooldownBar);

}

void UABCharacterSkillWidget::UpdateCooldown(float CooldownPercent)
{
	if (SkillCooldownBar)
	{
		SkillCooldownBar->SetPercent(CooldownPercent);
	}

}

