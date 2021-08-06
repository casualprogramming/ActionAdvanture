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

	//Init Child
	for (auto& Child: ChildActionsClass)
	{
		auto const& Default = Child.ActionClass.GetDefaultObject();
		FName ChildActionName;
		if (Child.bInstancingWithParentName)
		{
			ChildActionName = FName(this->GetActionName().ToString() + "." + Default->GetActionName().ToString());
			ActionSystemComponent->RegisterAction(Child.ActionClass , nullptr, ChildActionName);
		}
		else
		{
			//soft register
			ChildActionName = Default->GetActionName();
			if(!ActionSystemComponent->IsRegisteredAction(ChildActionName))
				ActionSystemComponent->RegisterAction(Child.ActionClass, nullptr);
		}
		Child.Action = ActionSystemComponent->GetAction(ChildActionName);
	}
}

bool UAction::CanStart_Implementation(AActor* Instigator)
{
	conditionb2(Owner, OwnerActionSystem);
	if (OwnerActionSystem->IsBlockedWith(GetBlockTags()) || bIsRunning) return false;
	//When a child's CanStart affects the parent
	for (auto const& Child : ChildActionsClass)
	{
		if (Child.bCallCanStartAtParent)
			if (!Child.Action->CanStart(Instigator))
				return false;
	}
	return true;
}

void UAction::StartAction_Implementation(AActor* Instigator)
{
	condition2(Owner, OwnerActionSystem);
	bIsRunning = true;
	OwnerActionSystem->ActivateTags(ActiveTags);

	//Start Child
	for (auto const& Child : ChildActionsClass)
	{
		//already checked at parent's CanStart
		if (Child.bCallCanStartAtParent)
		{
			Child.Action->StartAction(Instigator);
		}
		//already checked yet
		else
		{
			if (Child.Action->CanStart(Instigator))
				Child.Action->StartAction(Instigator);
		}
	}
}

void UAction::StopAction_Implementation(AActor* Instigator)
{
	condition2(Owner, OwnerActionSystem);
	bIsRunning = false;
	OwnerActionSystem->DeactivateTags(ActiveTags);

	//Stop Child
	for (auto const& Child : ChildActionsClass)
		if(Child.Action->IsRunning() && Child.bSyncActionStopWithParent)
			Child.Action->StopAction(Instigator);
}

