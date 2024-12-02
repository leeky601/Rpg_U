// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);


	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	Level = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLevel")));
	ensure(Level);

	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UABHpBarWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;

	if (HpProgressBar)
	{
		float HealthRatio = CurrentHp / MaxHp;
	
		FLinearColor FillColor;

		if (HealthRatio > 0.5f) 
		{
			FillColor = FLinearColor(1.0f - (HealthRatio - 0.5f) * 2.0f, 1.0f, 0.0f);
		}
		else 
		{
			
			FillColor = FLinearColor(1.0f, (HealthRatio * 2.0f), 0.0f);
		}
		
		HpProgressBar->SetPercent(HealthRatio);
		HpProgressBar->SetFillColorAndOpacity(FillColor); 
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}

	
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		// ü�� ���� ��� (0.0 ~ 1.0 ����)
		float HealthRatio = CurrentHp / MaxHp;

		// ���� ���
		FLinearColor FillColor;

		if (HealthRatio > 0.5f) // HP�� ���� �̻�
		{
			// ������� ���������
			FillColor = FLinearColor(1.0f - (HealthRatio - 0.5f) * 2.0f, 1.0f, 0.0f); // HealthRatio�� 1�� ��������� ���� �� �������
		}
		else // HP�� ���� ����
		{
			// ��������� ����������
			FillColor = FLinearColor(1.0f, (HealthRatio * 2.0f), 0.0f); // HealthRatio�� 0�� ��������� ���� �� ����������
		}

		// ���α׷��� ���� �ۼ�Ʈ �� ���� ����
		HpProgressBar->SetPercent(HealthRatio);
		HpProgressBar->SetFillColorAndOpacity(FillColor); // ���� ����
	}
	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UABHpBarWidget::UpdateLevel(float NewLevel)
{
	CurrentLevel = NewLevel;

	if (Level)
	{
		Level->SetText(FText::FromString(GetLevelText()));
	}
}

FString UABHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%.0f"), CurrentHp, MaxHp);
}

FString UABHpBarWidget::GetLevelText()
{
	return FString::Printf(TEXT("LV.%.0f"), CurrentLevel);
}
