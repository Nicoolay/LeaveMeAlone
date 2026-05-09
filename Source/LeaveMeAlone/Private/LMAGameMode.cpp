// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "LMAGameMode.h"
#include "Player/LMADefaultCharacter1.h"
#include "Player/LMAPlayerController.h"

ALMAGameMode::ALMAGameMode() {
  DefaultPawnClass = ALMADefaultCharacter1::StaticClass();
  PlayerControllerClass = ALMAPlayerController::StaticClass();
}