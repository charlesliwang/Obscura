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
	rotateLock = false;
	inputLock = false;
	Super::BeginPlay();

}

void ASun::UpdateSun(int octant)
{
	// If sun overhead.
	if (octant == 8) {
		targetPitch = -90.0f;
		rotateLock = true;
	}
	else {
		if (!inputLock) {
			targetPitch = -45.0f;
			targetAngle = (octant * 45.0f) - 180.0f;
			// Move the sun directly to the correct position if it's coming from overhead.
			FRotator sunRotation = GetActorRotation();
			if (!inputLock && fabs(sunRotation.Pitch + 90) < 0.5f) {
				sunRotation.Yaw = targetAngle + 180.0f;
				// Octant == 0 gives wrong value in editor
				sunRotation.Roll = 0.0f;
				SetActorRotation(sunRotation);
				UE_LOG(LogTemp, Warning, TEXT("Sun update: targetAngle = %f, actualAngle = %f, octant = %d"), targetAngle, sunRotation.Yaw, octant);
			}
			if (fabs(sunRotation.Pitch + 45) < 0.5f) {
				rotateLock = false;
				UE_LOG(LogTemp, Warning, TEXT("locked is false"));
			}
		}
		// CHECKPOINT
	}
	//UE_LOG(LogTemp, Warning, TEXT("cSun updating %f"), targetAngle);
}

// Called every frame
void ASun::Tick( float DeltaTime )
{
	FRotator sunRotation = GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("yaaaaaw %f"), sunRotation.Yaw);
	if (fabs(sunRotation.Pitch - targetPitch) > 0.6f) {
		inputLock = true;
		if (sunRotation.Pitch > targetPitch)
			sunRotation.Pitch--;
		else
			sunRotation.Pitch++;
		SetActorRotation(sunRotation);
	}
	else {
		inputLock = false;
		if (!rotateLock && fabs(sunRotation.Yaw - targetAngle) > 0.6f) {
			//UE_LOG(LogTemp, Warning, TEXT("spinning!!!"));
			sunRotation.Yaw += 0.5f;
			if (sunRotation.Yaw > 360)
				sunRotation.Yaw -= 360;
			SetActorRotation(sunRotation);
		}
	}
	Super::Tick( DeltaTime );
}

