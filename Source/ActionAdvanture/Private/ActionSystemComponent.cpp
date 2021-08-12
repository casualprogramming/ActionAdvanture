// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionSystemComponent.h"
#include "Action.h"
#include "CommonUtils.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
UActionSystemComponent::UActionSystemComponent()
{

}

void UActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	ActiveTagCountMap.Empty();
	CancelActionByTagMap.Empty();
	for (TSubclassOf<UAction> ActionClass : DefaultActions)
	{
		RegisterAction(ActionClass, GetOwner());
	}
}

void UActionSystemComponent::RegisterAction(TSubclassOf<UAction> ActionClass, AActor* Instigator, FName ForcedActionName)
{
	condition(ActionClass);
	UAction* Action = NewObject<UAction>(GetOwner(), ActionClass);
	if(!(ForcedActionName==""))
		Action->SetActionName(ForcedActionName);
	conditionf(!Actions.Find(Action->GetActionName()), TEXT("AddAction Failed!: Action %s already exist in ActionSystemComponent"), *Action->GetActionName().ToString());
	Actions.Add(Action->GetActionName(),Action);
	Action->Initialize(this);
}

void UActionSystemComponent::StartActionByName(FName ActionName, AActor* Instigator = nullptr)
{
	conditionf(Actions.Contains(ActionName), TEXT("Action: \"%s\" is not in ActionSystemComponent"), *ActionName.ToString());
	UAction* Action = Actions[ActionName];
	Action->CommitStartAction(Instigator);
}

void UActionSystemComponent::StopActionByName(FName ActionName, AActor* Instigator = nullptr)
{
	conditionf(Actions.Contains(ActionName), TEXT("Action: \"%s\" is not in ActionSystemComponent"), *ActionName.ToString());
	UAction* Action = Actions[ActionName];
	Action->CommitStopAction(Instigator, false);
}

void UActionSystemComponent::CancelActionByName(FName ActionName, AActor* Instigator = nullptr)
{
	conditionf(Actions.Contains(ActionName), TEXT("%s is not in ActionSystemComponent"), *ActionName.ToString());
	UAction* Action = Actions[ActionName];
	Action->CommitStopAction(Instigator, true);
}

void UActionSystemComponent::ActivateTags(FGameplayTagContainer const& Tags)
{
	for (auto const& Tag : Tags.GetGameplayTagParents())
	{
		if (ActiveTagCountMap.Find(Tag.GetTagName()))
			ActiveTagCountMap[Tag.GetTagName()]++;
		else
			ActiveTagCountMap.Add(Tag.GetTagName(),1);
	}
}

void UActionSystemComponent::DeactivateTags(FGameplayTagContainer const& Tags)
{
	for (auto const& Tag : Tags.GetGameplayTagParents())
	{
		if (ActiveTagCountMap.Find(Tag.GetTagName()))
		{
			uint32 Count = --ActiveTagCountMap[Tag.GetTagName()];
			if (Count == 0) ActiveTagCountMap.Remove(Tag.GetTagName());
		}
	}
}

bool UActionSystemComponent::HasAny(FGameplayTagContainer const& Tags) const
{
	for (auto const& Tag : Tags)
	{
		if (ActiveTagCountMap.Find(Tag.GetTagName()))
			return true;
	}
	return false;
}

bool UActionSystemComponent::HasAll(FGameplayTagContainer const& Tags) const
{
	for (auto const& Tag : Tags)
	{
		if (!ActiveTagCountMap.Find(Tag.GetTagName()))
			return false;
	}
	return true;
}

bool UActionSystemComponent::SatisfyTagRequirements(const UAction* Action) const
{
	return !HasAny(Action->GetBlockTags()) && HasAll(Action->GetRequiredTags());
}

void UActionSystemComponent::CancelAllAction(AActor* Instigator)
{
	for (auto& Action: Actions)
	{
		Action.Value->CommitStopAction(Instigator, true);
	}
}

void UActionSystemComponent::CancelActionByTags(FGameplayTagContainer const& Tags, AActor* Instigator)
{
	for (auto const& Tag : Tags)
	{
		FActionArray* ActionArray = CancelActionByTagMap.Find(Tag.GetTagName());
		if (ActionArray)
		{
			TArray<UAction*> CancelActions = ActionArray->Actions;// copy it because the Actions->CommitStopAction remove the array element, prevent crash
			for (UAction* Action : CancelActions)
			{
				if (IsValid(Action))
					Action->CommitStopAction(Instigator, true);
			}
		}
	}
}

//As the number of parents of the CancelTag increases, there can be an overhead. Consider tree-based structure.
void UActionSystemComponent::AddCancelTagsListener(FGameplayTagContainer const& CancelTags, UAction* Listener)
{
	for (auto const& Tag : CancelTags.GetGameplayTagParents())
	{
		FActionArray& ActionArray = CancelActionByTagMap.FindOrAdd(Tag.GetTagName());
		ActionArray.Actions.Emplace(Listener);
	}
}

void UActionSystemComponent::DeleteCancelTagsListener(FGameplayTagContainer const& CancelTags, UAction* Listener)
{
	for (auto const& Tag : CancelTags.GetGameplayTagParents())
	{
		FActionArray* ActionArray = CancelActionByTagMap.Find(Tag.GetTagName());
		//Not finding the array is not what I expected. The Action registers with a cancel tag to Array when it starts and removes it when it stops. Always registered once and removed once, never removed again.
		conditionf(ActionArray, TEXT("Tag \"%s\" is not found. The function \"DeleteCancelTagsListener\" is called when the action is stopped(or cancel). Do not call manually."), *Tag.GetTagName().ToString());
		conditionf(ActionArray->Actions.Contains(Listener), TEXT("Tag \"%s\" Listener is not found. The function \"DeleteCancelTagsListener\" is called when the action is stopped(or cancel). Do not call manually."), *Tag.GetTagName().ToString());
		//TODO: Find it backwards and delete it, Most likely it was started a few seconds before stopping and is stored at the end of the array. or change to double list and delete with list address O(1).
		ActionArray->Actions.Remove(Listener);
		if (ActionArray->Actions.Num() == 0)
			CancelActionByTagMap.Remove(Tag.GetTagName());
	}
}

void UActionSystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CancelAllAction(GetOwner());
	Super::EndPlay(EndPlayReason);
}
