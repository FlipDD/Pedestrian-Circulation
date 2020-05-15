// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wallu.generated.h"

UCLASS()
class PEDESTRIANS_API AWallu : public AActor
{
	GENERATED_BODY()

	// The mesh of the wall
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* Mesh;
public:	
	// Sets default values for this actor's properties
	AWallu();
};
