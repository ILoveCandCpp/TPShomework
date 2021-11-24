// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TPSGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATPSGameStateBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Socre = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsVictory = false;

protected:
	void Tick(float DeltaSeconds) override;
};
