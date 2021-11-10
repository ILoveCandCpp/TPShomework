// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTPSCharacter.h"

// Sets default values
AMyTPSCharacter::AMyTPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

