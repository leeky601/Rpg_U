// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WarriorCharacterBase.h"

// Sets default values
AWarriorCharacterBase::AWarriorCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

}



