// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABTreeInstance.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AABTreeInstance::AABTreeInstance()
{
    // Instanced Static Mesh Components �迭 ����
    for (int32 i = 0; i < 3; i++)
    {
        FString ComponentName = FString::Printf(TEXT("InstancedMesh%d"), i);
        UInstancedStaticMeshComponent* NewComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*ComponentName);
        NewComponent->SetupAttachment(RootComponent);
        TreeMeshComponents.Add(NewComponent); // �迭�� �߰�
    }


    // Static Mesh �ε�
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TreeMesh1(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Meshes/SM_Tree_1.SM_Tree_1'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TreeMesh2(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Meshes/SM_Tree_2.SM_Tree_2'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TreeMesh3(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Meshes/SM_Tree_3.SM_Tree_3'"));

    if (TreeMesh1.Succeeded()) TreeMeshComponents[0]->SetStaticMesh(TreeMesh1.Object);
    if (TreeMesh2.Succeeded()) TreeMeshComponents[1]->SetStaticMesh(TreeMesh2.Object);
    if (TreeMesh3.Succeeded()) TreeMeshComponents[2]->SetStaticMesh(TreeMesh3.Object);

    FVector BaseLocation = FVector(0.0f, 0.0f, 0.0f);
    FVector Scale(4.0f);

    // ���彺������ ũ�� �� ������ ���� ����
    float LandscapeSize = 50000.0f; // ���彺�������� ����/���� ũ��
    float BorderBuffer = 2000.0f;  // �����ڸ��κ��� �Ÿ�(����)
    float RandomRange = (LandscapeSize / 2) - BorderBuffer;

    // ���� �ν��Ͻ� �߰�
    for (int32 i = 0; i < 200; i++) // 100���� ���� ����
    {
        // ���� �޽� ������Ʈ ����
        int32 RandomIndex = FMath::RandRange(0, TreeMeshComponents.Num() - 1);

        // ���� ��ġ ���� (�����ڸ� ����� �ʵ��� ���� ����)
        float XOffset = FMath::RandRange(-RandomRange, RandomRange);
        float YOffset = FMath::RandRange(-RandomRange, RandomRange);
        FVector Offset = FVector(XOffset, YOffset, 0.0f);

        // ���� ȸ�� �� ��ġ ��ȯ
        FTransform InstanceTransform(FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f), BaseLocation + Offset, Scale);

        // �ν��Ͻ� �߰�
        TreeMeshComponents[RandomIndex]->AddInstance(InstanceTransform);
    }
}




