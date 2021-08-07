// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "GrabAction.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UGrabAction : public UAction
{
	GENERATED_BODY()
private:
	/*cache data*/
	UPROPERTY()
	class UAnimInstance* OwnerAnim;

	UPROPERTY()
	class USkeletalMeshComponent* OwnerMesh;
private:
	UPROPERTY()
	AActor* GrabbedActor;
	
	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	UAnimMontage* GrabMontage;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	UAnimMontage* CancelAnim;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	float GrabRange;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	float ThrowForce;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	FName GrabSocketNameInOwner;

public:
	UGrabAction();
	virtual void Initialize(UActionSystemComponent* ActionSystemComponent) override;
	virtual bool CanStart_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bCancel) override;
	AActor* GetGrabbedActor() { return GrabbedActor; }
	FName GetGrabSocketName() {return GrabSocketNameInOwner;}
};
