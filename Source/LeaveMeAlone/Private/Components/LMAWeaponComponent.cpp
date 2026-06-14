// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

void ULMAWeaponComponent::BeginPlay() {
  Super::BeginPlay();

  SpawnWeapon();
  InitAnimNotify();
}

void ULMAWeaponComponent::Fire() {
  if (Weapon && !AnimReloading) {
    Weapon->Fire();
  }
}
void ULMAWeaponComponent::NoFire() {
  if (Weapon) {
    Weapon->NoFire();
  }
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
      Weapon->ClipEmpty.AddDynamic(
          this, &ULMAWeaponComponent::ClipEmpty);

    }
  }
}

void ULMAWeaponComponent::InitAnimNotify() {
  if (!ReloadMontage)
    return;

  const auto NotifiesEvents = ReloadMontage->Notifies;
  for (auto NotifyEvent : NotifiesEvents) {
    auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
    UE_LOG(LogTemp, Warning,
           TEXT("=== InitAnimNotify() called. ReloadFinish=%d"), ReloadFinish);

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

void ULMAWeaponComponent::Reload() { ClipEmpty(); }

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
}
void ULMAWeaponComponent::ClipEmpty() {
  if (!CanReload())
    return;
  NoFire();
  Weapon->ChangeClip();
  AnimReloading = true;
  ACharacter *Character = Cast<ACharacter>(GetOwner());
  Character->PlayAnimMontage(ReloadMontage);
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon &AmmoWeapon) const {
  if (Weapon) {
    AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
    return true;
  }
  return false;
}

ALMABaseWeapon *ULMAWeaponComponent::GetWeaponObject() const {
  if (Weapon) {
    return Weapon;
  }
  return nullptr;
}