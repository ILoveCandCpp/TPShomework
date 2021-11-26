// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootTarget.h"

#include "TPSGameStateBase.h"
#include "TPSProjectile.h"

// Sets default values
AShootTarget::AShootTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShootTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShootTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShootTarget::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if(Cast<ATPSProjectile>(Other))
	{
		ATPSGameStateBase *GS = Cast<ATPSGameStateBase>(GetWorld()->GetGameState());
		GS->Socre++;
		if(GS->Socre >= 10)
		{
			GS->bIsVictory = true;
		}
		Other->Destroy();
	}
}

