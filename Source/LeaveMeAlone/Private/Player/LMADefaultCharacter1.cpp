// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Player/LMADefaultCharacter1.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Components/LMAHealthComponent.h"
#include "Components/LMAWeaponComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALMADefaultCharacter1::ALMADefaultCharacter1() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
  SpringArmComponent->SetupAttachment(GetRootComponent());
  SpringArmComponent->SetUsingAbsoluteRotation(
      true); // when character rotates it block camera rotation
  SpringArmComponent->TargetArmLength = ArmLength;
  SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
  SpringArmComponent->bDoCollisionTest =
      false; // Prohibits the camera from rotating around the camera arm
  SpringArmComponent->bEnableCameraLag = true;

  CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
  CameraComponent->SetupAttachment(SpringArmComponent);
  CameraComponent->SetFieldOfView(FOV);
  CameraComponent->bUsePawnControlRotation = false;
  HealthComponent =
      CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
  WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
}

// Called to bind functionality to input
void ALMADefaultCharacter1::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
  PlayerInputComponent->BindAxis("MoveForward", this,
                                 &ALMADefaultCharacter1::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this,
                                 &ALMADefaultCharacter1::MoveRight);
  PlayerInputComponent->BindAxis("Zoom", this, &ALMADefaultCharacter1::Zoom);
  PlayerInputComponent->BindAction("Sprint", IE_Pressed, this,
                                   &ALMADefaultCharacter1::BeginSprint);
  PlayerInputComponent->BindAction("Sprint", IE_Released, this,
                                   &ALMADefaultCharacter1::EndSprint);
  PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent,
                                   &ULMAWeaponComponent::Fire);
  PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent,
                                   &ULMAWeaponComponent::NoFire);
  PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent,
                                   &ULMAWeaponComponent::Reload);
}

// Called when the game starts or when spawned
void ALMADefaultCharacter1::BeginPlay() {
  Super::BeginPlay();

  if (CursorMaterial) {
    CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(
        GetWorld(), CursorMaterial, CursorSize, FVector(0));
  }

  OnHealthChanged(HealthComponent->GetHealth());
  HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter1::OnDeath);
  HealthComponent->OnHealthChanged.AddUObject(
      this, &ALMADefaultCharacter1::OnHealthChanged);
}

// Called every frame
void ALMADefaultCharacter1::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  	ControlStamina();

  if (!(HealthComponent->IsDead())) {
    RotationPlayerOnCursor();
  }
}

void ALMADefaultCharacter1::MoveForward(float Value) {
  AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter1::MoveRight(float Value) {
  AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter1::Zoom(float AxisValue) {
  if (SpringArmComponent && AxisValue != 0.0f) {
    float NewLength = SpringArmComponent->TargetArmLength;

    NewLength -= AxisValue * ZoomSpeed;
    NewLength = FMath::Clamp(NewLength, MinZoomLength, MaxZoomLength);
    SpringArmComponent->TargetArmLength = NewLength;
  }
}

void ALMADefaultCharacter1::OnDeath() {
  CurrentCursor->DestroyRenderState_Concurrent();

  PlayAnimMontage(DeathMontage);

  GetCharacterMovement()->DisableMovement();

  SetLifeSpan(5.0f);

  if (Controller) {
    Controller->ChangeState(NAME_Spectating);
  }
}

void ALMADefaultCharacter1::RotationPlayerOnCursor() {
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PC) {
    FHitResult ResultHit;
    PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
    float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(
                                     GetActorLocation(), ResultHit.Location)
                                     .Yaw;
    SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
    if (CurrentCursor) {
      CurrentCursor->SetWorldLocation(ResultHit.Location);
    }
  }
}

void ALMADefaultCharacter1::OnHealthChanged(float NewHealth) {
  GEngine->AddOnScreenDebugMessage(
      -1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

bool ALMADefaultCharacter1::IsSprinting() const { return bIsSprinting; }

void ALMADefaultCharacter1::BeginSprint() {
  if (Stamina >= 50 && IsMovingForward()) {

    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = 900.0f;
    if (EnableLog) {
      GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
                                       TEXT("SprintEnd"));
    }
  }
}
void ALMADefaultCharacter1::EndSprint() {
  bIsSprinting = false;
  GetCharacterMovement()->MaxWalkSpeed = 300.0f;
  if (EnableLog) {
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("SprintEnd"));
  }
}

void ALMADefaultCharacter1::ControlStamina() {
  if (bIsSprinting) {
    Stamina = Stamina - SprintCost;
    if (EnableLog) {
      GEngine->AddOnScreenDebugMessage(
          -1, 2.0f, FColor::Red, FString::Printf(TEXT("Sprint = %f"), Stamina));
    }
    if (Stamina <= 0) {
      EndSprint();
    }
  } else {
    Stamina = Stamina + StaminaRecoveryRate / 10;
    Stamina = FMath ::Min(Stamina, MaxStamina);
    if (EnableLog) {
      GEngine->AddOnScreenDebugMessage(
          -1, 2.0f, FColor::Red,
          FString::Printf(TEXT("RecoveryStamina = %f"), Stamina));
    }
  }
}

bool ALMADefaultCharacter1::IsMovingForward() {
  FVector Velocity = GetVelocity();
  FVector ForwardVector = GetActorForwardVector();

  return FVector::DotProduct(Velocity, ForwardVector) > 0;
}