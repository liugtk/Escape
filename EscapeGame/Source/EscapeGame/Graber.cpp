// copyright Carter 2016

#include "Graber.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

#include "DrawDebugHelpers.h"
//#include "line"

#define OUT

// Sets default values for this component's properties
UGraber::UGraber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGraber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Graber reporting duty"));

	// ...
	
}


// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player's  view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	OUT	PlayerViewPointLocation, 
	OUT	PlayerViewPointRotation);
	
	//ray-cast out to reach distance
	//see what we hit
	//UE_LOG(LogTemp, Warning, TEXT("PlayerViewPointLocation Report: %s"), *PlayerViewPointLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("PlayerViewPointRotation Report: %s"), *PlayerViewPointRotation.ToString());
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
		
	// draw 
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255,0,0),
		false,
		0.f,
		0,
		10.f
	);

	//set up query parameters.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	// line-trace (AKA rat-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	//See what we hit
	AActor* ActorHit =  Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}
	



}

