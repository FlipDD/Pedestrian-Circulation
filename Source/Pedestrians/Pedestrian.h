// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pedestrian.generated.h"

UCLASS(config = Game)
class APedestrian : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UArrowSceneComp* Arrow;

public:
	APedestrian();

	virtual void Tick(float DeltaTime) override;

	TArray<APedestrian*> Agents;
	TArray<class ATarget*> Targets;
	TArray<class AObstacle*> Obstacles;
	TArray<class AWallu*> Walls;

	float vX = 0;
	float vY = 0;
	float aX = 0;
	float aY = 0;
	float fX = 0;
	float fY = 0;
	double fMag = 0;
	float nX = 0;
	float nY = 0;
	float dist = 0;	

	float fx1, fy1, fx2, fy2, fx3, fy3;

	UPROPERTY(EditAnywhere)
		float mass = 80;
	UPROPERTY(EditAnywhere)
		float radius = 0.25;
	UPROPERTY(EditAnywhere)
		float A = 2000;
	UPROPERTY(EditAnywhere)
		float B = 0.08;
	UPROPERTY(EditAnywhere)
		float AWall = 2000;
	UPROPERTY(EditAnywhere)
		float vDes = 1;
	UPROPERTY(EditAnywhere)
		float tau = 0.5;
	UPROPERTY(EditAnywhere)
		float hue = 90;

	// Target stuff;
	UPROPERTY(EditAnywhere)
		int targNr = 0;

	// Debug force arrow switches
	bool bDrawTargetForce = false;
	bool bDrawWallForce = true;
	bool bDrawAgentAgentForce = false;
	bool bDrawTotalForce = true;
	bool bDrawObstacleForce;

protected:
	virtual void BeginPlay() override;
};

