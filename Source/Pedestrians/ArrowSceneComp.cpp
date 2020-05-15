// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowSceneComp.h"

#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UArrowSceneComp::UArrowSceneComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	F1Mesh = CreateDefaultSubobject<UStaticMeshComponent>("F1Mesh");
	F1Mesh->SetupAttachment(GetAttachmentRoot());
	F2Mesh = CreateDefaultSubobject<UStaticMeshComponent>("F2Mesh");
	F2Mesh->SetupAttachment(GetAttachmentRoot());
	F3Mesh = CreateDefaultSubobject<UStaticMeshComponent>("F3Mesh");
	F3Mesh->SetupAttachment(GetAttachmentRoot());
	// ...
}

void UArrowSceneComp::SetF1Rot(FRotator Rot)
{
	F1Mesh->SetWorldRotation(Rot);
}

void UArrowSceneComp::SetF2Rot(FRotator Rot)
{
	F2Mesh->SetWorldRotation(Rot);
}

void UArrowSceneComp::SetF3Rot(FRotator Rot)
{
	F3Mesh->SetWorldRotation(Rot);
}