// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ElementalListenerComponent.h"
#include "ElementalEmitterComponent.generated.h"


UCLASS( ClassGroup=(Custom), hidecategories = (Activation), meta=(BlueprintSpawnableComponent) )
class ACTIONADVANTURE_API UElementalEmitterComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElementalEmitterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EmitterElement")
	TEnumAsByte<EElementalStateType> Element;

	UPROPERTY()
	class UPrimitiveComponent* EmitterCollider;

	/** Forces the collider to enable collision when this component beginplay. otherwise disable. Be careful because the order of beginplay is not guaranteed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EmitterElement")
	bool bAutoActivateInComponentBeginplay;

	FTimerHandle StartBeginOverlapTrickTimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	virtual void OnEmitterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ActivateElement(TEnumAsByte<EElementalStateType> InElement);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override { Super::EndPlay(EndPlayReason); GetWorld()->GetTimerManager().ClearTimer(StartBeginOverlapTrickTimerHandle); };

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EElementalStateType> const& GetElement() { return Element; }
};
