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

void ASun::UpdateSun(int octant)
{
	// If sun overhead.
	if (octant == 8) {

	}
	else {
		targetAngle = octant * 45.0f;
	}
}

// Called every frame
void ASun::Tick( float DeltaTime )
{
	FRotator sunRotation = GetActorRotation();
	if (fabs(sunRotation.Yaw - targetAngle) > 0.1f) {
		//sunRotation.Yaw = (sunRotation.Yaw + 1) % 360;
		SetActorRotation(sunRotation);
	}
	Super::Tick( DeltaTime );
}

