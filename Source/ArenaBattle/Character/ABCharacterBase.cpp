// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"
#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "CharacterStat/ABCharacterInvenComponent.h"
#include "CharacterStat/ABCharacterSkillComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABHpBarWidget.h"
#include "UI/ABInventoryWidget.h"
#include "UI/ABCharacterSkillWidget.h"
#include "Item/ABItems.h"
#include "CharacterSkill/ABSkillDataAsset.h"
#include <Kismet/GameplayStatics.h>
#include "Particles/ParticleSystem.h"

DEFINE_LOG_CATEGORY(LogABCharacter);

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	//Pawn
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    //Cpasule
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

    //Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Robo.SK_CharM_Robo'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder"));
    if (ShoulderDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater"));
    if (QuaterDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAction.AM_ComboAction'"));
    if (ComboActionMontageRef.Object)
    {
        ComboActionMontage = ComboActionMontageRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterComboAction/ABA_ComboAction.ABA_ComboAction'"));
    if (ComboActionDataRef.Object)
    {
        ComboActionData = ComboActionDataRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_Dead.AM_Dead'"));
    if (DeadMontageRef.Object)
    {
        DeadMontage = DeadMontageRef.Object;
    }


    // Stat Component 
    Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));
    Inventory = CreateDefaultSubobject<UABCharacterInvenComponent>(TEXT("Inventory"));
    Skill = CreateDefaultSubobject<UABCharacterSkillComponent>(TEXT("Skill"));

    // Widget Component 
    HpBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
    HpBar->SetupAttachment(GetMesh());
    HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
    static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
    if (HpBarWidgetRef.Class)
    {
        HpBar->SetWidgetClass(HpBarWidgetRef.Class);
        HpBar->SetWidgetSpace(EWidgetSpace::Screen);
        HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
        HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // Item Actions
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::EquipWeapon)));
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::DrinkPotion)));
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::ReadScroll)));

    // Weapon Component
    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
    Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AABCharacterBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    Stat->OnHpZero.AddUObject(this, &AABCharacterBase::SetDead);
    Stat->OnStatChanged.AddUObject(this, &AABCharacterBase::ApplyStat);
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
    // Pawn
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    // CharacterMovement
    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AABCharacterBase::ProcessComboCommand()
{
    if (CurrentCombo == 0)
    {
        ComboActionBegin();
        return;
    }

    if (!ComboTimerHandle.IsValid())
    {
        HasNextComboCommand = false;
    }
    else
    {
        HasNextComboCommand = true;
    }
}

void AABCharacterBase::ComboActionBegin()
{
    CurrentCombo = 1;

    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    ensure(CurrentCombo != 0);
    CurrentCombo = 0;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    NotifyComboActionEnd();
}

void AABCharacterBase::NotifyComboActionEnd()
{
}

void AABCharacterBase::SetComboCheckTimer()
{
    int32 ComboIndex = CurrentCombo - 1;
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
    if (ComboEffectiveTime > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
    }
}

void AABCharacterBase::ComboCheck()
{
    ComboTimerHandle.Invalidate();
    if (HasNextComboCommand)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

        CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
        AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
        SetComboCheckTimer();
        HasNextComboCommand = false;
    }
}

void AABCharacterBase::ExcuteHitCheck()
{
    if (IsSkill)
    {
        SkillEffect();
        SkillHitcheck();
    }
    else
    {
        AttackHitCheck();
    }
}

void AABCharacterBase::HitCheck(float InAttackRange, float InAttackDamage)
{
    FHitResult OutHitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

    const float AttackRange = InAttackRange;
    const float AttackRadius = Stat->GetAttackRadius();
    const float AttackDamage = InAttackDamage;
    const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
    const FVector End = Start + GetActorForwardVector() * AttackRange;

    bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
    if (HitDetected)
    {
        FDamageEvent DamageEvent;
        OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
    }

#if ENABLE_DRAW_DEBUG
    FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
    float CapsuleHalfHeight = AttackRange * 0.5f;
    FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

    DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

void AABCharacterBase::AttackHitCheck()
{
    const float AttackRange = Stat->GetTotalStat().AttackRange;
    const float AttackDamage = Stat->GetTotalStat().Attack;

    HitCheck(AttackRange, AttackDamage);
}

void AABCharacterBase::SkillHitcheck()
{
    const float AttackRange = Stat->GetTotalStat().AttackRange + Skill->QSkillData->SkillRange;
    const float AttackDamage = Stat->GetTotalStat().Attack + Skill->QSkillData->SkillDamage;

    HitCheck(AttackRange, AttackDamage);
}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Stat->ApplyDamage(DamageAmount);

    return DamageAmount;
}

