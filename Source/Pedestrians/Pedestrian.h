// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pedestrian.generated.h"

UCLASS(config = Game)
class APedestrian : public AActor
{
	GENERATED_BODY()

	// The mesh of the pedestrian
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USkeletalMeshComponent* Mesh;
	// The capsule collider
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCapsuleComponent* Capsule;
public:
	APedestrian();

	virtual void Tick(float DeltaTime) override;

	// Reference to the pedestrian animation instance
	class UPedestrianAnimInstance* PedestrianAnim;

	// Arrays to store the agents, targets, obstacles and walls
	TArray<APedestrian*> Agents;
	TArray<class ATarget*> Targets;
	TArray<class AObstacle*> Obstacles;
	TArray<class AWallu*> Walls;

	// Variables
	UPROPERTY(BlueprintReadOnly)
	float fMag = 0;
	UPROPERTY(BlueprintReadOnly)
	float vX = 0;
	UPROPERTY(BlueprintReadOnly)
	float vY = 0;
	UPROPERTY(BlueprintReadOnly)
	float aX = 0;
	UPROPERTY(BlueprintReadOnly)
	float aY = 0;
	UPROPERTY(BlueprintReadOnly)
	float fX = 0;
	UPROPERTY(BlueprintReadOnly)
	float fY = 0;
	UPROPERTY(BlueprintReadOnly)
	float nX = 0;
	UPROPERTY(BlueprintReadOnly)
	float nY = 0;
	UPROPERTY(BlueprintReadOnly)
	float dist = 0;	
	float fx1, fy1, fx2, fy2, fx3, fy3;

	// Constants
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float mass = 200;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float radius = 25;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float A = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float B = 8.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float vDes = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float tau = 0.5;

	// Max force the wall can apply to the pedestrian
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxWallForce = 2000;

	// Current target;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int targNr = 0;

	// Animation constants
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float RotLerpSpeed = .05f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AnimSpeed = 50;

	// Debug force arrow switches
	//bool bDrawTargetForce = false;
	//bool bDrawWallForce = true;
	//bool bDrawAgentAgentForce = false;
	//bool bDrawTotalForce = true;
	//bool bDrawObstacleForce;

protected:
	virtual void BeginPlay() override;
};

