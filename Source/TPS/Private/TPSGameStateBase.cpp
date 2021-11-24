// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameStateBase.h"

ATPSGameStateBase::ATPSGameStateBase()
{
}

void ATPSGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Socre >= 10)
	{
		bIsVictory = true;
	}
}
