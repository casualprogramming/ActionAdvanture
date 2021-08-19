// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <array>
#include <functional>
#include "ElementalListenerComponent.generated.h"
UENUM(BlueprintType)
enum EElementalStateType
{
	None,
	Fire,
	Water,
	Ice,
	ElementalStateTypeCount,
};

UENUM(BlueprintType)
enum EElementalChangeType
{
	NO_CHANGE,
	MELT,
	BURN,
	FROZEN,
	WET,
	EXTINGUISHED,
	
	ElementalChangeTypeCount,
};

struct EElementalChangeData
{ 
	EElementalStateType NextState;
	EElementalChangeType ChangeType;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnElementalStateChanged, EElementalStateType, PreviousState, EElementalStateType, ChangedState, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnElementalStateChangedNoParam);


UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class ACTIONADVANTURE_API UElementalListenerComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:
	static const std::array<std::array<EElementalChangeData, ElementalStateTypeCount>, ElementalStateTypeCount> StateRule;
	static const EElementalChangeData& QueryRule(EElementalStateType State, EElementalStateType ReceivedElement) { return StateRule[State][ReceivedElement]; }

	static void Melt(UElementalListenerComponent* Self) { Self->OnMelt.Broadcast(); };
	static void Burn(UElementalListenerComponent* Self) { Self->OnBurn.Broadcast(); };
	static void Frozen(UElementalListenerComponent* Self) { Self->OnFrozen.Broadcast(); };
	static void Wet(UElementalListenerComponent* Self) { Self->OnWet.Broadcast(); };
	static void Extinguished(UElementalListenerComponent* Self) { Self->OnExtinguished.Broadcast(); };

public:	
	// Sets default values for this component's properties
	UElementalListenerComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElementalListener")
	TEnumAsByte<EElementalStateType> State;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ElementalListener")
	TSet<TEnumAsByte<EElementalChangeType>> IgnoreMap;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	static const std::array<std::function<void(UElementalListenerComponent*)>, ElementalChangeTypeCount> StateChangeEventMap;

	UPROPERTY(BlueprintAssignable, Category = "ElementalListener")
	FOnElementalStateChangedNoParam OnMelt;//[Ice, Fire] -> None

	UPROPERTY(BlueprintAssignable, Category = "ElementalListener")
	FOnElementalStateChangedNoParam OnBurn;//[None, Fire] -> Fire

	UPROPERTY(BlueprintAssignable, Category = "ElementalListener")
	FOnElementalStateChangedNoParam OnFrozen;//[Water, Ice] -> Ice

	UPROPERTY(BlueprintAssignable, Category = "ElementalListener")
	FOnElementalStateChangedNoParam OnWet;//[None, Water] -> Wet

	UPROPERTY(BlueprintAssignable, Category = "ElementalListener")
	FOnElementalStateChangedNoParam OnExtinguished;//[Fire, Water] -> Extinguished

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void RecieveElement(EElementalStateType ReceivedElement, AActor* Instigator);
	UFUNCTION()
	void OnListenerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	EElementalStateType GetState() { return State; }
};


