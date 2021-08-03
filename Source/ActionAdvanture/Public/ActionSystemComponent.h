// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	TMap<uint32, UAction*> Actions;

	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(UAction* Action);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartAction(UAction* Action, AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopAction(UAction* Action, AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	void ActivateTags(FGameplayTagContainer const& Tags);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	void DeactivateTags(FGameplayTagContainer const& Tags);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsBlockedWith(FGameplayTagContainer const& BlockTags) const;
};
