// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttackAction.h"
#include "ActionSystemComponent.h"
#include "CommonUtils.h"
#include "DrawDebugHelpers.h"


UMeleeAttackAction::UMeleeAttackAction()
{
	ActionName = "MeleeAttackAction";
	EnableMeleeAttackCollisionEventTag = FGameplayTag::RequestGameplayTag(FName("ActionSystemEvent.MeleeAttack.EnableCollision"));
	DisableMeleeAttackCollisionEventTag = FGameplayTag::RequestGameplayTag(FName("ActionSystemEvent.MeleeAttack.DisableCollision"));
}


void UMeleeAttackAction::Initialize_Implementation(UActionSystemComponent* ActionSystemComponent)
{
	Super::Initialize_Implementation(ActionSystemComponent);
	TArray<UActorComponent*> Components = GetOwner()->GetComponentsByTag(UPrimitiveComponent::StaticClass(), MeleeColliderComponentTag);
	conditionf(Components.Num() == 1,TEXT("The UPrimitiveComponent variable \"MeleeAttackCollider\" is %s. Make sure %s have one UPrimitiveComponent with Tag %s."), Components.Num()? TEXT("duplicated"): TEXT("empty"), *GetOwner()->GetName(), *MeleeColliderComponentTag.ToString());
	MeleeAttackCollider = Cast<UPrimitiveComponent>(Components[0]);
	condition(MeleeAttackCollider);
	MeleeAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeAttackCollider->OnComponentBeginOverlap.AddDynamic(this, &UMeleeAttackAction::OnMeleeAttackOverlap);
}

bool UMeleeAttackAction::CanStart_Implementation(AActor* Instigator)
{
	if (!Super::CanStart_Implementation(Instigator)) return false;

	return true;
}


void UMeleeAttackAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	GetOwnerActionSystem()->GetEventDelegate(EnableMeleeAttackCollisionEventTag.GetTagName()).AddDynamic(this, &UMeleeAttackAction::OnEventEnableMeleeAttackCollision);
	GetOwnerActionSystem()->GetEventDelegate(DisableMeleeAttackCollisionEventTag.GetTagName()).AddDynamic(this, &UMeleeAttackAction::OnEventDisableMeleeAttackCollision);
}

void UMeleeAttackAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel);
	
	MeleeAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetOwnerActionSystem()->GetEventDelegate(EnableMeleeAttackCollisionEventTag.GetTagName()).RemoveDynamic(this, &UMeleeAttackAction::OnEventEnableMeleeAttackCollision);
	GetOwnerActionSystem()->GetEventDelegate(DisableMeleeAttackCollisionEventTag.GetTagName()).RemoveDynamic(this, &UMeleeAttackAction::OnEventDisableMeleeAttackCollision);
}

//Enable Event from Montage
void UMeleeAttackAction::OnEventEnableMeleeAttackCollision(AActor* Instigator)
{
	MeleeAttackCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

//Disable Event from Montage
void UMeleeAttackAction::OnEventDisableMeleeAttackCollision(AActor* Instigator)
{
	MeleeAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UMeleeAttackAction::OnMeleeAttackOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}