// Fill out your copyright notice in the Description page of Project Settings.


#include "NewBlueprintFunctionLibrary.h"
#include "Action.h"

UAction* UNewBlueprintFunctionLibrary::GetDefaultActionClass(TSubclassOf<UAction> Class)
{
	return Class->GetDefaultObject<UAction>();
}

FName UNewBlueprintFunctionLibrary::GetDefaultActionName(TSubclassOf<UAction> Class)
{
	return Class->GetDefaultObject<UAction>()->GetActionName();
}
