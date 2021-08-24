// Fill out your copyright notice in the Description page of Project Settings.


#include "MontageAction.h"
#include "CommonUtils.h"
#include "DrawDebugHelpers.h"

UMontageAction::UMontageAction()
{
	ActionName = "MontageAction";
	PlayRate = 1.0f;
}

void UMontageAction::Initialize(UActionSystemComponent* ActionSystemComponent)
{
	Super::Initialize(ActionSystemComponent);
	OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();	condition(OwnerMesh);
	OwnerAnim = OwnerMesh->GetAnimInstance(); condition(OwnerAnim);
	if (StopAtMontageStop)
	{
		CommitStopDelegate.BindWeakLambda(this, [&](class UAnimMontage* Montage, bool bInterrupted )
			{
				CommitStopAction(GetOwner(), bInterrupted);
			});
	}
	condition2(OwnerMesh, OwnerAnim);
}

void UMontageAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	condition(Montage);
	OwnerAnim->Montage_Play(Montage, PlayRate);
	if(StopAtMontageStop)
		OwnerAnim->Montage_SetEndDelegate(CommitStopDelegate, Montage);//cancle delegate
}

void UMontageAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel); FOnMontageEnded EmptyFunction;
	if (StopAtMontageStop)
	{
		OwnerAnim->Montage_SetEndDelegate(EmptyFunction, Montage);//reset
		GetOwnerAnim()->Montage_Stop(Montage->GetDefaultBlendOutTime(), Montage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::Printf(TEXT("%s : %s"), *GetActionName().ToString(), bCancel? TEXT("Cancel") : TEXT("Stop")));
}

