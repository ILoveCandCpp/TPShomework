// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

UCLASS(minimalapi)
class ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPSGameMode();

	/** 移除当前菜单控件，并在指定类（如有）中新建控件。*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	/** 游戏开始时，用作菜单的控件类。*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	
protected:
	/** 游戏开始时调用。*/
	virtual void BeginPlay() override;

	/** 用作菜单的控件实例。*/
	UPROPERTY()
	UUserWidget* CurrentWidget;
};



