// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalListenerComponent.h"
#include "ElementalBomb.generated.h"


UCLASS()
class ACTIONADVANTURE_API AElementalBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElementalBomb();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//class UExplosionComponent* ExplosionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* ElementalListenerCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UElementalListenerComponent* ElementalListenerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* ElementalEmitterCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UElementalEmitterComponent* ElementalEmitterComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* OngoingEffectComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* OngoingAudioComp;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION()
	//virtual void OnElementalStateChanged(EElementalStateType PreviousState, EElementalStateType ChangedState, AActor* Other);

};
