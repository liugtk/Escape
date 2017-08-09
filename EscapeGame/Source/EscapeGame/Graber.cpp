// copyright Carter 2016

#include "Graber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
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

	FindPhysicsHandleComponent();

	SetupInputComponent();

}

void UGraber::FindPhysicsHandleComponent() {
	// look for attached physics  handle

	PhysicasHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicasHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGraber::SetupInputComponent()
{
	// look for attached Input Component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGraber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGraber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}

}





// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicasHandle) { return; }
	if (PhysicasHandle->GrabbedComponent) {
		PhysicasHandle->SetTargetLocation(GetReachLineEnd());
	}


}

void UGraber::Grab() {
	
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	//LINE TRACE and try and reach any action with phisical body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();


	///if we hit something then attach a physics handle
	if (ActorHit != nullptr) {
		if (!PhysicasHandle) { return; }
		PhysicasHandle->GrabComponent(
			ComponentToGrab, 
			NAME_None, // No Bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //allow rotation

		);
	}
}

void UGraber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"))
	if (!PhysicasHandle) { return; }
	PhysicasHandle->ReleaseComponent();
}



const FHitResult UGraber::GetFirstPhysicsBodyInReach()
{
	

	//set up query parameters.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// line-trace (AKA rat-cast) out to reach distance
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	//See what we hit
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}

	return HitResult;
}

FVector UGraber::GetReachLineStart() {
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
	return PlayerViewPointLocation;

}


FVector UGraber::GetReachLineEnd() {
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
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
}