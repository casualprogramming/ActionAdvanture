// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MontageAction.h"
#include "TelekinesisAction.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UTelekinesisAction : public UMontageAction
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AActor* HitActor;

	UPROPERTY()
	class UPrimitiveComponent* HitActorCollider;

	UPROPERTY(EditAnywhere, Category = "Telekinesis")
	float FindRange = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Telekinesis")
	float Radius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Telekinesis")
	FName GrabSocketNameInOwner;

	UPROPERTY(EditAnywhere, Category = "Telekinesis", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GrabSpeed = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Telekinesis")
	float AttachThreshold;

	UPROPERTY(EditAnywhere, Category = "Telekinesis")
	float LaunchForce=1500.0f;

	FTimerHandle MoveToSocketHandle;

	UFUNCTION()
	void GrabEvent(AActor* Instigator);
public:
	UTelekinesisAction();
	virtual bool CanStart_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bCancel) override;

	UFUNCTION()
	void OnHitActorDestroyedDoCancelAction(AActor* DestroyedActor) { CommitStopAction(GetOwner(), true); }
};
