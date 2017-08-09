// copyright Carter 2016

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Classes/Components/InputComponent.h" 
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Graber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEGAME_API UGraber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGraber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.f;
	UPhysicsHandleComponent *  PhysicasHandle = nullptr;
	UInputComponent * InputComponent = nullptr;
	//ray-cast and grab what's in reach
	void Grab();
	void Release();

	void FindPhysicsHandleComponent();
	void SetupInputComponent();

	//return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	
};
