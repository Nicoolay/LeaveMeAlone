// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter1.generated.h"

class UCameraComponent;    // Camera
class USpringArmComponent; // Sort of camera manager
class ULMAHealthComponent; // Health
class ULMAWeaponComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter1 : public ACharacter {
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ALMADefaultCharacter1();

  UFUNCTION()
  ULMAHealthComponent *GetHealthComponent() const { return HealthComponent; }

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USpringArmComponent *SpringArmComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  UCameraComponent *CameraComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
  ULMAHealthComponent *HealthComponent;

  UPROPERTY()
  UDecalComponent *CurrentCursor = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
  UMaterialInterface *CursorMaterial = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
  FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

  UPROPERTY(EditDefaultsOnly, Category = "Animation")
  UAnimMontage *DeathMontage;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  ULMAWeaponComponent *WeaponComponent;

  UPROPERTY(EditAnywhere, Category = "Sprint")
  float Stamina = 100.0f;

  UPROPERTY(EditAnywhere, Category = "Sprint")
  float MaxStamina = 100.0f;

  UPROPERTY(EditAnywhere, Category = "Sprint")
  float SprintCost = 0.3f;

  UPROPERTY(EditAnywhere, Category = "Sprint")
  float StaminaRecoveryRate = 1.3f;

  UPROPERTY(BlueprintReadOnly, Category = "Sprint")
  bool bIsSprinting;

  UFUNCTION(BlueprintCallable)
  bool IsSprinting() const;

  UPROPERTY(EditAnywhere, Category = "Components")
  bool EnableLog = false;

  FTimerHandle TimerHandle;

  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent *PlayerInputComponent) override;

private:
  float YRotation = -75.0f;  // – defines camera rotation on Y
  float ArmLength = 1400.0f; //  defiens lenght camera arm
  float FOV = 55.0f;         // defines angle of camera view
  void MoveForward(float Value);
  void MoveRight(float Value);
  void Zoom(float AxisValue);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Zoom",
            meta = (AllowPrivateAccess = "true"))
  float ZoomSpeed = 200.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Zoom",
            meta = (AllowPrivateAccess = "true"))
  float MinZoomLength = 300.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Zoom",
            meta = (AllowPrivateAccess = "true"))
  float MaxZoomLength = 1400.0f;

  void OnDeath();

  void RotationPlayerOnCursor();

  void OnHealthChanged(float NewHealth);

  void BeginSprint();
  void EndSprint();
  void ControlStamina();
  bool IsMovingForward();
};
