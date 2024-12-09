// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABSpawnGimmick.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"

// Sets default values
AABSpawnGimmick::AABSpawnGimmick()
{
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(2000.0f, 2000.0f, 300.0f));
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSpawnGimmick::OnStageTriggerBeginOverlap);

	OpponentClass = AABCharacterNonPlayer::StaticClass();
}

void AABSpawnGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AABCharacterPlayer* PlayerCharacter = Cast<AABCharacterPlayer>(OtherActor);
	if (!PlayerCharacter)
	{
		return;
	}

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	OnOpponentSpawn(PlayerCharacter); 
}

void AABSpawnGimmick::OnOpponentSpawn(AABCharacterPlayer* Player)
{
	FVector CenterLocation = GetActorLocation();
	CenterLocation.Z = 128.0f;

	int32 PlayerLevel = Player->GetLevel();

	int32 MinLevel = FMath::Max(1, PlayerLevel - 2);
	int32 MaxLevel = PlayerLevel + 2;

	int32 NumOpponents = FMath::RandRange(3, 6);

	for (int32 i = 0; i < NumOpponents; ++i)
	{
		// 트리거 중심을 기준으로 랜덤 위치 생성
		float RandomX = FMath::RandRange(-1000.0f, 1000.0f); // X축 랜덤 위치
		float RandomY = FMath::RandRange(-1000.0f, 1000.0f); // Y축 랜덤 위치

		FVector SpawnLocation = CenterLocation + FVector(RandomX, RandomY, 0.0f);

		int32 RandomLevel = FMath::RandRange(MinLevel, MaxLevel);

		// 적 스폰
		const FTransform SpawnTransform(SpawnLocation);
		AABCharacterNonPlayer* ABOpponentCharacter = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(OpponentClass, SpawnTransform);
		if (ABOpponentCharacter)
		{
			ABOpponentCharacter->SetLevel(RandomLevel);
			ABOpponentCharacter->FinishSpawning(SpawnTransform);
		}
	}
}




