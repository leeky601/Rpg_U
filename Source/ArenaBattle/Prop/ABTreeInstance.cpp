// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABTreeInstance.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AABTreeInstance::AABTreeInstance()
{
    // Instanced Static Mesh Components 배열 생성
    for (int32 i = 0; i < 3; i++)
    {
        FString ComponentName = FString::Printf(TEXT("InstancedMesh%d"), i);
        UInstancedStaticMeshComponent* NewComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*ComponentName);
        NewComponent->SetupAttachment(RootComponent);
        TreeMeshComponents.Add(NewComponent); // 배열에 추가
    }


    // Static Mesh 로드
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TreeMesh1(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Meshes/SM_Tree_1.SM_Tree_1'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TreeMesh2(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Meshes/SM_Tree_2.SM_Tree_2'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TreeMesh3(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Meshes/SM_Tree_3.SM_Tree_3'"));

    if (TreeMesh1.Succeeded()) TreeMeshComponents[0]->SetStaticMesh(TreeMesh1.Object);
    if (TreeMesh2.Succeeded()) TreeMeshComponents[1]->SetStaticMesh(TreeMesh2.Object);
    if (TreeMesh3.Succeeded()) TreeMeshComponents[2]->SetStaticMesh(TreeMesh3.Object);

    FVector BaseLocation = FVector(0.0f, 0.0f, 0.0f);
    FVector Scale(4.0f);

    // 랜드스케이프 크기 및 오프셋 범위 설정
    float LandscapeSize = 50000.0f; // 랜드스케이프의 가로/세로 크기
    float BorderBuffer = 2000.0f;  // 가장자리로부터 거리(버퍼)
    float RandomRange = (LandscapeSize / 2) - BorderBuffer;

    // 나무 인스턴스 추가
    for (int32 i = 0; i < 200; i++) // 100개의 나무 생성
    {
        // 랜덤 메시 컴포넌트 선택
        int32 RandomIndex = FMath::RandRange(0, TreeMeshComponents.Num() - 1);

        // 랜덤 위치 설정 (가장자리 벗어나지 않도록 범위 제한)
        float XOffset = FMath::RandRange(-RandomRange, RandomRange);
        float YOffset = FMath::RandRange(-RandomRange, RandomRange);
        FVector Offset = FVector(XOffset, YOffset, 0.0f);

        // 랜덤 회전 및 위치 변환
        FTransform InstanceTransform(FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f), BaseLocation + Offset, Scale);

        // 인스턴스 추가
        TreeMeshComponents[RandomIndex]->AddInstance(InstanceTransform);
    }
}




