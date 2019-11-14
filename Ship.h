// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrueSkyWaterBuoyancyComponent.h"
#include "Ship.generated.h"

UCLASS()
class ALMAZ_API AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();

		AShip* Ship;
		class USplash* splash;

		UPROPERTY()
		TArray<class USplash*> splashComponents;

		UPROPERTY()
		TArray<UTrueSkyWaterBuoyancyComponent*> waterComponents;

		UPROPERTY()
		TArray<FVector> rightTracesStart;

		UPROPERTY()
		TArray<FVector> leftTracesStart;

		UFUNCTION()
		TArray<FVector> rangeOfVectors(FVector start, FVector end, int32 counter);

		void calculateLineTraceLocations();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
