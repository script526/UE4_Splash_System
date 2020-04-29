// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Splash.h"
#include "TrueSkyWaterBuoyancyComponent.h"
#include "TrueSkyWaterProbeComponent.h"
#include "Engine/World.h"




// Sets default values
AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShip::setProbesToPhysics()
{
	UPROPERTY()
		TArray<UTrueSkyWaterProbeComponent*> waterProbes;

	GetComponents<UTrueSkyWaterProbeComponent>(waterProbes);

	if (waterComponents[0] && waterProbes.IsValidIndex(0))
	{
		for (int i = 0; i < waterProbes.Num(); i++)
		{
			USceneComponent* waterProbesAttachedTo = Cast<USceneComponent>(waterProbes[i]->GetAttachParent());
			if (waterProbesAttachedTo == waterComponents[0] && waterProbes[i]->ProbeType == EProbeType::None)
			{
				waterProbes[i]->ProbeType = EProbeType::Physics;
			}
		}
	}
	// GetWorld()->GetTimerManager().ClearTimer(timerForProbes);
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	GetComponents(waterComponents);
	// GetWorld()->GetTimerManager().SetTimer(timerForProbes, this, &AShip::setProbesToPhysics, 0.1f, false);

	FHitResult outHit;
	FCollisionQueryParams collisionParams;

	FVector center = GetActorLocation();
	center.Z -= 200.f;

	FVector prevSplashLocation(0.f, 0.f, 0.f);

	for (double a = 0.0; a < 2 * PI; a += 2 * PI / 128) // devide circle on 128 parts
	{
		FVector end_vector;
		end_vector.X = 4000.f * cos(a);
		end_vector.Y = 4000.f * sin(a);
		end_vector.Z = center.Z;

		if (GetWorld()->LineTraceSingleByChannel(outHit, end_vector, center, ECC_Visibility, collisionParams))
		{
			if (FVector::Dist(prevSplashLocation, outHit.Location) > 300.f)
			{
				USplash* currentSplash = NewObject<USplash>(this);

				// Make sure the probes won't affect the ship physics and won't create waves
				currentSplash->ProbeType = EProbeType::DepthTest;

				currentSplash->RegisterComponent();
				currentSplash->SetWorldLocationAndRotation(outHit.Location, FRotator::ZeroRotator);

				// saving vector of a normal relative to respective USplash instance
				currentSplash->splashNormalLocal = outHit.Normal * 100.f - currentSplash->GetComponentLocation().Normalize();
				currentSplash->AttachToComponent(waterComponents[0], FAttachmentTransformRules::KeepWorldTransform, NAME_None);
				prevSplashLocation = outHit.Location;
			}
			/*	if (a <= PI)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("You hit %s"), *outHit.Location.ToString()));
					new_splash->setRightSided(true);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("You hit %s"), *outHit.Location.ToString()));
					new_splash->setRightSided(false);
				}
				splashComponents.Add(new_splash); */
		}
	}
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);





}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}