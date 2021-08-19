// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalListenerComponent.h"
#include "ElementalEmitterComponent.h"
#include "CommonUtils.h"
#include <array>

const std::array<std::array<EElementalChangeData, ElementalStateTypeCount>, ElementalStateTypeCount> UElementalListenerComponent::StateRule = [] {
	std::array<std::array<EElementalChangeData, ElementalStateTypeCount>, ElementalStateTypeCount> Rules;

	//[Current][Receive] = [Next]
	Rules[None][None]	= { None, NO_CHANGE };							//no change
	Rules[None][Fire]	= {	Fire, BURN};								//Burn
	Rules[None][Ice]	= {	None, NO_CHANGE };							//no change
	Rules[None][Water]	= { Water, WET};								//Wet
						  	
	Rules[Fire][None]	= { Fire, NO_CHANGE };							//no change
	Rules[Fire][Fire]	= { Fire, NO_CHANGE };							//no change
	Rules[Fire][Ice]	= { Fire, NO_CHANGE };							//no change
	Rules[Fire][Water]	= { None, EXTINGUISHED };							//extinguished
						  	
	Rules[Ice][None]	= { Ice, NO_CHANGE };								//no change
	Rules[Ice][Fire]	= { None, MELT };							//melt
	Rules[Ice][Ice]		= { Ice, NO_CHANGE };								//no change
	Rules[Ice][Water]	= { Ice, NO_CHANGE };							//no change
						  
	Rules[Water][None]	= { Water, NO_CHANGE };							//no change
	Rules[Water][Fire]	= { Water, NO_CHANGE };							//no change
	Rules[Water][Ice]	= { Ice, FROZEN };							//frozen
	Rules[Water][Water] = { Water, NO_CHANGE };						//no change
	return Rules;
}();

const std::array<std::function<void(UElementalListenerComponent*)>, ElementalChangeTypeCount> UElementalListenerComponent::StateChangeEventMap = [] {
	std::array<std::function<void(UElementalListenerComponent*)>, ElementalChangeTypeCount> EvemtMap;

	EvemtMap[NO_CHANGE] =nullptr;
	EvemtMap[BURN] = &UElementalListenerComponent::Burn;
	EvemtMap[WET] = &UElementalListenerComponent::Wet;
	EvemtMap[MELT] = &UElementalListenerComponent::Melt;
	EvemtMap[FROZEN] = &UElementalListenerComponent::Frozen;
	EvemtMap[EXTINGUISHED] = &UElementalListenerComponent::Extinguished;
	return EvemtMap;
}();


// Sets default values for this component's properties
UElementalListenerComponent::UElementalListenerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UElementalListenerComponent::BeginPlay()
{
	Super::BeginPlay();
	UPrimitiveComponent* Collider = Cast<UPrimitiveComponent>(GetAttachParent());//listener collider
	conditionf(Collider,TEXT("Collider is not found. UElementalListenerComponent should be attached as a child of the UPrimitiveComponent"));
	condition(Collider->GetCollisionProfileName() == "OverlapOnlyElementalEmitter");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &UElementalListenerComponent::OnListenerBeginOverlap);


	//To simplify the logic, this object is always waiting for element overlap.
	// ...
	
}


// Called every frame
void UElementalListenerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UElementalListenerComponent::RecieveElement(EElementalStateType InState, AActor* Instigator)
{
	auto const& Rule = QueryRule(State, InState);
	if (!IgnoreMap.Contains(Rule.ChangeType))
	{
		auto PreviousState = State;
		auto const& EventFunc = StateChangeEventMap[Rule.ChangeType];
		EventFunc(this);
		State = Rule.NextState;
		UE_LOG(LogTemp, Log, TEXT("UElementalListenerComponent::Change [%s] -> [%s] %s is ElementalStateChanged by %s."), *UEnum::GetValueAsString(PreviousState.GetValue()), *UEnum::GetValueAsString(State.GetValue()), *GetOwner()->GetName(), Instigator ? *Instigator->GetName() : TEXT("null"));
	}
}


void UElementalListenerComponent::OnListenerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	condition(GetOwner());
	UElementalEmitterComponent* OtherEmitter = OtherActor->FindComponentByClass<UElementalEmitterComponent>();
	condition(OtherEmitter);
	UE_LOG(LogTemp, Log, TEXT("UElementalListenerComponent::Overlap. %s <- %s, TryChange %s <- %s "), *GetOwner()->GetName(), *OtherActor->GetName(), *UEnum::GetValueAsString(State.GetValue()), *UEnum::GetValueAsString(OtherEmitter->GetElement().GetValue()));

	if(OtherEmitter)
	{
		RecieveElement(OtherEmitter->GetElement(), OtherActor->GetInstigator());
	}
}