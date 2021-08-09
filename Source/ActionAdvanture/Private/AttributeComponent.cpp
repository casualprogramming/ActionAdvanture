// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void UAttributeComponent::AddHP(AActor* Instigator, float HP_)
{
	float OldHP = HP;
	HP = FMath::Clamp(HP + HP_, 0.0f, MaxHP);
	float Delta = HP - OldHP;
	if(Delta!=0.0f)
		OnHealthChanged.Broadcast(Instigator, HP, Delta);
	if (HP == 0)
		OnHealthZero_Internal(Instigator);
}

void UAttributeComponent::AddSP(AActor* Instigator, float SP_)
{
	float OldSP = SP;
	SP = FMath::Clamp(SP + SP_, 0.0f, MaxSP);
	float Delta = SP - OldSP;
	if (Delta != 0.0f)
		OnStaminaChanged.Broadcast(Instigator, SP, Delta);
}

void UAttributeComponent::OnHealthZero_Internal(AActor* Instigator)
{
	OnHealthZero.Broadcast(Instigator);
	GetOwner()->SetLifeSpan(4.0f);
}