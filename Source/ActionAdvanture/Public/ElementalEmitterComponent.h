// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ElementalListenerComponent.h"
#include "ElementalEmitterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	class UPrimitiveComponent* EmitterCollider;

	bool bAutoActivateEmitter;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	virtual void OnEmitterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ActivateElement(TEnumAsByte<EElementalStateType> InElement);
	virtual void Activate(bool bReset=false) override;
	virtual void Deactivate() override;

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EElementalStateType> const& GetElement() { return Element; }
};
