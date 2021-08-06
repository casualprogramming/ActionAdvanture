// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Action.generated.h"

/**
 * Actions are performed according to the tag conditions.
 * Actions are similar GameAbility of GameAbilitySystem.
 * References: https://github.com/tomlooman/ActionRoguelike, https://docs.unrealengine.com/4.26/en-US/InteractiveExperiences/GameplayAbilitySystem/
 */

class UActionSystemComponent;

UCLASS(Blueprintable)
class ACTIONADVANTURE_API UAction : public UObject
{
	GENERATED_BODY()
private:
	/* cache data*/
	AActor* Owner;
	UActionSystemComponent* OwnerActionSystem;

	//true at StartAction, false at StopAction
	bool bIsRunning;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer ActiveTags;

	/* Action can only start if OwningActor has none of these Tags applied*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	AActor* GetOwner() const { return Owner; };
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionSystemComponent* GetOwnerActionSystem() const { return OwnerActionSystem; };

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

public:
	virtual void Initialize(UActionSystemComponent* ActionSystemComponent);

	UFUNCTION(BlueprintPure, Category = "Action")
	FName const& GetActionName() const { return ActionName; };

	//NOTE: Use BlockedTags instead of CanStart whenever possible
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	const FGameplayTagContainer& GetBlockTags() const{ return BlockedTags; }
	const FGameplayTagContainer& GetActiveTags() const{ return ActiveTags; }

	bool IsRunning() const{ return bIsRunning; };
};
