// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Player/LMADefaultCharacter1.h"

// Sets default values
ALMADefaultCharacter1::ALMADefaultCharacter1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALMADefaultCharacter1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALMADefaultCharacter1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALMADefaultCharacter1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

