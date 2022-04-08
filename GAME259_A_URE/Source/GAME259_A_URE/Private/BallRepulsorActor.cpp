// Fill out your copyright notice in the Description page of Project Settings.


#include "BallRepulsorActor.h"
#include "BallActor.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABallRepulsorActor::ABallRepulsorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BallRepulsorCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	BallRepulsorCollision->InitSphereRadius(150.0f);
	BallRepulsorCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = BallRepulsorCollision;
	sendRequest = false;
	triggered = false;
}

// Called when the game starts or when spawned
void ABallRepulsorActor::BeginPlay()
{
	Super::BeginPlay();
	BallRepulsorCollision->OnComponentBeginOverlap.AddDynamic(this, &ABallRepulsorActor::BeginOverlap);
	BallRepulsorCollision->OnComponentEndOverlap.AddDynamic(this, &ABallRepulsorActor::EndOverlap);
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

		if (ballActor->GetOwner() != GetOwner()) {
			//UE_LOG(LogTemp, Warning, TEXT("BallRepulsor Ability Overlapping"));
			//UE_LOG(LogTemp, Warning, TEXT("SengRequest: %s"), (sendRequest ? TEXT("true") : TEXT("false")));

			if (sendRequest) {
				//Set a negative force then set the booleans
				UE_LOG(LogTemp, Warning, TEXT("Requesting to activate ability"));
				//ballActor->ApplyForce(-1.0f); //need test
				triggered = true;
				sendRequest = false;
			}
		}

	}

}

void ABallRepulsorActor::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	triggered = false;
}
