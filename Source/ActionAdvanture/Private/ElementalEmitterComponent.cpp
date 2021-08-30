// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalEmitterComponent.h"
#include "CommonUtils.h"

// Sets default values for this component's properties
UElementalEmitterComponent::UElementalEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivateInComponentBeginplay = true;
	// ...
}


// Called when the game starts
void UElementalEmitterComponent::BeginPlay()
{
	Super::BeginPlay();
	EmitterCollider = Cast<UPrimitiveComponent>(GetAttachParent());//Emitter Collider
	conditionf(EmitterCollider, TEXT("Collider is not found. UElementalListenerComponent should be attached as a child of the UPrimitiveComponent"));
	conditionf(EmitterCollider->GetCollisionProfileName() == "OverlapOnlyElementalListner", TEXT("Check Actor %s"), *GetOwner()->GetName());
	//if (bAutoActivateInComponentBeginplay)
	//	EmitterCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//else
	//	EmitterCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	EmitterCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EmitterCollider->OnComponentBeginOverlap.AddDynamic(this, &UElementalEmitterComponent::OnEmitterBeginOverlap);

	if (bAutoActivateInComponentBeginplay)
	{
		ActivateElement();
	}

	/*TODO:
	* If it is already overlapped at start time, it will not generate overlap event.
	* I used the trick, but it's not the right way.*/
	
	//Initial Overlaps
	//if (EmitterCollider->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
	//{
	//	TArray<FOverlapResult> Hits;
	//	EmitterCollider->ComponentOverlapMulti(Hits,GetWorld(), EmitterCollider->GetComponentLocation(), EmitterCollider->GetComponentRotation(),ECollisionChannel::ECC_GameTraceChannel2);
	//	for (auto const& Hit: Hits)
	//	{
	//		UElementalListenerComponent* OtherListener = Hit.GetActor()->FindComponentByClass<UElementalListenerComponent>();
	//		condition(OtherListener);
	//		UE_LOG(LogTemp, Log, TEXT("%-16s |%12s|. %20s -> %-20s"), TEXT("BeginPlay Emit"), *UEnum::GetValueAsString(Element), *GetOwner()->GetName(), *Hit.GetActor()->GetName());
	//		OtherListener->RecieveElement(Element, GetOwner());
	//	}
	//}
}


// Called every frame
void UElementalEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UElementalEmitterComponent::OnEmitterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UElementalListenerComponent* OtherListener = OtherActor->FindComponentByClass<UElementalListenerComponent>();
	condition(OtherListener);
	UE_LOG(LogTemp, Log, TEXT("%-16s |%12s|. %20s -> %-20s"), TEXT("Emit"), *UEnum::GetValueAsString(Element), *GetOwner()->GetName(), *OtherActor->GetName());
	
	OtherListener->RecieveElement(Element,GetOwner());
}


void UElementalEmitterComponent::ActivateElement()
{
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindWeakLambda(this, [&]()
			{
				EmitterCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				EmitterCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		);
		GetWorld()->GetTimerManager().SetTimer(PeriodTimerHandle, Delegate, Period, true);
	}
}

void UElementalEmitterComponent::DeactivateElement()
{
	GetWorld()->GetTimerManager().ClearTimer(PeriodTimerHandle);
	EmitterCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
