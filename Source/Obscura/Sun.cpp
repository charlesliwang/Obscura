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
	targetPitch = -45.0f;
	Super::BeginPlay();
}

void ASun::UpdateSun(int octant)
{
	// If sun overhead.
	if (octant == 8) {
		targetPitch = -90.0f;
		locked = true;
	}
	else {
		targetPitch = -45.0f;
		targetAngle = (octant * 45.0f) - 180.0f;
		// Move the sun directly to the correct position if it's coming from overhead.
		FRotator sunRotation = GetActorRotation();
		if (!locked && fabs(sunRotation.Pitch + 90) < 0.5f) {
			sunRotation.Yaw = targetAngle + 180.0f;
			SetActorRotation(sunRotation);
			UE_LOG(LogTemp, Warning, TEXT("cSun updating %f"), targetAngle);
		}

	}
	//UE_LOG(LogTemp, Warning, TEXT("cSun updating %f"), targetAngle);
}

// Called every frame
void ASun::Tick( float DeltaTime )
{
	FRotator sunRotation = GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("yaaaaaw %f"), sunRotation.Yaw);
	if (fabs(sunRotation.Pitch - targetPitch) > 0.6f) {
		if (sunRotation.Pitch > targetPitch)
			sunRotation.Pitch--;
		else
			sunRotation.Pitch++;
		SetActorRotation(sunRotation);
	}
	else {
		locked = false;
		if (fabs(sunRotation.Yaw - targetAngle) > 0.6f) {
			//UE_LOG(LogTemp, Warning, TEXT("sun differential"));
			sunRotation.Yaw += 0.5f;
			if (sunRotation.Yaw > 360)
				sunRotation.Yaw -= 360;
			SetActorRotation(sunRotation);
		}
	}
	Super::Tick( DeltaTime );
}

