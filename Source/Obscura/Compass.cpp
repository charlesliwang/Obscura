// Fill out your copyright notice in the Description page of Project Settings.

#include "Obscura.h"
#include "Compass.h"

// Sets default values
ACompass::ACompass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currPosition = MM;
}

// Called when the game starts or when spawned
void ACompass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACompass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

ACompass::CompassPositions ACompass::getCompassPosition() {
	return currPosition;
}

