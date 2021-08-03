// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "ActionSystemComponent.h"
#include "GameplayTagContainer.h"
#include "CommonUtils.h"

void UAction::Initialize(UActionSystemComponent* ActionSystemComponent)
{
	OwnerActionSystem = ActionSystemComponent;
	Owner = ActionSystemComponent->GetOwner();
	condition2(Owner, OwnerActionSystem);
}

bool UAction::CanStart_Implementation(AActor* Instigator) const
{
	conditionb2(Owner, OwnerActionSystem);
	return (!OwnerActionSystem->IsBlockedWith(GetBlockTags())) && !bIsRunning;
}

void UAction::StartAction_Implementation(AActor* Instigator)
{
	condition2(Owner, OwnerActionSystem);
	bIsRunning = true;
	OwnerActionSystem->ActivateTags(ActiveTags);
}

void UAction::StopAction_Implementation(AActor* Instigator)
{
	condition2(Owner, OwnerActionSystem);
	bIsRunning = false;
	OwnerActionSystem->DeactivateTags(ActiveTags);
}

