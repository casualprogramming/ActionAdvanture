// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NewBlueprintFunctionLibrary.h"
#include "ActionSystemComponent.generated.h"

class UAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionCustomEvents, AActor*, Instigator);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomEvent, AActor*, Instigator);

/** (It was possible to compile without using a wrapping class and there was no problem at runtime, but the crash occurred when the megascan plugin was put, and it was fixed to wrapping class.)
 *  Warpping Class for Action Event delegate to be stored in TMap
 */
USTRUCT(Atomic, BlueprintType)
struct FActionEventDelegateWrapper {
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FOnActionCustomEvents Delegate;
};

USTRUCT(Atomic, BlueprintType)
struct FActionArray{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	TArray<UAction*> Actions;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONADVANTURE_API UActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UActionSystemComponent();

protected:
	UPROPERTY(VisibleAnywhere)
	TMap<FName, uint32> ActiveTagCountMap;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, FActionArray> CancelActionByTagMap;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, UAction*> Actions;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, FActionEventDelegateWrapper> CustomEventMap;

	virtual void BeginPlay() override;

	/* Granted abilities at game start */
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UAction>> DefaultActions;

public:	

	FOnActionCustomEvents& GetEventDelegate(FName EventName) {return CustomEventMap.FindOrAdd(EventName).Delegate;}
	
	/* After binding, the function remains even if the Uobject is deleted. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void BindEventUnsafe(FName EventName, FOnCustomEvent const& Event) { CustomEventMap.FindOrAdd(EventName).Delegate.Add(Event); }
	void DeleteEvent(FName EventName, FOnCustomEvent const& Event) { FActionEventDelegateWrapper* Events = CustomEventMap.Find(EventName); if (Events) Events->Delegate.Remove(Event);}


	UFUNCTION(BlueprintCallable, Category = "Action")
	void BroadcastEvent(FName EventName, AActor* Instigator) { FActionEventDelegateWrapper* Events = CustomEventMap.Find(EventName); if(Events)Events->Delegate.Broadcast(Instigator);}


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
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	void CancelActionByTags(FGameplayTagContainer const& Tags, AActor* Instigator = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddCancelTagsListener(FGameplayTagContainer const& CancelTags, UAction* Listener);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void DeleteCancelTagsListener(FGameplayTagContainer const& CancelTags, UAction* Listener);

	UAction* GetAction(FName ActionName) const { return Actions[ActionName]; }

	template<typename T>
	T* GetAction() const { return Cast<T>(Actions[T::StaticClass()->GetDefaultObject<T>()->GetActionName()]); }

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
