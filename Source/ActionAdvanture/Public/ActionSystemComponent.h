// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NewBlueprintFunctionLibrary.h"
#include "ActionSystemComponent.generated.h"

class UAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONADVANTURE_API UActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UActionSystemComponent();

protected:
	UPROPERTY(VisibleAnywhere)
	TSet<FName> ActiveTags;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, UAction*> Actions;

	virtual void BeginPlay() override;

	/* Granted abilities at game start */
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UAction>> DefaultActions;

public:	
	/*	To start an Action, you need to register the Action. 
		It is recommended to use class default, If you want to instantiate the same class within one actor, use forcedname to avoid collision of Action Map.*/
	UFUNCTION(BlueprintCallable, Category = "Action")
	void RegisterAction(TSubclassOf<UAction> ActionClass, AActor* Instigator, FName ForcedActionName="");

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StartActionByName(FName ActionName, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StopActionByName(FName ActionName, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void CancelActionByName(FName ActionName, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void ActivateTags(FGameplayTagContainer const& Tags);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	void DeactivateTags(FGameplayTagContainer const& Tags);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool HasAny(FGameplayTagContainer const& BlockTags) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool HasAll(FGameplayTagContainer const& Tags) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool SatisfyTagRequirements(const UAction* Action) const;

	bool IsRegisteredAction(FName ActionName) const { return bool(Actions.Find(ActionName)); }

	UFUNCTION(BlueprintCallable, Category = "Action")
	void CancelAllAction(AActor* Instigator = nullptr);
	
	UAction* GetAction(FName ActionName) const { return Actions[ActionName]; }

	template<typename T>
	T* GetAction() const { return Cast<T>(Actions[T::StaticClass()->GetDefaultObject<T>()->GetActionName()]); }
};
