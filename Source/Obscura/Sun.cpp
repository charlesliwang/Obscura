// Fill out your copyright notice in the Description page of Project Settings.

#include "Obscura.h"
#include "Compass.h"
#include "Sun.h"


// Sets default values
ASun::ASun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASun::Tick( float DeltaTime )
{
	FRotator sunRotation = GetActorRotation();
	sunRotation.Yaw++;
	SetActorRotation(sunRotation);
	Super::Tick( DeltaTime );
}

