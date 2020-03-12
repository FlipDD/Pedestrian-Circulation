// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ArrowSceneComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEDESTRIANS_API UArrowSceneComp : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* F1Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* F2Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* F3Mesh;

public:	
	// Sets default values for this component's properties
	UArrowSceneComp();

	void SetF1Rot(FRotator Rot);
	void SetF2Rot(FRotator Rot);
	void SetF3Rot(FRotator Rot);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
