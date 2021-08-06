// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NewBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
class UAction;
UCLASS()
class ACTIONADVANTURE_API UNewBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/*It is better not to expose this function, 
	 *but it is often used because the blueprint class defaults value for UObject (UAction) is not visible.*/
	UFUNCTION(BlueprintCallable, Category = "NewBlueprintFunction")
	static UAction* GetDefaultActionClass(TSubclassOf<UAction> Class);
};