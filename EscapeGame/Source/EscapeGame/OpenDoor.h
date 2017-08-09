// copyright Carter 2016

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEventRequest);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEGAME_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
	void CLoseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FDoorEventRequest OnOpenRequest;
	UPROPERTY(BlueprintAssignable)
	FDoorEventRequest OnCloseRequest;

private:


	UPROPERTY(EditAnywhere)
	ATriggerVolume * PressurePlate = nullptr;
		
	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	UPROPERTY(EditAnywhere)

	AActor * Owner = nullptr;
	
	float GetTotalMassOfActorsOnPlate();
	
};
