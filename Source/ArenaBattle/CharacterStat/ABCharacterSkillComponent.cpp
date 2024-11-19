// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterSkillComponent.h"
#include "CharacterSkill/ABSkillDataAsset.h"

// Sets default values for this component's properties
UABCharacterSkillComponent::UABCharacterSkillComponent()
{
    static ConstructorHelpers::FObjectFinder<UABSkillDataAsset> QSkillDataRef(TEXT("/Script/ArenaBattle.ABSkillDataAsset'/Game/ArenaBattle/Skills/ABS_Skill1.ABS_Skill1'"));
    if (QSkillDataRef.Object)
    {
        QSkillData = QSkillDataRef.Object;
    }

	CooldownPercent = 0.0f;
}

void UABCharacterSkillComponent::ActivateSkill()
{
	if (QSkillData && CurrentCooldownTime <= 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Skill Activated!"));

		StartCooldown();
	}
}

void UABCharacterSkillComponent::StartCooldown()
{
	if (QSkillData)
	{
		CurrentCooldownTime = QSkillData->CoolDown;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UABCharacterSkillComponent::UpdateCooldown, 0.1f, true);
	}
}

void UABCharacterSkillComponent::UpdateCooldown()
{
	CurrentCooldownTime -= 0.1f;

	CooldownPercent = FMath::Clamp(1 - (CurrentCooldownTime / QSkillData->CoolDown), 0.0f, 1.0f);
	OnCooldownUpdate.Broadcast(CooldownPercent);

	if (CurrentCooldownTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}




