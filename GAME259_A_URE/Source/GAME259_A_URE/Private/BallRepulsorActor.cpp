// Fill out your copyright notice in the Description page of Project Settings.


#include "BallRepulsorActor.h"
#include "BallActor.h"
#include "Components/StaticMeshComponent.h"
#include "../Main_Character.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABallRepulsorActor::ABallRepulsorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BallRepulsorCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = BallRepulsorCollision;
	BallRepulsorCollision->InitSphereRadius(200.0f);
	BallRepulsorCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BallRepulsorCollision->bHiddenInGame = false;

	BallRepulsorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BallRepulsorMesh->SetupAttachment(RootComponent);

	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	bNetUseOwnerRelevancy = true;
	bReplicates = true;
	durationTime = 3.0f;

}

// Called when the game starts or when spawned
void ABallRepulsorActor::BeginPlay()
{
	Super::BeginPlay();
	Activate();
}

// Called every frame
void ABallRepulsorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABallRepulsorActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	//Check if interacting with ball actor
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) &&
		OtherActor->IsA(ABallActor::StaticClass())) {
		ABallActor* ballActor = (ABallActor*)OtherActor;

		//Check that it is not a ball owned by the player
		if (GetOwner()) {
			if (ballActor->GetOwner() != GetOwner()) {
				if (GetOwner()->HasAuthority()) {
					if (sendRequest) {
						//Set a negative force then set the booleans
						UE_LOG(LogTemp, Warning, TEXT("Ability Activating"));
						FVector ballVector = ballActor->GetActorForwardVector();
						ballActor->ApplyImpulse(ballVector * -5000.0f); // change back to -1.0f after testing
					}
				}
			}
		}
	}
}


void ABallRepulsorActor::Activate(){
	GetOwner()->GetWorldTimerManager().SetTimer(TimeHandle, this,
		&ABallRepulsorActor::OnDestroy, durationTime, true);
}

void ABallRepulsorActor::OnDestroy() {
	Destroy();
}

