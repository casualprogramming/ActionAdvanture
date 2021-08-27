// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "MontageAction.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UMontageAction : public UAction
{
	GENERATED_BODY()

private:
	/*cache data OwnerAnim, OwnerMesh*/

	UPROPERTY()
	class UAnimInstance* OwnerAnim;

	UPROPERTY()
	class USkeletalMeshComponent* OwnerMesh;
	
	FOnMontageEnded CommitStopDelegate;

protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	bool StopAtMontageStop = true;

public:
	UMontageAction();
	UAnimInstance* GetOwnerAnim() { return OwnerAnim; }
	USkeletalMeshComponent* GetOwnerMesh() { return OwnerMesh; }
	virtual void Initialize_Implementation(UActionSystemComponent* ActionSystemComponent) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bCancel) override;
};
