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
	if (PhysicasHandle) {
		//Physical handle is found 
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGraber::SetupInputComponent()
{
	// look for attached Input Component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		//Physical handle is found 
		UE_LOG(LogTemp, Warning, TEXT("%s Found input component"), *GetOwner()->GetName());
		//bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGraber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGraber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}

}



void UGraber::Grab() {
	
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	//LINE TRACE and try and reach any action with phisical body collision channel
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	///if we hit something then attach a physics handle
	auto ActorHit = HitResult.GetActor();
	if (ActorHit != nullptr) {
		PhysicasHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //allow rotation

		);
	}
}

void UGraber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"))
		PhysicasHandle->ReleaseComponent();
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

	if (PhysicasHandle->GrabbedComponent) {
		PhysicasHandle->SetTargetLocation(LineTraceEnd);
	}


}

const FHitResult UGraber::GetFirstPhysicsBodyInReach()
{
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
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}