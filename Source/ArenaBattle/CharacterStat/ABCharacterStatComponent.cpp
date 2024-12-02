// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"


// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;
	CurrentExp = 0;
	MaxExp = 200;

	bWantsInitializeComponent = true;
}

void UABCharacterStatComponent::InitializeComponent()
{
	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	SetHp(BaseStat.MaxHp);
	OnLevelChanged.Broadcast(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f);
}

void UABCharacterStatComponent::AddExp(float InExp)
{
	CurrentExp = FMath::Clamp(CurrentExp + InExp, 0, MaxExp);
	if (CurrentExp >= MaxExp)
	{
		SetLevelStat(CurrentLevel + 1);
		CurrentExp = 0;
	}
	OnExpChanged.Broadcast(CurrentExp);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}




