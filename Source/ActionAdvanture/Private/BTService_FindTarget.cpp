// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindTarget.h"
#include "AIController.h"
#include "CommonUtils.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_FindTarget::UBTService_FindTarget()
{

}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	condition(AIController);
	APawn* Self = AIController->GetPawn();
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	condition2(Self, BlackBoardComp);


	//Find Actor in Range
	FVector const Direction = Self->GetActorForwardVector();
	FVector const Origin = Self->GetActorLocation();

	FCollisionQueryParams params;
	params.AddIgnoredActor(Self);
	
	//FHitResult Hit;
	//bool bSweep = GetWorld()->SweepSingleByObjectType(Hit, Origin, Origin, FQuat::Identity, ObjectType.GetValue(), FCollisionShape::MakeSphere(FindRange), params);
	
	TArray<FHitResult> Hits;
	GetWorld()->SweepMultiByObjectType(Hits, Origin, Origin, FQuat::Identity, ObjectType.GetValue(), FCollisionShape::MakeSphere(FindRange), params);

#if ENABLE_DRAW_DEBUG
	FColor	DrawColor = Hits.Num()==0 ? FColor::Red : FColor::Green;
	DrawDebugSphere(GetWorld(), Origin, FindRange, 20, DrawColor, false, 0.3f);
#endif

	for (auto const& Hit : Hits) 
	{

	AActor* Target = Hit.GetActor();

	//Check view angle (Reference: PawnSensingComponent.h, ::CouldSeePawn, ::HasLineOfSightTo)
	if (Target)
	{
		FRotator SelfEyeRotation; FVector SelfEyeLocation; 
		Self->GetActorEyesViewPoint(SelfEyeLocation, SelfEyeRotation);
		FVector const SelfEyeToTarget = Target->GetActorLocation() - SelfEyeLocation;
		FVector const SelfEyeToTargetDir = SelfEyeToTarget.GetSafeNormal();
		float ViewAngle = FVector::DotProduct(SelfEyeToTargetDir, Self->GetActorRotation().Vector());

		if (ViewAngle > FMath::Cos(FMath::DegreesToRadians(FindViewAngle)))
		{
			//Check visibility
			FCollisionQueryParams CollisionParms(SCENE_QUERY_STAT(LineOfSight), true, Target);
			CollisionParms.AddIgnoredActor(Self);
			bool bHit = GetWorld()->LineTraceTestByChannel(SelfEyeLocation, Target->GetActorLocation(), ECC_Visibility, CollisionParms);
			if (!bHit)
			{
				BlackBoardComp->SetValueAsObject(TargetActorKey.SelectedKeyName, Target);
				return;
			}
		}
	}

	}


	BlackBoardComp->SetValueAsObject(TargetActorKey.SelectedKeyName, nullptr);
}
