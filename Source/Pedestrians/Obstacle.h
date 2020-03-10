// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class PEDESTRIANS_API AObstacle : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	UPROPERTY(EditAnywhere)
		float Radius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
