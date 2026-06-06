// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "LMAReloadFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyReloadFinishedSignature,
                                    USkeletalMeshComponent *);

UCLASS()
class LEAVEMEALONE_API ULMAReloadFinishedAnimNotify : public UAnimNotify {
  GENERATED_BODY()

public:
  virtual void Notify(USkeletalMeshComponent *MeshComp,
                      UAnimSequenceBase *Animation) override;

  FOnNotifyReloadFinishedSignature OnNotifyReloadFinished;
};