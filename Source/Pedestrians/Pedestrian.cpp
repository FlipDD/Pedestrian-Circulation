// Fill out your copyright notice in the Description page of Project Settings.


#include "Pedestrian.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PedestriansCharacter.h"

// Needed includes
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Custom classes
#include "Obstacle.h"
#include "Target.h"
#include "Wallu.h"

#include "ArrowSceneComp.h"

// Macros
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
//////////////////////////////////////////////////////////////////////////
// APedestriansCharacter

APedestrian::APedestrian()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(RootComponent);
	Capsule->InitCapsuleSize(42.f, 96.f);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Capsule);

	Arrow = CreateDefaultSubobject<UArrowSceneComp>("Arrow");
	Arrow->SetupAttachment(Capsule);
}

// This will only run once at the start of the level (when pressing Play)
void APedestrian::BeginPlay()
{
	Super::BeginPlay();

	auto World = GetWorld();
	// Don't go any further if we can't find the world
	// Failing to finding will crash the Editor with the ensure
	// It should always find it, but we should always ensure to avoid crashes
	if (!ensureAlways(World))
		return;

	// Get all the agents, targets, walls and obstacles currently in the Level
	// Declare temporary arrays to fill
	TArray<AActor*> AAgents;
	TArray<AActor*> ATargets;
	TArray<AActor*> AWalls;
	TArray<AActor*> AObstacles;

	// Declare which classes to look for
	TSubclassOf<APedestrian> PedestrianClass = APedestrian::StaticClass();
	TSubclassOf<ATarget> TargetClass = ATarget::StaticClass();
	TSubclassOf<AWallu> WallClass = AWallu::StaticClass();
	TSubclassOf<AObstacle> ObstacleClass = AObstacle::StaticClass();

	// Get all actor of a specific class and add it to the declared arrays
	UGameplayStatics::GetAllActorsOfClass(World, PedestrianClass, AAgents);
	UGameplayStatics::GetAllActorsOfClass(World, TargetClass, ATargets);
	UGameplayStatics::GetAllActorsOfClass(World, WallClass, AWalls);
	UGameplayStatics::GetAllActorsOfClass(World, ObstacleClass, AObstacles);

	// Cast from actors to the specific type a populate the arrays of that type
	for (int i = 0; i < AAgents.Num(); i++)
		Agents.Add(Cast<APedestrian>(AAgents[i]));
	for (int i = 0; i < ATargets.Num(); i++)
		Targets.Add(Cast<ATarget>(ATargets[i]));
	for (int i = 0; i < AWalls.Num(); i++)
		Walls.Add(Cast<AWallu>(AWalls[i]));
	for (int i = 0; i < AObstacles.Num(); i++)
		Obstacles.Add(Cast<AObstacle>(AObstacles[i]));

}

