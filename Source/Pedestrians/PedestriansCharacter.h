// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PedestriansCharacter.generated.h"

UCLASS(config=Game)
class APedestriansCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APedestriansCharacter();

	virtual void Tick(float DeltaTime) override;

	TArray<APedestriansCharacter*> Agents;
	TArray<class ATarget*> Targets;
	TArray<class AObstacle*> Obstacles;
	TArray<class AWallu*> Walls;

	float vX, vY;
	float aX, aY;
	float fX, fY;
	float mass = 80;
	float radius = 0.25;
	float A = 2000;
	float B = 0.08;
	float AWall = 2000;
	float vDes;
	float tau = 0.5;
	float hue;
	double fMag;
	float nX, nY, dist;

	float fx1, fy1, fx2, fy2, fx3, fy3;

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

