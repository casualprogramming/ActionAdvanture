// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MontageAction.h"
#include "GrabAction.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UGrabAction : public UMontageAction
{
	GENERATED_BODY()
private:
	UPROPERTY()
	AActor* HitActor;
	UPROPERTY()
	AActor* GrabbedActor;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	float GrabRange;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	FName GrabSocketNameInOwner;
	
	UFUNCTION()
	void GrabEvent(AActor* Instigator);
public:
	UGrabAction();
	virtual bool CanStart_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bCancel) override;
	AActor* GetGrabbedActor() { return GrabbedActor; }
	FName GetGrabSocketName() {return GrabSocketNameInOwner;}

	UFUNCTION()
	void OnGrabbedActorDestroyedDoCancelAction(AActor* DestroyedActor) { CommitStopAction(GetOwner(),true);}
};
