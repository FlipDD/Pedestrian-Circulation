// Fill out your copyright notice in the Description page of Project Settings.


#include "Wallu.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AWallu::AWallu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWallu::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

