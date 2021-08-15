// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashMob.h"
#include "ActionSystemComponent.h"
#include "AttributeComponent.h"

// Sets default values
ATrashMob::ATrashMob()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ActionSystemComp = CreateDefaultSubobject<UActionSystemComponent>("ActionSystemComp");

	AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComp");
}

// Called when the game starts or when spawned
void ATrashMob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrashMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrashMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

