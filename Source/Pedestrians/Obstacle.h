// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class PEDESTRIANS_API AObstacle : public AActor
{
	GENERATED_BODY()

	// The mesh of the obstacle
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	// The radius of the obstacle
	UPROPERTY(EditAnywhere)
		float Radius = 100;
};