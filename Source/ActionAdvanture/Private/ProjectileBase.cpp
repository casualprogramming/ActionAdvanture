// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ExplosionComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	OngoingEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	OngoingEffectComp->SetupAttachment(RootComponent);
	
	OngoingAudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	OngoingAudioComp->SetupAttachment(RootComponent);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");

	//Initial speed if you want to use simulating physics of SphereComponent, otherwise use Velocity
	MoveComp->Velocity = FVector(500, 0, 500);
	//MoveComp->bRotationFollowsVelocity = true;
	//MoveComp->bInitialVelocityInLocalSpace = true;
	//MoveComp->ProjectileGravityScale = 0.0f;
	//MoveComp->InitialSpeed = 8000;

	ExplosionComp = CreateDefaultSubobject<UExplosionComponent>("ExplosionComp");
	ExplosionComp->SetupAttachment(RootComponent);
	
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

