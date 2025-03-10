// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Obscura.h"
#include "Compass.h"
#include "ObscuraCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AObscuraCharacter

AObscuraCharacter::AObscuraCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Initially in shadow
	isInSun = false;
	damageTime = 0.0f;
	speedFactor = 1.0f;

	pillarMode = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AObscuraCharacter::BeginPlay() {
	charZ = GetActorTransform().GetLocation().Z;
	spawnPoint = GetActorTransform();
	//UE_LOG(LogTemp, Warning, TEXT("spawnPoint: %f, %f, %f"), spawnPoint.GetLocation().X, spawnPoint.GetLocation().Y, spawnPoint.GetLocation().Z);

	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AObscuraCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AObscuraCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AObscuraCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AObscuraCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AObscuraCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AObscuraCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AObscuraCharacter::TouchStopped);

	InputComponent->BindAction("ToggleCompass", IE_Pressed, this, &AObscuraCharacter::CompassOn);
	InputComponent->BindAction("ToggleCompass", IE_Released, this, &AObscuraCharacter::CompassOff);
	InputComponent->BindAction("CompassOverhead", IE_Pressed, this, &AObscuraCharacter::SetCompassOverhead);

	InputComponent->BindAction("TogglePillar", IE_Pressed, this, &AObscuraCharacter::PillarOn);
	InputComponent->BindAction("TogglePillar", IE_Released, this, &AObscuraCharacter::PillarOff);
}

void AObscuraCharacter::CompassOn() {
	compass->isCompassOn = true;
	UE_LOG(LogTemp, Warning, TEXT("compass on"));
}

void AObscuraCharacter::CompassOff() {
	compass->isCompassOn = false;
	compass->isDirty = true;
	UE_LOG(LogTemp, Warning, TEXT("compass off"));
}

void AObscuraCharacter::PillarOn() {
	pillarMode = true;
	UE_LOG(LogTemp, Warning, TEXT("pillar on"));
}

void AObscuraCharacter::PillarOff() {
	pillarMode = false;
	UE_LOG(LogTemp, Warning, TEXT("pillar off"));
}

void AObscuraCharacter::updateInSun() {
	//hit info
	FHitResult RV_Hit(ForceInit);

	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + (sun->GetActorRotation().Vector() * -9999);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	ECollisionChannel trace_channel = ECC_Visibility;
	bool hit = GetWorld()->LineTraceSingleByChannel(RV_Hit, start, end, trace_channel);

	isInSun = !hit;
}

float prevTime = 0.0f;
void AObscuraCharacter::updatePlayerDamage() {
	float currTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	float timeDiff = currTime - prevTime;
	if (isInSun) {
		damageTime += timeDiff;
	}
	else {
		damageTime -= timeDiff;
	}

	if (damageTime > 2.0f) {
		SetActorTransform(spawnPoint);
		damageTime = 0.0f;
	}
	else if (damageTime < 0.0f) {
		damageTime = 0.0f;
	}
	prevTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	speedFactor = damageTime + 1.0f;
}

void AObscuraCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AObscuraCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AObscuraCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (compass->isCompassOn) {
		xInput = FMath::Clamp(Rate, -1.0f, 1.0f);
	}
	else {
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AObscuraCharacter::LookUpAtRate(float Rate)
{
	if (compass->isCompassOn) {
		yInput = FMath::Clamp(Rate, -1.0f, 1.0f);
	}
	else {
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AObscuraCharacter::SetCompassOverhead()
{
	if (compass->isCompassOn) {
		compass->setCompassPosition(0, 0);
	}
}

void AObscuraCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value*0.75f/speedFactor);
	}
}

void AObscuraCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value*.75f/speedFactor);
	}
}

// Called every frame
void AObscuraCharacter::Tick(float DeltaTime)
{
	if ((compass->isCompassOn && (fabs(xInput) > 0.1f || fabs(yInput) > 0.1f)) || compass->isDirty) {
		//UE_LOG(LogTemp, Warning, TEXT("axis inputs are %f %f"), xInput, yInput);
		compass->setCompassPosition(xInput, yInput);
	}
	updateInSun();
	updatePlayerDamage();
	Super::Tick(DeltaTime);

}
