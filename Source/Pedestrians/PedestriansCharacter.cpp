// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PedestriansCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Target.h"
#include "Wall.h"
#include "Obstacle.h"

//////////////////////////////////////////////////////////////////////////
// APedestriansCharacter

APedestriansCharacter::APedestriansCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

}

void APedestriansCharacter::BeginPlay()
{
	if (GetWorld())
	{
		TSubclassOf<APedestriansCharacter> PedestrianClass;
		PedestrianClass = APedestriansCharacter::StaticClass();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PedestrianClass, Agents);

		TSubclassOf<ATarget> TargetClass;
		TargetClass = ATarget::StaticClass();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetClass, Targets);

		TSubclassOf<AWall> WallClass;
		WallClass = AWall::StaticClass();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), WallClass, Walls);

		TSubclassOf<AObstacle> ObstacleClass;
		ObstacleClass = AObstacle::StaticClass();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObstacleClass, Obstacles);
	}
}

void APedestriansCharacter::Tick(float DeltaTime)
{
	int i;
	Agent other;
	Wall wall;
	Target target;
	float distToTarget;
	Obstacle obst;
}
