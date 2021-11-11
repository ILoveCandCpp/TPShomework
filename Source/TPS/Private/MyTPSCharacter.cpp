// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTPSCharacter.h"

#include "GunSkeletalMeshComponent.h"
#include "TPSProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyTPSCharacter::AMyTPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//实例化你的类组件

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
	
	Gun = CreateDefaultSubobject<UGunSkeletalMeshComponent>(TEXT("Gun"));
	if (Gun != nullptr)
	{
		// UE_LOG(LogTemp,Error, TEXT("12121111"));
		// UGunSkeletalMeshComponent* MyGun = Cast<UGunSkeletalMeshComponent>(Gun);
		auto AssetGunMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
		if(AssetGunMesh.Succeeded())
		{
			Gun->SetSkeletalMesh(AssetGunMesh.Object);
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("index_03_r_socket"));
		}
	}
	else
	{
		UE_LOG(LogTemp,Error, TEXT("1dasd2121111"));
	}
}

// Called when the game starts or when spawned
void AMyTPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	if (ProjectileClass)
	{
		// 获取摄像机变换
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		// UE_LOG(LogTemp, Error, TEXT("FIRE2"));
		// 设置MuzzleOffset，在略靠近摄像机前生成发射物。
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// 将MuzzleOffset从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// 使目标方向略向上倾斜。
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 在枪口位置生成发射物。
			ATPSProjectile* Projectile = World->SpawnActor<ATPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 设置发射物的初始轨迹。
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
		// isFire = false;
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

