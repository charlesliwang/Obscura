// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Sun.h"
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
	bool isCompassOn;

	void ACompass::SetupPlayerInputComponent(class UInputComponent* InputComponent);

	// Sets default values for this actor's properties
	ACompass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void setCompassPosition(float x, float y);

	CompassPositions getCompassPosition();

	UPROPERTY(EditAnywhere)
	class ASun* sun;
};
