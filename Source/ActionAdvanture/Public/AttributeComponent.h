// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, AActor*, InstigatorActor, float, NewValue, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthZero, AActor*, InstigatorActor);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONADVANTURE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxSP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float SP;

	void OnHealthZero_Internal(AActor* Instigator);

public:	

	FOnAttributeChanged OnHealthChanged;
	FOnAttributeChanged OnStaminaChanged;
	FOnHealthZero OnHealthZero;
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddHP(AActor* Instigator, float _HP);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddSP(AActor* Instigator, float _SP);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SubtractHP(AActor* Instigator, float _HP) { AddHP(Instigator, -_HP); }
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SubtractSP(AActor* Instigator, float _SP) { AddSP(Instigator, -_SP); }
};
