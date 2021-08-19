// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ExplosionComponent.h"
#include "CommonUtils.h"
#include "ElementalListenerComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AElementalBomb::AElementalBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;

	OngoingEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	OngoingEffectComp->SetupAttachment(RootComponent);

	OngoingAudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	OngoingAudioComp->SetupAttachment(RootComponent);

	//ExplosionComp = CreateDefaultSubobject<UExplosionComponent>("ExplosionComp");
	//ExplosionComp->SetupAttachment(RootComponent);
	//ExplosionComp->AutoActivateCollisionListener = false;

	StaticMeshComp->SetSimulatePhysics(true);


	ElementalListenerCollider = CreateDefaultSubobject<USphereComponent>("ElementalListnerColliderComp");
	ElementalListenerCollider->SetupAttachment(RootComponent);
	ElementalListenerCollider->SetCollisionProfileName("OverlapOnlyElementalEmitter");
	ElementalListenerComp = CreateDefaultSubobject<UElementalListenerComponent>("ElementalListenerComp");
	ElementalListenerComp->SetupAttachment(ElementalListenerCollider);


}

// Called when the game starts or when spawned
void AElementalBomb::BeginPlay()
{
	Super::BeginPlay();
	condition(ElementalListenerComp);
	//ElementalListenerComp->OnElementStateChanged.AddDynamic(this, &AElementalBomb::OnElementalStateChanged);
}

// Called every frame
void AElementalBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AElementalBomb::OnElementalStateChanged(EElementalStateType PreviousState, EElementalStateType ChangedState, AActor* Other)
//{
//	//UE_LOG(LogTemp, Log, TEXT("ElementalStateChanged."));
//	//
//	//if (PreviousState == Ice && ChangedState == None)
//	//{
//	//	//melt
//	//	SetLifeSpan(4.0f);
//	//}
//}


