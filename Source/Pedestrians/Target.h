// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class PEDESTRIANS_API ATarget : public AActor
{
	GENERATED_BODY()

	// The mesh of the target
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	ATarget();
};
