// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTPSCharacter.h"

#include "GunSkeletalMeshComponent.h"
#include "TPSProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyTPSCharacter::AMyTPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//实例化你的类组件
	TP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	// TP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	// TP_Gun->bCastDynamicShadow = false;
	// TP_Gun->CastShadow = false;
	TP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(TP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	//设置角色网格体变换的位置和旋转

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	//将你的类组件附加到默认角色的骨骼网格体组件。

	SpringArmComp->SetupAttachment(GetMesh());

	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);

	//设置弹簧臂的类变量

	SpringArmComp->bUsePawnControlRotation = true;

	//设置角色动作组件的类变量

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->bIgnoreBaseRotation = true;
	
}

// Called when the game starts or when spawned
void AMyTPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	TP_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("index_03_r_socket"));
	
}

void AMyTPSCharacter::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// 找出向前方向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取向前矢量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyTPSCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// 找出向右方向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取向右矢量 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 添加该方向动作
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyTPSCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void AMyTPSCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMyTPSCharacter::BeginCrouch()
{
	Crouch();
}

void AMyTPSCharacter::EndCrouch()
{
	UnCrouch();
}

void AMyTPSCharacter::Jump()
{
	bIsJumping = true;
	Super::Jump();
}

void AMyTPSCharacter::StopJumping()
{
	Super::StopJumping();
	bIsJumping = false;
}

void AMyTPSCharacter::Fire()
{
	// 试图发射发射物。
	// UE_LOG(LogTemp, Error, TEXT("FIRE"));
	isFire = true;
	if (ProjectileClass && bHasGun)
	{
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
		
		UWorld* World = GetWorld();
		if (World)
		{
			
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ATPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
		// isFire = false;

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("hello"));
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("dsajkdhajslkdhask"));
				AnimInstance->Montage_Play(FireAnimation, 0.5f);
			}
		}
	}
	
}

void AMyTPSCharacter::StopFire()
{
	isFire = false;
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

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyTPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyTPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMyTPSCharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&AMyTPSCharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&AMyTPSCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch",IE_Released,this,&AMyTPSCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AMyTPSCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AMyTPSCharacter::EndSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyTPSCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyTPSCharacter::StopFire);
}

