// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalEmitter.h"
#include "ElementalEmitterComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AElementalEmitter::AElementalEmitter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ElementalEmitterCollider = CreateDefaultSubobject<USphereComponent>("ElementalListnerColliderComp");
	RootComponent = ElementalEmitterCollider;
	ElementalEmitterCollider->SetCollisionProfileName("OverlapOnlyElementalListener");
	ElementalEmitterComp = CreateDefaultSubobject<UElementalEmitterComponent>("ElementalEmitterComp");
	ElementalEmitterComp->SetupAttachment(RootComponent);

	EmitterParticle = CreateDefaultSubobject<UParticleSystemComponent>("EmitterPaticle");

	EmitterParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AElementalEmitter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AElementalEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElementalEmitter::ActivateEmitter()
{
	EmitterParticle->Activate();
	ElementalEmitterComp->Activate();//automatic Activate ElementalEmitterCollider
}

void AElementalEmitter::DeactivateEmitter()
{
	EmitterParticle->Deactivate();
	ElementalEmitterComp->Deactivate();//automatic Deactivate ElementalEmitterCollider
}
