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

bool UActionSystemComponent::IsBlockedWith(FGameplayTagContainer const& BlockTags) const
{
	for (auto const& Tag : BlockTags)
	{
		if (ActiveTags.Find(Tag.GetTagName()))
			return true;
	}
	return false;
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

bool UActionSystemComponent::StartActionByName(FName ActionName, AActor* Instigator = nullptr)
{
	conditionb(Actions.Contains(ActionName));
	UAction* Action = Actions[ActionName];
	if (!Action->CanStart(Instigator))
		return false;

	Action->StartAction(Instigator);
	return true;
}

bool UActionSystemComponent::StopActionByName(FName ActionName, AActor* Instigator = nullptr)
{
	conditionb(Actions.Contains(ActionName));
	UAction* Action = Actions[ActionName];
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



