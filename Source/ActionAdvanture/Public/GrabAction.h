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
	class UAnimInstance* OwnerAnim;
	class USkeletalMeshComponent* OwnerMesh;
private:
	UPROPERTY()
	AActor* GrabbedActor;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	UAnimMontage* GrabAnim;

	UPROPERTY(EditAnywhere, Category = "Grab")
	UAnimMontage* GrabStopAnim;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	float GrabRange;

	UPROPERTY(EditAnywhere, Category = "GrabAndThrow")
	FName GrabSocketNameInOwner;

public:
	virtual void Initialize(UActionSystemComponent* ActionSystemComponent) override;
	virtual bool CanStart_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

};