// This will run every frame
void APedestrian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int i;
	APedestrian* Other;
	AWallu* Wall;
	AObstacle* Obstacle;
	float DistToTarget;

	fX = 0;
	fY = 0;
	// Get current target
	ATarget* Target = Targets[targNr];

	// First calculate the force on the agent by any target ===============================
	// Get distance to the target 
	// Get the vector to the target
	FVector TargetLoc = Target->GetActorLocation();
	FVector MyLoc = GetActorLocation();
	nX = TargetLoc.X - MyLoc.X;
	nY = TargetLoc.Y - MyLoc.Y;
	/*nX = UKismetMathLibrary::Abs(nX);
	nY = UKismetMathLibrary::Abs(nY);*/
	// Get distance to the target  
	DistToTarget = FVector::Dist(TargetLoc, MyLoc);//UKismetMathLibrary::Sqrt(nX * nX + nY * nY);
	// Normalize vector	
	nX = nX / DistToTarget;
	nY = nY / DistToTarget;
	// Calculate the force needed to achieve desired velocity vDes
	fX += (vDes*nX - vX)*mass / tau;
	fY += (vDes*nY - vY)*mass / tau;
	// TODO: add arrows
	fx3 = (vDes*nX - vX)*mass / tau;
	fy3 = (vDes*nY - vY)*mass / tau;
	//if(bDrawTargetForce) forceArrowTarget.draw(x,y,x+fx3/mass,y+fy3/mass,1,60); 
	//print("fTarg %s, fX = %f fY = %f target # = %d", *GetName(), fX, fY, targNr);
	//print(TEXT("fTarg %s"), *GetName());

	// Now deal with the walls ============================================================
	// First look for a horizontal wall
	i = 0;
	// Loop over the walls ArrayList 
	while (i < Walls.Num())
	{
		Wall = Walls[i];
		FVector WScale = Wall->GetActorScale3D();
		FVector WLoc = Wall->GetActorLocation();
		if (MyLoc.Y > (WLoc.Y - WScale.Y / 2) &&
			MyLoc.Y < (WLoc.Y + WScale.Y / 2))
		{
			dist = UKismetMathLibrary::Sqrt((MyLoc.X - WLoc.X) * (MyLoc.X - WLoc.X));
			//FVector::Dist(MyLoc, WLoc);

			nX = MyLoc.X - WLoc.X;
			nX = nX / dist;
			// Calculate the magnitude of the force
			fMag = AWall * UKismetMathLibrary::Exp((radius - dist) / B);
			// Add to total force
			fX += fMag * nX;
			fy1 = 0;
			fx1 = fMag * nX;
			//if (bDrawWallForce) forceArrowWall.draw(x, y, x + fx1, y + fy1, 1, 60);
		}
		// First make sure we are "along" the wall somewhere
		else if (MyLoc.X > (WLoc.X - WScale.X / 2) &&
			MyLoc.X < (WLoc.X + WScale.X / 2))
		{
			// Get distance to wall
			dist = UKismetMathLibrary::Sqrt((MyLoc.Y - WLoc.Y) * (MyLoc.Y - WLoc.Y));
			//FVector::Dist(MyLoc, WLoc)
					
			// Get vector to wall
			nY = MyLoc.Y - WLoc.Y;
			// Normalize vector
			nY = nY / dist;
			// Calculate the magnitude of the force
			fMag = AWall * UKismetMathLibrary::Exp((radius - dist) / B);
			// Add to total force
			fY += fMag * nY;
			fx1 = 0;
			fy1 = fMag * nY;
			//if (bDrawWallForce) forceArrowWall.draw(x, y, x + fx1, y + fy1, 1, 60);
		}
		i++;
	}

	//// Now Look for agent-agent interactions =================================================
	i = 0;
	// Loop over the agents ArrayList and get each "other" agent in turn
	while (i < Agents.Num()) {
		Other = Agents[i];
		FVector OLoc = Other->GetActorLocation();
		// Do not calculate force of this agent on itself
		if (this != Other) {
			// Now calc the force between this agent and the current other agent 
			// First get the distance between the agents
			dist = FVector::Dist(MyLoc, OLoc);//UKismetMathLibrary::Sqrt((MyLoc.X - OLoc.X)*(MyLoc.X - OLoc.X) + (MyLoc.Y - OLoc.Y)*(MyLoc.Y - OLoc.Y));
			// Get vector between agents   
			nX = MyLoc.X - OLoc.X;
			nY = MyLoc.Y - OLoc.Y;
			// Normalize vector
			nX = nX / dist;
			nY = nY / dist;
			//Sum the agents radii
			float sumR = radius + Other->radius;
			// Calculate force components and add to total force
			double fMag = A * UKismetMathLibrary::Exp((sumR - dist) / B);
			fX += (float)fMag*nX;
			fY += (float)fMag*nY;
			fx2 = (float)fMag*nX;
			fy2 = (float)fMag*nY;
			/*if (bDrawAgentAgentForce) forceArrowAgent.draw(x, y, x + fx2, y + fy2, 1, 60);
			println("fInter " + name + " fX = " + fx2 + " fY = " + fy2);
			println("dist = " + dist);*/
		} // if
		i++;
	} // while

  // Now deal with the obstacles =======================================================
	i = 0;
	while (i < Obstacles.Num()) {
		Obstacle = Obstacles[i];
		FVector ObLoc = Obstacle->GetActorLocation();
		dist = FVector::Dist(MyLoc, ObLoc);//UKismetMathLibrary::Sqrt((MyLoc.X - ObLoc.X)*(MyLoc.X - ObLoc.X) + (MyLoc.Y - ObLoc.Y)*(MyLoc.Y - ObLoc.Y));
		// Get vector between  
		nX = MyLoc.X - ObLoc.X;
		nY = MyLoc.Y - ObLoc.Y;
		// Normalize vector
		nX = nX / dist;
		nY = nY / dist;
		//Sum the a radii
		float sumR = radius + Obstacle->Radius;
		// Calculate force components
		double fMag = A * UKismetMathLibrary::Exp((sumR - dist) / B);
		fX += (float)fMag*nX;
		fY += (float)fMag*nY;
		fx2 = (float)fMag*nX;
		fy2 = (float)fMag*nY;
		//if (bDrawObstacleForce) forceArrowAgent.draw(x, y, x + fx2, y + fy2, 1, 60);
		/*print("Obstacle "+obst+" x = "+obst.x+" y = "+obst.y);
		print("dist = "+dist);
		print("R = "+sumR);
		print(" fX = "+fx2+" fY = "+fy2);      */

		i++;
	} // while

	//// Update the dynamics ====================================================================
	aX = fX / mass;
	aY = fY / mass;

	vX += aX * DeltaTime;
	vY += aY * DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("VX IS %f, VY IS %f"), vX, vY);

	//sSetActorLocation(FVector(MyLoc.X + vX, MyLoc.Y + vY, MyLoc.Z + DeltaTime));
	AddActorWorldOffset(FVector(vX * 100 * DeltaTime, vY * 100 * DeltaTime, 0));
	//print(TEXT("X is %f"), MyLoc.X);
	// MyLoc.X += vX * 100 * DeltaTime;
	// MyLoc.Y += vY * 100 * DeltaTime;

	//print(TEXT("%s, vX = %f, vY = %f"), *GetName(), vX, vY);
	// Wrap-around boundary conditions
	/*if (x < 0) x = widthPhysical;
	if (x > widthPhysical) x = 0;
	if (y < 0) this.y = heightPhysical;
	if (y > heightPhysical) this.y = 0;*/

	//// next target
	DistToTarget = FVector::Dist(MyLoc, TargetLoc);
	//DistToTarget = UKismetMathLibrary::Sqrt(MyLoc.X - TLoc.X)*(MyLoc.X - TLoc.X) + (MyLoc.Y - TLoc.Y)*(MyLoc.Y - TLoc.Y);
	if (DistToTarget <= 5 * radius)
	{
		if (targNr < Targets.Num() - 1)
		{
			targNr++;
			//println("****** Target changed to ***** " + targNr);
			// set forces to zero
			fX = 0;
			fY = 0;
		}
	}
}

