// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionComponent.h"
#include "CommonUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UExplosionComponent::UExplosionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
	Collider = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	condition(Collider);
	if (AutoActivateCollisionListener)
	{
		ActivateCollisionListener();
	}
	// ...
}


// Called every frame
void UExplosionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UExplosionComponent::ActivateCollisionListener()
{
	condition(Collider);
	if (!Collider->OnComponentBeginOverlap.IsAlreadyBound(this, &UExplosionComponent::Explode))
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &UExplosionComponent::Explode);
		Collider->SetCollisionProfileName("OverlapAll");
	}
}

void UExplosionComponent::DeactivateCollisionListener()
{
	condition(Collider);
	Collider->OnComponentBeginOverlap.RemoveDynamic(this, &UExplosionComponent::Explode);
}

void UExplosionComponent::Explode_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	condition(GetOwner());

	if (GetOwner()->GetInstigator() == OtherActor)
	{
		return;
	}

	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all
	if (ensure(!GetOwner()->IsPendingKill()))
	{
		UE_LOG(LogTemp, Log, TEXT("overlap %s."), *OtherActor->GetName());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetComponentLocation(), GetComponentRotation());

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetComponentLocation());

		//		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

		GetOwner()->Destroy();
	}
}


