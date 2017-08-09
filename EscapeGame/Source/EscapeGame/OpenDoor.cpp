// copyright Carter 2016

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"


#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s is miss pressure plate!!"), *GetOwner()->GetName())
	}
}

void UOpenDoor::OpenDoor()
{
	
	
	//Create a rotator
	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.f);

	//set the door rotation
	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CLoseDoor()
{
	//Create a rotator
	FRotator NewRotation = FRotator(0.0f, -90.0f, 0.f);

	//set the door rotation
	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > 40.f) { //TODO make into a parameter
		OpenDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() - lastDoorOpenTime > DoorCloseDelay) {
		CLoseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActings;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActings);

	//Iterate through then adding their masses
	for (const auto& Actor : OverlappingActings) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp,Warning,TEXT("%s on Pressure plate"), *Actor->GetName())
	}


	return TotalMass;
}