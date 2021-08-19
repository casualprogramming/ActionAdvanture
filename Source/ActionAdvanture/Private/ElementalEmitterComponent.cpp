// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalEmitterComponent.h"
#include "CommonUtils.h"

// Sets default values for this component's properties
UElementalEmitterComponent::UElementalEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	// ...
}


// Called when the game starts
void UElementalEmitterComponent::BeginPlay()
{
	Super::BeginPlay();
	EmitterCollider = Cast<UPrimitiveComponent>(GetAttachParent());//Emitter Collider
	conditionf(EmitterCollider, TEXT("Collider is not found. UElementalListenerComponent should be attached as a child of the UPrimitiveComponent"));
	condition(EmitterCollider->GetCollisionProfileName() == "OverlapOnlyElementalListner");
	EmitterCollider->OnComponentBeginOverlap.AddDynamic(this, &UElementalEmitterComponent::OnEmitterBeginOverlap);
	if (bAutoActivate)
	{
		EmitterCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	// ...
}


// Called every frame
void UElementalEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UElementalEmitterComponent::OnEmitterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	condition(GetOwner());
	UE_LOG(LogTemp, Log, TEXT("EmitterOverlap. %s -> %s"), *GetOwner()->GetName(), *OtherActor->GetName());
}


void UElementalEmitterComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	if(EmitterCollider)
		EmitterCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UElementalEmitterComponent::Deactivate()
{
	Super::Deactivate();
	if (EmitterCollider)
		EmitterCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UElementalEmitterComponent::ActivateElement(TEnumAsByte<EElementalStateType> InElement)
{
	Element = InElement;
	Activate(false);
}
