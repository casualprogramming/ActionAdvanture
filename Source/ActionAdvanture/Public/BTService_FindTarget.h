// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UBTService_FindTarget : public UBTService
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float FindRange;

	UPROPERTY(EditAnywhere, Category = "AI")
	float FindViewAngle;

	UPROPERTY(EditAnywhere, Category = "AI")
	TEnumAsByte<ECollisionChannel> ObjectType;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:

	UBTService_FindTarget();
};
