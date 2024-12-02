// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterExpWidget.h"
#include "Components/ProgressBar.h"

UABCharacterExpWidget::UABCharacterExpWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	MaxExp = 200.0f;
}

void UABCharacterExpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ExpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbExpBar")));
	ensure(ExpProgressBar);

}

void UABCharacterExpWidget::UpdateExpBar(float NewExp)
{
	ExpProgressBar->SetPercent(NewExp / MaxExp);
}
