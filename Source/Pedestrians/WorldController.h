// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldController.generated.h"

UCLASS()
class PEDESTRIANS_API AWorldController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldController();

	class AWallu* wall;
	class APedestriansCharacter* agent;
	class ATarget* targ;
	class AObstacle* obst;

	int i;

	bool showArrows = true; // Shows force and velocity arrows

	//// Establish the arena size, both in physical coordinates (metres)
	//// and in canvas pixels
	//int width, height; // canvas pixel coordinates
	//int widthPhysical = 10;   // arena width in metres
	//int heightPhysical = 5;   // arena height in metres
	//int scale = 100;          // Number of pixels per metre.

	//// definitions of walls
	//int WALL_VERT = 1;
	//int WALL_HORZ = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
