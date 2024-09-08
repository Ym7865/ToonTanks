// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	//Spring Arm Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	//Camera Component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Binds the MoveForward function to the MoveForward axis mapping
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	//Binds the turn function to the turn axis mapping
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankplayerController = Cast<APlayerController>(GetController());

}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (TankplayerController)
	{
		FHitResult HitResult;
		TankplayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult);

		RotateTurret(HitResult.ImpactPoint);
	
	}
}

void ATank::Move(float Value)
{
	//Allows the Tank to move forwards and backwards
	FVector DeltaLocation = FVector::ZeroVector;
	//x = value * deltaTime * speed
	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	//Rotates the tank left or right
	FRotator DeltaRotation = FRotator::ZeroRotator;
	//Yaw = value * turn rate * deltatime
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}
