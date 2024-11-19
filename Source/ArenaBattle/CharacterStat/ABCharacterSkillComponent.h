// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterSkillComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCooldownUpdateDelegate, float /*Cooldown*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterSkillComponent();

	FOnCooldownUpdateDelegate OnCooldownUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<class UABSkillDataAsset> QSkillData;
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ActivateSkill();

	FORCEINLINE float GetCooldownPercent() const { return CooldownPercent; }


private:
	void StartCooldown();
	void UpdateCooldown();

	FTimerHandle CooldownTimerHandle;
	float CurrentCooldownTime = 0.0f;

	float CooldownPercent;
};
