// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "CooldownAction.h"
#include "ActionSystemComponent.h"
#include "GameplayTagContainer.h"
#include "CommonUtils.h"

void UAction::Initialize_Implementation(UActionSystemComponent* ActionSystemComponent)
{
	OwnerActionSystem = ActionSystemComponent;
	Owner = ActionSystemComponent->GetOwner();
	condition2(Owner, OwnerActionSystem);

	//Init Child
	for (auto& Child: ChildActionsClass)
	{
		condition(Child.ActionClass);
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

void UAction::CommitStartAction(AActor* Instigator)
{
	if (CanStart(Instigator))
		StartAction(Instigator);
}

void UAction::CommitStopAction(AActor* Instigator, bool bCancel)
{
	UE_LOG(LogTemp, Log, TEXT("CommitStopAction %s."), *GetActionName().ToString());
	if (bIsRunning)
		StopAction(Instigator, bCancel);
}

UWorld* UAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}


bool UAction::CanStart_Implementation(AActor* Instigator)
{
	conditionb2(Owner, OwnerActionSystem);
	conditionbf(!CanceledTags.HasAny(ActiveCancelTags),TEXT("\"%s\" Self Canceled(Tags) by ActiveCancelTags found"), *GetActionName().ToString());//self cancel
	if (bIsRunning|| !OwnerActionSystem->SatisfyTagRequirements(this)) return false;
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
	OwnerActionSystem->AddCancelTagsListener(CanceledTags, this);
	OwnerActionSystem->CancelActionByTags(ActiveCancelTags, GetOwner());
	//Start Child
	for (auto const& Child : ChildActionsClass)
	{
		if(Child.bSyncActionStartWithParent)
		{
			//already checked at parent's CanStart
			if (Child.bCallCanStartAtParent)
			{
				Child.Action->StartAction(Instigator);
			}
			else
			{
				if (Child.Action->CanStart(Instigator))
					Child.Action->StartAction(Instigator);
			}
		}
	}
}

void UAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	condition2(Owner, OwnerActionSystem);
	bIsRunning = false;
	OwnerActionSystem->DeactivateTags(ActiveTags);
	OwnerActionSystem->DeleteCancelTagsListener(CanceledTags, this);

	//Stop Child
	for (auto const& Child : ChildActionsClass)
		if(Child.bSyncActionStopWithParent)
			Child.Action->CommitStopAction(Instigator, bCancel);
}

UCooldownAction* UAction::GetorAddChildCooldownHelperAction()
{
	if (!ChildCooldownAction)
	{
		FName ChildCooldownActionName = FName(this->GetActionName().ToString() + "." + "CooldownHelper");

		OwnerActionSystem->RegisterAction(UCooldownAction::StaticClass(), nullptr, ChildCooldownActionName);
		ChildCooldownAction = Cast<UCooldownAction>(OwnerActionSystem->GetAction(ChildCooldownActionName));
		FChildActionDesc Desc;
		Desc.Action = ChildCooldownAction;
		Desc.bCallCanStartAtParent = true;//prevent parent's start during cooltime
		Desc.bSyncActionStartWithParent = false;
		Desc.bSyncActionStopWithParent = false;
		ChildActionsClass.Add(Desc);
	}
	return ChildCooldownAction;
}
void UAction::AddCooldown(float CooldownDuration, bool bActorTimer)
{
	GetorAddChildCooldownHelperAction()->AddCooldownDuration(CooldownDuration, bActorTimer);
}

void UAction::StartCooldown(float CooldownDuration, bool bActorTimer)
{
	GetorAddChildCooldownHelperAction()->StartCooldownDuration(CooldownDuration, bActorTimer);
}