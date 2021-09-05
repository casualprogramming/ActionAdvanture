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
	Collider = Cast<UPrimitiveComponent>(GetAttachParent());
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
	if (!Collider->OnComponentBeginOverlap.IsAlreadyBound(this, &UExplosionComponent::OnColliderBeginOverlap))
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &UExplosionComponent::OnColliderBeginOverlap);
		Collider->SetCollisionProfileName("OverlapAll");
	}
}

void UExplosionComponent::DeactivateCollisionListener()
{
	condition(Collider);
	Collider->OnComponentBeginOverlap.RemoveDynamic(this, &UExplosionComponent::OnColliderBeginOverlap);
}

void UExplosionComponent::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	condition(GetOwner());
	//Self Collision
	if (GetOwner()->GetInstigator() == OtherActor)
	{
		return;
	}
	if (ensure(IsValid(GetOwner())))
	{
		Explode(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void UExplosionComponent::Explode_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetComponentLocation(), GetComponentRotation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetComponentLocation());

	//		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
	OnExploded.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	GetOwner()->Destroy();
}


