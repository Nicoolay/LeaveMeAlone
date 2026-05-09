// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Player/LMADefaultCharacter1.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
ALMADefaultCharacter1::ALMADefaultCharacter1() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

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
}

// Called when the game starts or when spawned
void ALMADefaultCharacter1::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ALMADefaultCharacter1::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void ALMADefaultCharacter1::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}
