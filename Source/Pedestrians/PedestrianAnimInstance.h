// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PedestrianAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PEDESTRIANS_API UPedestrianAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Speed;

	APawn *OwningPawn;
};
