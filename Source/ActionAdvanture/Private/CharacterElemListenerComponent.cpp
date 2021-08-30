// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterElemListenerComponent.h"
#include "CommonUtils.h"
#include "GameplayTagContainer.h"
#include "ActionSystemComponent.h"
#include <array>



UCharacterElemListenerComponent::UCharacterElemListenerComponent()
{
	DurationMap[EElementalChangeType::DRY] = nullptr;
	DurationMap[EElementalChangeType::EXTINGUISHED] = nullptr;
	DurationMap[EElementalChangeType::MELT] = nullptr;
	DurationMap[EElementalChangeType::BURN] = &BurningDuration;
	DurationMap[EElementalChangeType::WET] = &WetDuration;
	DurationMap[EElementalChangeType::FROZEN] = &FrozenDuration;
}

void UCharacterElemListenerComponent::BeginPlay()
{
	Super::BeginPlay();
	ActionSystemComp = GetOwner()->FindComponentByClass<UActionSystemComponent>();
	condition(ActionSystemComp);
}

void UCharacterElemListenerComponent::StateChangeBegin(EElementalStateType State, EElementalStateType NextState, EElementalChangeType ChangeType, AActor* Instigator)
{
	Super::StateChangeBegin(State, NextState,ChangeType,Instigator);
	UE_LOG(LogTemp, Log, TEXT("StateChangeBegin %s : %s"), *UEnum::GetValueAsString(ChangeType), *GetOwner()->GetName());
	//Clear
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	ActionSystemComp->DeactivateTags(CurrentElementStateTag.GetSingleTagContainer());

	//Update Tags
	CurrentElementStateTag = FGameplayTag::RequestGameplayTag(GetTagName(NextState));
	ActionSystemComp->ActivateTags(CurrentElementStateTag.GetSingleTagContainer());
	ActionSystemComp->CancelActionByTags(CurrentElementStateTag.GetSingleTagContainer());

	//Self Timer
	EElementalStateType OppositeElement = ElementsToNoneState(ChangeType);
	float Duration = DurationMap[ChangeType]? *DurationMap[ChangeType]:0.0f;

	FTimerDelegate Delegate;
	Delegate.BindWeakLambda(this, [=]()
		{
			RecieveElement(OppositeElement, GetOwner());
		}
	);
	GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
}
//TODO: change to an array instead of a switch case (Define rules in ElementalListenerComponent.cpp)
EElementalStateType UCharacterElemListenerComponent::ElementsToNoneState(EElementalChangeType ChangeType)
{
	switch (ChangeType)
	{
	case EElementalChangeType::BURN:
		return EElementalStateType::Water;
	case EElementalChangeType::WET:
		return EElementalStateType::Fire;
	case EElementalChangeType::FROZEN:
		return EElementalStateType::Fire;
	default:
		return EElementalStateType::None;
	}
}

//TODO: change to an array instead of a switch case (Define rules in ElementalListenerComponent.cpp)
FName UCharacterElemListenerComponent::GetTagName(EElementalStateType NextState)
{
	switch (NextState)
	{
	case EElementalStateType::Fire:
		return "State.Burn";
	case EElementalStateType::Water:
		return "State.Wet";
	case EElementalStateType::Ice:
		return "State.Frozen";
	default:
		return "State.None";
	}
}


