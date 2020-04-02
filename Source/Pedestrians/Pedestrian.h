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

	class UPedestrianAnimInstance* PedestrianAnim;

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
	UPROPERTY(EditAnywhere)
		float fMag = 0;
	float nX = 0;
	float nY = 0;
	float dist = 0;	

	float fx1, fy1, fx2, fy2, fx3, fy3;

	UPROPERTY(EditAnywhere)
		float mass = 200;
	UPROPERTY(EditAnywhere)
		float radius = 60;
	UPROPERTY(EditAnywhere)
		float A = 2000;
	UPROPERTY(EditAnywhere)
		float B = 8.89;
	UPROPERTY(EditAnywhere)
		float AWall = 7000;
	UPROPERTY(EditAnywhere)
		float vDes = 2;
	UPROPERTY(EditAnywhere)
		float tau = 0.5;
	UPROPERTY(EditAnywhere)
		float hue = 90;

	// Target stuff;
	UPROPERTY(EditAnywhere)
		int targNr = 0;

	// Extras
	UPROPERTY(EditAnywhere)
		float RotLerpSpeed = .05f;
	UPROPERTY(EditAnywhere)
		float AnimSpeed = 50;
	UPROPERTY(EditAnywhere)
		float WallForce = 1000;
	UPROPERTY(EditAnywhere)
		float MaxWallForce = 2000;
	UPROPERTY(EditAnywhere)
		float WallForceMultiplier = 30;
	UPROPERTY(EditAnywhere)
		float DistToWall = 100;

	// Debug force arrow switches
	bool bDrawTargetForce = false;
	bool bDrawWallForce = true;
	bool bDrawAgentAgentForce = false;
	bool bDrawTotalForce = true;
	bool bDrawObstacleForce;

protected:
	virtual void BeginPlay() override;
};

