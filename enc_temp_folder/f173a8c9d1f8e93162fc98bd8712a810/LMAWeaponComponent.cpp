// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

void ULMAWeaponComponent::Fire() {
  UE_LOG(LogTemp, Warning, TEXT("Fire() called. AnimReloading=%d"),
         AnimReloading);

  if (Weapon && !AnimReloading) {
    Weapon->Fire();
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Fire blocked"));
  }
}

void ULMAWeaponComponent::BeginPlay() {
  Super::BeginPlay();

  SpawnWeapon();
  InitAnimNotify();
}

void ULMAWeaponComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  if (AnimReloading) {
    UE_LOG(LogTemp, Verbose, TEXT("TickComponent: AnimReloading is true"));
  }

}

void ULMAWeaponComponent::SpawnWeapon() {
  Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
  if (Weapon) {
    const auto Character = Cast<ACharacter>(GetOwner());
    if (Character) {
      FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
                                                false);
      Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules,
                                "r_Weapon_Socket");
    }
  }
}

void ULMAWeaponComponent::InitAnimNotify() {
  if (!ReloadMontage)
    return;

  const auto NotifiesEvents = ReloadMontage->Notifies;
  for (auto NotifyEvent : NotifiesEvents) {
    auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
    UE_LOG(LogTemp, Warning, TEXT("=== InitAnimNotify() called. ReloadFinish=%d"),
           ReloadFinish);

    if (ReloadFinish) {
      ReloadFinish->OnNotifyReloadFinished.AddUObject(
          this, &ULMAWeaponComponent::OnNotifyReloadFinished);
      break;
    }
  }
}

bool ULMAWeaponComponent::CanReload() const {
  return !AnimReloading && Weapon->CanReload();
}

void ULMAWeaponComponent::Reload() {
  UE_LOG(LogTemp, Warning, TEXT("=== Reload() called. AnimReloading=%d"),
         AnimReloading);

  if (!CanReload()) {
    return;
  }
  Weapon->ChangeClip();
  AnimReloading = true;
  ACharacter *Character = Cast<ACharacter>(GetOwner());

  if (Character) {
    float MontageLength = Character->PlayAnimMontage(ReloadMontage);
    UE_LOG(LogTemp, Warning, TEXT("PlayAnimMontage returned length: %f"),
           MontageLength);
  } else {
    UE_LOG(LogTemp, Error, TEXT("Character is null!"));
  }

  Character->PlayAnimMontage(ReloadMontage);
}

void ULMAWeaponComponent::OnNotifyReloadFinished(
    USkeletalMeshComponent *SkeletalMesh) {
  UE_LOG(LogTemp, Warning, TEXT(">>> OnNotifyReloadFinished called. Mesh: %s"),
         *GetNameSafe(SkeletalMesh));

  const auto Character = Cast<ACharacter>(GetOwner());
  if (Character->GetMesh() == SkeletalMesh) {
    UE_LOG(
        LogTemp, Warning,
        TEXT("Mesh matches character. Stopping montage and resetting flags."));

    AnimReloading = false;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Mesh mismatch or character null!"));
  }


  //Character->StopAnimMontage(ReloadMontage);
  //UE_LOG(LogTemp, Warning, TEXT("StopAnimMontage executed"));

}