// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyTPSCharacter.generated.h"

UCLASS()
class TPS_API AMyTPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyTPSCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Health = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Energy = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Ammo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 OnceFiredAmmo = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 MaxAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isFire = false;
	
	// 枪口相对于摄像机位置的偏移。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bHasGun = true;

	// 要生成的发射物类。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class ATPSProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* TP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//跟随玩家背后摄像机的弹簧臂组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	//玩家跟随摄像机
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;
	
	//为向前/向后输入而调用
	void MoveForward(float InputAxis);

	//为左/右侧输入而调用
	void MoveRight(float InputAxis);
	
	//将角色移动速度设置为冲刺值。
	void BeginSprint();

	//将角色移动速度设置回默认速度值。
	void EndSprint();

	//角色蹲伏请求
	void BeginCrouch();

	//角色结束蹲伏请求
	void EndCrouch();

	//转向
	void TurnAtRate(float Rate);

	//向上转向
	void LookUpAtRate(float Rate);

	//角色跳
	void Jump() override;

	//角色停止跳
	void StopJumping() override;

	//开火
	void Fire();

	//停止开火
	void StopFire();

	/** 回复Ammo */
	UFUNCTION(BlueprintCallable)
	void ResumeAmmo();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
