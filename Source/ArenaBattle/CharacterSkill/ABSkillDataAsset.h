// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABSkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABSkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSoftObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSoftObjectPtr<UTexture2D> SkillTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSoftObjectPtr<UParticleSystem> SkillEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float SkillDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float CoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float SkillRange;
};
