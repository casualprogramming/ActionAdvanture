// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NewBlueprintFunctionLibrary.h"
#include "Action.generated.h"

/**
 * Actions are performed according to the tag conditions.
 * Actions are similar GameAbility of GameAbilitySystem.
 * References: https://github.com/tomlooman/ActionRoguelike, https://docs.unrealengine.com/4.26/en-US/InteractiveExperiences/GameplayAbilitySystem/
 */

class UActionSystemComponent;
class UCooldownAction;

USTRUCT(Atomic, BlueprintType)
struct FChildActionDesc
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAction> ActionClass;

	/* Check CanStart in parent. Affects the parent's CanStart. If set to false, Do not Affects the parent's CanStart and this action's CanStart() will be called when the parent Start() is called.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCallCanStartAtParent;

	/* Add the action name after the parent name. Thanks to the new name, the action is identified and instantiated.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInstancingWithParentName = true;

	/* The action starts when the parent action starts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSyncActionStartWithParent = true;

	/* The action stops when the parent action stops. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSyncActionStopWithParent = true;

	//Available after Initialize
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAction* Action;
};


UCLASS(Blueprintable)
class ACTIONADVANTURE_API UAction : public UObject
{
	GENERATED_BODY()
private:
	/* cache data*/
	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	UActionSystemComponent* OwnerActionSystem;

	//true at StartAction, false at StopAction
	bool bIsRunning;

	UCooldownAction* GetorAddChildCooldownHelperAction();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer ActiveTags;

	/* Action can only start if OwningActor has none of these Tags applied*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags", DisplayName = "Blocked with Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags", DisplayName = "Canceled with Tags")
	FGameplayTagContainer CanceledTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer ActiveCancelTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	AActor* GetOwner() const { return Owner; };
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionSystemComponent* GetOwnerActionSystem() const { return OwnerActionSystem; };

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<FChildActionDesc> ChildActionsClass;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator, bool bCancel);

public:

	//Rename forcibly when you want to use an instance different from the class default.
	void SetActionName(FName name) { ActionName = name; }

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void Initialize(UActionSystemComponent* ActionSystemComponent);

	UFUNCTION(BlueprintPure, Category = "Action")
	FName GetActionName() const { return ActionName; };

	//NOTE: Use BlockedTags instead of CanStart whenever possible
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void CommitStartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void CommitStopAction(AActor* Instigator, bool bCancel);

	const FGameplayTagContainer& GetBlockTags() const{ return BlockedTags; }
	const FGameplayTagContainer& GetActiveTags() const{ return ActiveTags; }
	const FGameplayTagContainer& GetRequiredTags() const { return RequiredTags; }

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const{ return bIsRunning; };

	UWorld* GetWorld() const override;

	//helper function for creating child CooldownAction (runtime cooldown)
	UPROPERTY()
	UCooldownAction* ChildCooldownAction;
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddCooldown(float CooldownDuration, bool bActorTimer = false);
	UFUNCTION(BlueprintCallable, Category = "Action")
	void StartCooldown(float CooldownDuration, bool bActorTimer = false);

	TArray<TPair<FName, FOnCustomEvent>> EventsDuringRunning;
	UFUNCTION(BlueprintCallable, Category = "Action")
	void BindEventDuringRunning(FName EventName, FOnCustomEvent const& Event);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void UnBindEventDuringRunning(FName EventName, FOnCustomEvent const& Event);
};


