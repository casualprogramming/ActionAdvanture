// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MontageAction.h"
#include "MeleeAttackAction.generated.h"

class UPrimitiveComponent;
/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UMeleeAttackAction : public UMontageAction
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "MeleeAttackAction")
	UPrimitiveComponent* MeleeAttackCollider;

	UPROPERTY(VisibleAnywhere, Category = "MeleeAttackAction")
	FName MeleeColliderComponentTag = "MeleeAttackCollider";

public:
	UMeleeAttackAction();
	virtual void Initialize(UActionSystemComponent* ActionSystemComponent) override;
	virtual bool CanStart_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bCancel) override;

	UFUNCTION()
	virtual void OnEventEnableMeleeAttackCollision(AActor* Instigator);
	UFUNCTION()
	virtual void OnEventDisableMeleeAttackCollision(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "MeleeAttackAction")
	void OnMeleeAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
