// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "ActionSystemComponent.h"
#include "CommonUtils.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
UActionSystemComponent::UActionSystemComponent()
{

}

void UActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
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
	for (auto const& Tag : Tags)
		ActiveTags.Add(Tag.GetTagName());
}

void UActionSystemComponent::DeactivateTags(FGameplayTagContainer const& Tags)
{
	for (auto const& Tag : Tags)
		ActiveTags.Remove(Tag.GetTagName());
}

bool UActionSystemComponent::HasAny(FGameplayTagContainer const& Tags) const
{
	for (auto const& Tag : Tags)
	{
		if (ActiveTags.Find(Tag.GetTagName()))
			return true;
	}
	return false;
}

bool UActionSystemComponent::HasAll(FGameplayTagContainer const& Tags) const
{
	for (auto const& Tag : Tags)
	{
		if (!ActiveTags.Find(Tag.GetTagName()))
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



