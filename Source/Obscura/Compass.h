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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int compassPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int fromPosition;

	// All the path visibility flags
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path23;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path34;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path56;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path67;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path70;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path81;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path82;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path83;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path84;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path85;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path86;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int path87;

	int *paths[8];
	int *overhead_paths[8];

	bool isCompassOn;
	bool isDirty;

	void ACompass::SetupPlayerInputComponent(class UInputComponent* InputComponent);

	void ACompass::setPathFlags();
	void ACompass::updateFromPosition();

	// Sets default values for this actor's properties
	ACompass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void setCompassPosition(float x, float y);

	CompassPositions getCompassPosition();

	// Offset for the compass controls -- correcting for disparity in input to compass rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int octant_offset;


	UPROPERTY(EditAnywhere)
	class ASun* sun;
};