//int i;
//APedestrian* Other;
//AWallu* Wall;
//AObstacle* Obstacle;
//float DistToTarget;
//
//fX = 0;
//fY = 0;
//// Get current target
//ATarget* Target = Targets[targNr];
//
//// First calculate the force on the agent by any target ===============================
//// Get distance to the target 
//// Get the vector to the target
//FVector TargetLoc = Target->GetActorLocation();
//FVector MyLoc = GetActorLocation();
//nX = TargetLoc.X - MyLoc.X;
//nY = TargetLoc.Y - MyLoc.Y;
//// Get distance to the target  
//DistToTarget = UKismetMathLibrary::Sqrt(nX * nX + nY * nY);
//// Normalize vector	
//nX = nX / DistToTarget;
//nY = nY / DistToTarget;
//// Calculate the force needed to achieve desired velocity vDes
//fX += (vDes*nX - vX)*mass / tau;
//fY += (vDes*nY - vY)*mass / tau;
//// TODO: add arrows
//fx3 = (vDes*nX - vX)*mass / tau;
//fy3 = (vDes*nY - vY)*mass / tau;
////if(bDrawTargetForce) forceArrowTarget.draw(x,y,x+fx3/mass,y+fy3/mass,1,60); 
////print("fTarg %s, fX = %f fY = %f target # = %d", *GetName(), fX, fY, targNr);
////print(TEXT("fTarg %s"), *GetName());
//
//// Now deal with the walls ============================================================
//// First look for a horizontal wall
//i = 0;
//// Loop over the walls ArrayList 
//while (i < Walls.Num())
//{
//	Wall = Walls[i];
//	FVector WScale = Wall->GetActorScale3D();
//	FVector WLoc = Wall->GetActorLocation();
//	if (MyLoc.Y > (WLoc.Y - WScale.Y / 2) &&
//		MyLoc.Y < (WLoc.Y + WScale.Y / 2))
//	{
//		dist = UKismetMathLibrary::Sqrt((MyLoc.X - WLoc.X) * (MyLoc.X - WLoc.X));
//		nX = MyLoc.X - WLoc.X;
//		nX = nX / dist;
//		// Calculate the magnitude of the force
//		fMag = AWall * UKismetMathLibrary::Exp((radius - dist) / B);
//		// Add to total force
//		fX += fMag * nX;
//		fy1 = 0;
//		fx1 = fMag * nX;
//		//if (bDrawWallForce) forceArrowWall.draw(x, y, x + fx1, y + fy1, 1, 60);
//	}
//	// First make sure we are "along" the wall somewhere
//	else if (MyLoc.X > (WLoc.X - WScale.X / 2) &&
//		MyLoc.X < (WLoc.X + WScale.X / 2))
//	{
//		// Get distance to wall
//		dist = UKismetMathLibrary::Sqrt((MyLoc.Y - WLoc.Y) * (MyLoc.Y - WLoc.Y));
//		// Get vector to wall
//		nY = MyLoc.Y - WLoc.Y;
//		// Normalize vector
//		nY = nY / dist;
//		// Calculate the magnitude of the force
//		fMag = AWall * UKismetMathLibrary::Exp((radius - dist) / B);
//		// Add to total force
//		fY += fMag * nY;
//		fx1 = 0;
//		fy1 = fMag * nY;
//		//if (bDrawWallForce) forceArrowWall.draw(x, y, x + fx1, y + fy1, 1, 60);
//	}
//	i++;
//}
//
////// Now Look for agent-agent interactions =================================================
//i = 0;
//// Loop over the agents ArrayList and get each "other" agent in turn
//while (i < Agents.Num()) {
//	Other = Agents[i];
//	FVector OLoc = Other->GetActorLocation();
//	// Do not calculate force of this agent on itself
//	if (this != Other) {
//		// Now calc the force between this agent and the current other agent 
//		// First get the distance between the agents
//		dist = UKismetMathLibrary::Sqrt((MyLoc.X - OLoc.X)*(MyLoc.X - OLoc.X) + (MyLoc.Y - OLoc.Y)*(MyLoc.Y - OLoc.Y));
//		// Get vector between agents   
//		nX = MyLoc.X - OLoc.X;
//		nY = MyLoc.Y - OLoc.Y;
//		// Normalize vector
//		nX = nX / dist;
//		nY = nY / dist;
//		//Sum the agents radii
//		float sumR = radius + Other->radius;
//		// Calculate force components and add to total force
//		double fMag = A * UKismetMathLibrary::Exp((sumR - dist) / B);
//		fX += (float)fMag*nX;
//		fY += (float)fMag*nY;
//		fx2 = (float)fMag*nX;
//		fy2 = (float)fMag*nY;
//		/*if (bDrawAgentAgentForce) forceArrowAgent.draw(x, y, x + fx2, y + fy2, 1, 60);
//		println("fInter " + name + " fX = " + fx2 + " fY = " + fy2);
//		println("dist = " + dist);*/
//	} // if
//	i++;
//} // while
//
//// Now deal with the obstacles =======================================================
//i = 0;
//while (i < Obstacles.Num()) {
//	Obstacle = Obstacles[i];
//	FVector ObLoc = Obstacle->GetActorLocation();
//	dist = UKismetMathLibrary::Sqrt((MyLoc.X - ObLoc.X)*(MyLoc.X - ObLoc.X) + (MyLoc.Y - ObLoc.Y)*(MyLoc.Y - ObLoc.Y));
//	// Get vector between  
//	nX = MyLoc.X - ObLoc.X;
//	nY = MyLoc.Y - ObLoc.Y;
//	// Normalize vector
//	nX = nX / dist;
//	nY = nY / dist;
//	//Sum the a radii
//	float sumR = radius + Obstacle->Radius;
//	// Calculate force components
//	double fMag = A * UKismetMathLibrary::Exp((sumR - dist) / B);
//	fX += (float)fMag*nX;
//	fY += (float)fMag*nY;
//	fx2 = (float)fMag*nX;
//	fy2 = (float)fMag*nY;
//	//if (bDrawObstacleForce) forceArrowAgent.draw(x, y, x + fx2, y + fy2, 1, 60);
//	/*print("Obstacle "+obst+" x = "+obst.x+" y = "+obst.y);
//	print("dist = "+dist);
//	print("R = "+sumR);
//	print(" fX = "+fx2+" fY = "+fy2);      */
//
//	i++;
//} // while
//
////// Update the dynamics ====================================================================
//aX = fX / mass;
//aY = fY / mass;
//
//vX += aX * 100 * DeltaTime;
//vY += aY * 100 * DeltaTime;
//UE_LOG(LogTemp, Warning, TEXT("VX IS %f, VY IS %f"), vX, vY);
//
//SetActorLocation(FVector(MyLoc.X + vX, MyLoc.Y + vY, MyLoc.Z + DeltaTime));
////AddActorWorldOffset(FVector(vX * 100 * DeltaTime, vY * 100 * DeltaTime, 0));
////print(TEXT("X is %f"), MyLoc.X);
//// MyLoc.X += vX * 100 * DeltaTime;
//// MyLoc.Y += vY * 100 * DeltaTime;
//
////print(TEXT("%s, vX = %f, vY = %f"), *GetName(), vX, vY);
//// Wrap-around boundary conditions
///*if (x < 0) x = widthPhysical;
//if (x > widthPhysical) x = 0;
//if (y < 0) this.y = heightPhysical;
//if (y > heightPhysical) this.y = 0;*/
//
////// next target
//FVector TLoc = Target->GetActorLocation();
//DistToTarget = UKismetMathLibrary::Sqrt(MyLoc.X - TLoc.X)*(MyLoc.X - TLoc.X) + (MyLoc.Y - TLoc.Y)*(MyLoc.Y - TLoc.Y);
//if (DistToTarget <= 20 * radius)
//{
//	if (targNr < Targets.Num() - 1)
//	{
//		targNr++;
//		//println("****** Target changed to ***** " + targNr);
//		// set forces to zero
//		fX = 0;
//		fY = 0;
//	}
//}