void AABCharacterBase::SetDead()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    PlayDeadAnimation();
    SetActorEnableCollision(false);
    HpBar->SetHiddenInGame(true);
}

void AABCharacterBase::PlayDeadAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->StopAllMontages(0.0f);
    AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AABCharacterBase::SetupCharacterWidget(UABUserWidget* InUserWidget)
{
    UABHpBarWidget* HpBarWidget = Cast<UABHpBarWidget>(InUserWidget);
    if (HpBarWidget)
    {
        HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
        HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
        HpBarWidget->UpdateLevel(GetLevel());
        Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
        Stat->OnStatChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateStat);
        Stat->OnLevelChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateLevel);
    }
}



void AABCharacterBase::SetupInvenWidget(UABInventoryWidget* InInventoryWidget)
{
    UABInventoryWidget* InventoryWidget = InInventoryWidget;
    if (InventoryWidget)
    {
        InventoryWidget->UpdateInventory(Inventory->GetItems());
        Inventory->OnInventoryUpdated.AddUObject(InventoryWidget, &UABInventoryWidget::UpdateInventory);
    }
}


void AABCharacterBase::PickedItem(UABItemData* InItemData)
{
    if (InItemData)
    {
        Inventory->AddItem(InItemData);
    }
}

void AABCharacterBase::TakeItem(UABItemData* InItemData)
{
    if (InItemData)
    {
        Inventory->RemoveItem(InItemData);
        TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
    }
}

void AABCharacterBase::DrinkPotion(UABItemData* InItemData)
{
    UABPotionItemData* PotionItemData = Cast<UABPotionItemData>(InItemData);
    if (PotionItemData)
    {
        Stat->HealHp(PotionItemData->HealAmount);
    }
}

void AABCharacterBase::EquipWeapon(UABItemData* InItemData)
{
    UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);
    if (WeaponItemData)
    {
        if (WeaponItemData->WeaponMesh.IsPending())
        {
            WeaponItemData->WeaponMesh.LoadSynchronous();
        }
        Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
        Stat->SetModifierStat(WeaponItemData->ModifierStat);
    }
}

void AABCharacterBase::ReadScroll(UABItemData* InItemData)
{
    UABScrollItemData* ScrollItemData = Cast<UABScrollItemData>(InItemData);
    if (ScrollItemData)
    {
        Stat->AddBaseStat(ScrollItemData->BaseStat);
    }
}

int32 AABCharacterBase::GetLevel()
{
    return Stat->GetCurrentLevel();
}

void AABCharacterBase::SetLevel(int32 InNewLevel)
{
    Stat->SetLevelStat(InNewLevel);
}

void AABCharacterBase::ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
    float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AABCharacterBase::UseSkill(UABSkillDataAsset* InSkill)
{
    Skill->ActivateSkill();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (InSkill)
    {
        if (InSkill->SkillMontage.IsPending())
        {
            InSkill->SkillMontage.LoadSynchronous();
        }  
        AnimInstance->Montage_Play(InSkill->SkillMontage.Get(), AttackSpeedRate);
    }

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AABCharacterBase::SkillEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, InSkill->SkillMontage.Get());
}

void AABCharacterBase::SkillEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    IsSkill = false;
}

void AABCharacterBase::SkillEffect()
{
    if (Skill->QSkillData)
    {
        if (Skill->QSkillData->SkillEffect.IsPending())
        {
            Skill->QSkillData->SkillEffect.LoadSynchronous();
        }
    }
    float EffectDistance = 200.0f;
    FVector EffectLocation = GetActorLocation() + (GetActorForwardVector() * EffectDistance);
    FRotator EffectRotation = GetActorRotation();
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill->QSkillData->SkillEffect.Get(), EffectLocation, EffectRotation);
}




