// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "ActionSystemComponent.h"
#include "CommonUtils.h"

// Sets default values
UActionSystemComponent::UActionSystemComponent()
{

}

void UActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UActionSystemComponent::IsBlockedWith(FGameplayTagContainer const& BlockTags) const
{
	for (auto const& Tag : BlockTags)
	{
		if (ActiveTags.Find(Tag.GetTagName()))
			return true;
	}
	return false;
}

void UActionSystemComponent::AddAction(UAction* Action)
{
	condition(Action);
	Actions.Add(Action->GetUniqueID(),Action);
	Action->Initialize(this);
}

bool UActionSystemComponent::StartAction(UAction* Action, AActor* Instigator = nullptr)
{
	conditionb(Action);
	if (!Action->CanStart(Instigator))
		return false;

	Action->StartAction(Instigator);
	return true;
}

bool UActionSystemComponent::StopAction(UAction* Action, AActor* Instigator = nullptr)
{
	conditionb(Action);
	if (Action->IsRunning())//TODO: create UAction::CanStop()
	{
		Action->StopAction(Instigator);
		return true;
	}
	return false;
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



