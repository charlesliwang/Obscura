// Fill out your copyright notice in the Description page of Project Settings.

#include "Obscura.h"
#include "Sun.h"
#include "Compass.h"

// Sets default values
ACompass::ACompass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currPosition = TT;
	isCompassOn = false;
	
}

// Called when the game starts or when spawned
void ACompass::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("compass on"));
	Super::BeginPlay();
	
}

// Called every frame
void ACompass::Tick( float DeltaTime )
{

	Super::Tick( DeltaTime );

}

void ACompass::setCompassPosition(float x, float y) {

	y *= -1;

	if (fabs(x) < 0.001f && fabs(y) < 0.001f) {
		currPosition = MM;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is MM"));
	}
	else if (fabs(x) < 0.1f && y > 0.5f) {
		currPosition = TT;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is TT"));
	}
	else if (x > 0.5f && y > 0.5f) {
		currPosition = TR;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is TR"));
	}
	else if (x > 0.5f && fabs(y) < 0.1f) {
		currPosition = RR;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is RR"));
	}
	else if (x > 0.5f && y < -0.5f) {
		currPosition = BR;
		 UE_LOG(LogTemp, Warning, TEXT("compass pos is BR"));
	}
	else if (fabs(x) < 0.1f && y < -0.5f) {
		currPosition = BB;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is BB"));
	}
	else if (x < -0.5f && y < -0.5f) {
		currPosition = BL;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is BL"));
	}
	else if (x < -0.5f && fabs(y) < 0.1f) {
		currPosition = LL;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is LL"));
	}
	else if (x < -0.5f && y > 0.5f) {
		currPosition = TL;
		UE_LOG(LogTemp, Warning, TEXT("compass pos is TL"));
	}
	sun->UpdateSun((int)currPosition);
}

ACompass::CompassPositions ACompass::getCompassPosition() {
	return currPosition;
}