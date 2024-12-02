// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABTreeInstance.generated.h"

UCLASS()
class ARENABATTLE_API AABTreeInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABTreeInstance();

protected:
	UPROPERTY()
	TArray<UInstancedStaticMeshComponent*> TreeMeshComponents;
};
