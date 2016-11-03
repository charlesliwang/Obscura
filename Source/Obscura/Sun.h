// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Sun.generated.h"

UCLASS()
class OBSCURA_API ASun : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ASun();

	UPROPERTY(EditAnywhere)
	class ACompass* compass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ASun::UpdateSun(int octant);

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	float targetAngle;
	float targetPitch;
	bool rotateLock;
	bool inputLock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int sunPos;
};
