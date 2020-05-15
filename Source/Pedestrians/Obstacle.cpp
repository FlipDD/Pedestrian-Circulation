// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the mesh, attach it and set scale depending on the radius
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetWorldScale3D(FVector(Radius, Radius, Radius));
}