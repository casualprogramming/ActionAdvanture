// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ExplosionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONADVANTURE_API UExplosionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExplosionComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	class UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	class USoundCue* ImpactSound;

	UPROPERTY(VisibleAnywhere, Category = "Explosion")
	UPrimitiveComponent* Collider;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	bool AutoActivateCollisionListener = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void ActivateCollisionListener();
	void DeactivateCollisionListener();
};
