// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalListenerComponent.h"
#include "GameplayTagContainer.h"
#include "CharacterElemListenerComponent.generated.h"

/**
 * 
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONADVANTURE_API UCharacterElemListenerComponent : public UElementalListenerComponent
{
	GENERATED_BODY()
	
	static const std::array<EElementalStateType, ElementalChangeTypeCount> ElementsForReturningNone;
	std::array<float*, ElementalChangeTypeCount> DurationMap;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterElementalListener")
	float FrozenDuration = 3.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterElementalListener")
	float WetDuration = 3.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterElementalListener")
	float BurningDuration = 3.0f;

	FTimerHandle DurationHandle;

	UPROPERTY()
	class UActionSystemComponent* ActionSystemComp;

	FGameplayTag CurrentElementStateTag;

public:
	UCharacterElemListenerComponent();
	virtual void BeginPlay() override;
	virtual void StateChangeBegin(EElementalStateType CurrentState, EElementalStateType NextState, EElementalChangeType ChangeType, AActor* Instigator) override;
	static EElementalStateType ElementsToNoneState(EElementalChangeType CurrentState);
	static FName GetTagName(EElementalStateType ChangeType);
};
