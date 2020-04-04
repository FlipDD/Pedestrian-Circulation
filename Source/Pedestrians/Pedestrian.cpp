// Fill out your copyright notice in the Description page of Project Settings.

#include "Pedestrian.h"

// Necessary includes
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
#include "PedestrianAnimInstance.h"

// Macros
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

//////////////////////////////////////////////////////////////////////////
// APedestriansCharacter

APedestrian::APedestrian()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	// Store animation instance
	PedestrianAnim = Cast<UPedestrianAnimInstance>(Mesh->GetAnimInstance());

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
	{
		// Don't add this actor
		if (AAgents[i] != this)
			Agents.Add(Cast<APedestrian>(AAgents[i]));
	}
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

	// Set initial variables to use
	int i;
	APedestrian* Other;
	AObstacle* Obstacle;
	float DistToTarget;

	// Set forces to 0
	fX = 0;
	fY = 0;

	// Get current target
	ATarget* Target = Targets[targNr];

	// First calculate the force on the agent by any target ===============================
	FVector TargetLoc = Target->GetActorLocation();
	FVector MyLoc = GetActorLocation();
	nX = TargetLoc.X - MyLoc.X;
	nY = TargetLoc.Y - MyLoc.Y;

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

	// Now deal with the walls ============================================================
	// Raycast against the wall
	// Define the necessary variables for the raycast
	// HitInfo where we store the location the raycast hit
	FHitResult HitInfo;

	// The objects to trace
	TEnumAsByte<EObjectTypeQuery> ObjectToTrace = EObjectTypeQuery::ObjectTypeQuery1;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTraceAsByte;
	ObjectsToTraceAsByte.Add(ObjectToTrace);
	// Actors to ignore
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// The line trace function
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 1000,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitInfo,
		true
	);
	
	// If we hit an actor
	if (HitInfo.Actor != nullptr)
	{
		auto WallHit = Cast<AWallu>(HitInfo.Actor);
		// If the actor we hit was a wall
		if (WallHit)
		{
			// Get the scale and location of the wall we hit
			FVector WLoc = WallHit->GetActorLocation();
			FVector WScale = WallHit->GetActorScale3D();

			// Store the location the raycast hit
			FVector HitLocation = HitInfo.Location;
			FVector Normal = HitInfo.Normal;

			// Get distance to hit location
			dist = FVector::Dist(HitLocation /*+ Normal * 25*/, MyLoc);

			nY = HitInfo.Normal.Y;
			nX = HitInfo.Normal.X;

			// Calculate the magnitude of the force
			fMag = AWall * UKismetMathLibrary::Exp((radius - (dist/3)) / B);

			// Add to total force
			fY += fMag * nY;
			fy1 = fMag * nY;
			fX += fMag * nX;
			fx1 = fMag * nX;

			// Cap values
			if (fX < -MaxWallForce) fX = -MaxWallForce;
			if (fX > MaxWallForce) fX = MaxWallForce;
			if (fY < -MaxWallForce) fY = -MaxWallForce;
			if (fY > MaxWallForce) fY = MaxWallForce;
		}
	}

	// Look for agent-agent interactions =================================================
	i = 0;
	// Loop over the agents ArrayList and get each "other" agent in turn
	while (i < Agents.Num())
	{
		Other = Agents[i];
		FVector OLoc = Other->GetActorLocation();

		// Get the distance between the agents
		dist = FVector::Dist(MyLoc, OLoc);

		// Get vector between agents   
		nX = MyLoc.X - OLoc.X;
		nY = MyLoc.Y - OLoc.Y;

		// Normalize vector
		nX = nX / dist;
		nY = nY / dist;

		//Sum the agents radius
		float sumR = radius + Other->radius;

		// Calculate force components and add to total force
		double fMag = A * UKismetMathLibrary::Exp((sumR - dist) / B);
		fX += (float)fMag*nX;
		fY += (float)fMag*nY;
		fx2 = (float)fMag*nX;
		fy2 = (float)fMag*nY;

		i++;
	}

	// Deal with the obstacles =======================================================
	i = 0;
	while (i < Obstacles.Num())
	{
		Obstacle = Obstacles[i];
		FVector ObLoc = Obstacle->GetActorLocation();
		dist = FVector::Dist(MyLoc, ObLoc);//UKismetMathLibrary::Sqrt((MyLoc.X - ObLoc.X)*(MyLoc.X - ObLoc.X) + (MyLoc.Y - ObLoc.Y)*(MyLoc.Y - ObLoc.Y));

		// Get vector between  
		nX = MyLoc.X - ObLoc.X;
		nY = MyLoc.Y - ObLoc.Y;

		// Normalize vector
		nX = nX / dist;
		nY = nY / dist;

		//Sum the a radius
		float sumR = radius + Obstacle->Radius;

		// Calculate force components
		double fMag = A * UKismetMathLibrary::Exp((sumR - dist) / B);
		fX += (float)fMag*nX;
		fY += (float)fMag*nY;
		fx2 = (float)fMag*nX;
		fy2 = (float)fMag*nY;

		i++;
	} 

	// Update the dynamics ====================================================================
	// Calculate acceleration
	aX = fX / mass;
	aY = fY / mass;

	// Calculate velocity
	vX += aX * DeltaTime;
	vY += aY * DeltaTime;

	// Move the actor based on the velocity
	//AddActorLocalOffset

	AddActorWorldOffset(FVector(vX * 100 * DeltaTime, vY * 100 * DeltaTime, 0));

	// Set the speed of the animation based on the velocity
	float NewPos = (MyLoc - GetActorLocation()).Size();
	PedestrianAnim->Speed = NewPos * AnimSpeed;

	// Smoothly rotate based on the direction of the change to position
	FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(MyLoc, GetActorLocation());
	FRotator LerpedNewRot = UKismetMathLibrary::RLerp(GetActorRotation(), NewRot, RotLerpSpeed, true);
	SetActorRotation(LerpedNewRot);

	// Next target
	DistToTarget = FVector::Dist(MyLoc, TargetLoc);

	// If we are close enough to our target
	if (DistToTarget <= 5 * radius)
	{
		// If we aren't already at the last target...
		if (targNr < Targets.Num() - 1)
		{
			// Change target
			targNr++;

			// Set forces to zero
			fX = 0;
			fY = 0;
		}
	}
}