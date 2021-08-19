// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalListenerComponent.h"
#include "ElementalEmitter.generated.h"

UCLASS()
class ACTIONADVANTURE_API AElementalEmitter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElementalEmitter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* ElementalEmitterCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UElementalEmitterComponent* ElementalEmitterComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UParticleSystemComponent* EmitterParticle;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Emitter")
	virtual void ActivateEmitter();

	UFUNCTION(BlueprintCallable, Category = "Emitter")
	virtual void DeactivateEmitter();

};
