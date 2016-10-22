// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Compass.generated.h"

UCLASS()
class OBSCURA_API ACompass : public AActor
{
	GENERATED_BODY()
	
public:	
	enum CompassPositions {
		TT,
		TR,
		RR,
		BR,
		BB,
		BL,
		LL,
		TL,
		MM
	};

	CompassPositions currPosition;

	// Sets default values for this actor's properties
	ACompass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	CompassPositions getCompassPosition();
};
