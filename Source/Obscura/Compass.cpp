// Fill out your copyright notice in the Description page of Project Settings.

#include "Obscura.h"
#include "Sun.h"
#include "Compass.h"

// Sets default values
ACompass::ACompass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currPosition = BB;
	compassPosition = 4;
	fromPosition = 4;
	isCompassOn = false;
	isDirty = false;
}

// Called when the game starts or when spawned
void ACompass::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("compass on"));

	// Set up the array of path flags
	paths[0] = &path01;
	paths[1] = &path12;
	paths[2] = &path23;
	paths[3] = &path34;
	paths[4] = &path45;
	paths[5] = &path56;
	paths[6] = &path67;
	paths[7] = &path70;
	overhead_paths[0] = &path80;
	overhead_paths[1] = &path81;
	overhead_paths[2] = &path82;
	overhead_paths[3] = &path83;
	overhead_paths[4] = &path84;
	overhead_paths[5] = &path85;
	overhead_paths[6] = &path86;
	overhead_paths[7] = &path87;


	for (int i = 0; i < 8; ++i)
	{
		*(paths[i]) = 0;
		*(overhead_paths[i]) = 0;
	}

	Super::BeginPlay();
}

// Called every frame
void ACompass::Tick( float DeltaTime )
{
	updateFromPosition();
	setPathFlags();
	Super::Tick( DeltaTime );
}

void ACompass::setPathFlags()
{
	// Zero all paths.
	for (int i = 0; i < 8; ++i)
	{
		*(paths[i]) = 0;
		*(overhead_paths[i]) = 0;
	}

	if (compassPosition == fromPosition)
	{
		return;
	}

	// Sun overhead cases.
	if (fromPosition == 8)
	{
		*(overhead_paths[compassPosition]) = 1;
		return;
	}
	if (compassPosition == 8)
	{
		*(overhead_paths[fromPosition]) = 1;
		return;
	}


	int diff = 0;
	if (compassPosition > fromPosition)
	{
		diff = compassPosition - fromPosition;
	}
	else
	{
		diff = 8 - (fromPosition - compassPosition);
	}
	//UE_LOG(LogTemp, Warning, TEXT("diff is %f"), diff);

	for (int i = 0; i < diff; ++i)
	{
		int tmp = (fromPosition + i) % 8;
		*(paths[tmp]) = 1;
	}
	//UE_LOG(LogTemp, Warning, TEXT("path01 is %f"), paths[0]);
}

void ACompass::updateFromPosition()
{
	FRotator sunRot = sun->GetActorRotation();

	fromPosition = sun->sunPos;
	if (!sun->moving) {
		fromPosition = compassPosition;
	}
}


void ACompass::setCompassPosition(float x, float y) {

	y *= -1;
	if (isCompassOn) {
		if (fabs(x) < 0.001f && fabs(y) < 0.001f) {
			currPosition = MM;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is MM"));
		}
		else if (fabs(x) < 0.1f && y > 0.5f) {
			currPosition = TT;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is TT"));
		}
		else if (x > 0.5f && y > 0.5f) {
			currPosition = TR;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is TR"));
		}
		else if (x > 0.5f && fabs(y) < 0.1f) {
			currPosition = RR;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is RR"));
		}
		else if (x > 0.5f && y < -0.5f) {
			currPosition = BR;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is BR"));
		}
		else if (fabs(x) < 0.1f && y < -0.5f) {
			currPosition = BB;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is BB"));
		}
		else if (x < -0.5f && y < -0.5f) {
			currPosition = BL;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is BL"));
		}
		else if (x < -0.5f && fabs(y) < 0.1f) {
			currPosition = LL;
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is LL"));
		}
		else if (x < -0.5f && y > 0.5f) {
			currPosition = TL; 
			//UE_LOG(LogTemp, Warning, TEXT("compass pos is TL"));
		}
		
		// Correct for compass rotation
		if (currPosition != 8) {
			if (octant_offset <= 0) {
				currPosition = (ACompass::CompassPositions)((currPosition + abs(octant_offset)) % 8);
			}
			else {
				currPosition = (ACompass::CompassPositions)((currPosition + (8 - octant_offset)) % 8);
			}
			UE_LOG(LogTemp, Warning, TEXT("offset is %f"), octant_offset);
			UE_LOG(LogTemp, Warning, TEXT("new position is %f"), (int)currPosition);
		}

		compassPosition = (int)currPosition;
	}
	else if (isDirty) {
		sun->UpdateSun((int)currPosition);
		isDirty = false;
	}
}

ACompass::CompassPositions ACompass::getCompassPosition() {
	return currPosition;
}