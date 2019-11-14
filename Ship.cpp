// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Splash.h"

// Sets default values
AShip::AShip()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}




// Distribute vectors evengly in a straight line across X axis
TArray<FVector> AShip::rangeOfVectors(FVector start, FVector end, int32 sizeOf)
{
	TArray<FVector> rangeOfVectors;

	float step = FVector::Dist(start, end) / sizeOf - 1;
	for (int32 i = 0; i < sizeOf; i++)
	{
		rangeOfVectors.Add(FVector(start.X + (i * step), start.Y, start.Z));
	}
	return rangeOfVectors;
}

// Where to start traces from
void AShip::calculateLineTraceLocations()
{
	FVector forward = (AActor::GetActorForwardVector() * 3000.f) + GetActorLocation();
	FVector back = (-AActor::GetActorForwardVector() * 3000.f) + GetActorLocation();
	FVector right = (AActor::GetActorRightVector() * 800.f) + GetActorLocation();
	FVector left = ((-AActor::GetActorRightVector()) * 800.f) + GetActorLocation();

	FVector topRight = FVector(right.X + 3000.f, right.Y, right.Z - 200.f);
	FVector topLeft = FVector(left.X + 3000.f, left.Y, left.Z - 200.f);
	FVector backRight = FVector(right.X - 3000.f, right.Y, right.Z - 200.f);
	FVector backLeft = FVector(left.X - 3000.f, left.Y, left.Z - 200.f);

	rightTracesStart.Append(rangeOfVectors(backRight, topRight, 16));
	leftTracesStart.Append(rangeOfVectors(backLeft, topLeft, 16));
}

/*// Create a vector of evenly spaced numbers.
vector<double> range(double min, double max, size_t N)
{
	vector<double> range;
	double delta = (max - min) / double(N - 1);
	for (int i = 0; i < N; i++) {
		range.push_back(min + i * delta);
	}
	return range;
} */

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	calculateLineTraceLocations();

	FHitResult outHit;
	FCollisionQueryParams collisionParams;
	this->GetComponents(waterComponents);

	// Calculate positions of splash components and attach them to water byouancy component
	// Right side
	int hitFalse = 0;
	for (int32 i = 0; i < rightTracesStart.Num(); i++)
	{
		if (GetWorld()->LineTraceSingleByChannel(outHit, rightTracesStart[i], rightTracesStart[i] + FVector(0.f, -790.f, 0.f), ECC_Visibility, collisionParams))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("You hit %s"), *outHit.Location.ToString()));
			splashComponents.Add(splash);
			splashComponents[i - hitFalse] = NewObject<USplash>(this);
			splashComponents[i - hitFalse]->RegisterComponent();
			splashComponents[i - hitFalse]->SetWorldLocationAndRotation(outHit.Location, FRotator::ZeroRotator);
			splashComponents[i - hitFalse]->AttachToComponent(waterComponents[0], FAttachmentTransformRules::KeepWorldTransform, NAME_None);
		}
		else hitFalse++;
	}
	// Left side
	hitFalse = 0;
	for (int32 i = 0; i < leftTracesStart.Num(); i++)
	{
		if (GetWorld()->LineTraceSingleByChannel(outHit, leftTracesStart[i], leftTracesStart[i] + FVector(0.f, 790.f, 0.f), ECC_Visibility, collisionParams))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("You hit %s"), *outHit.Location.ToString()));
			splashComponents.Add(splash);
			splashComponents[i - hitFalse] = NewObject<USplash>(this);
			splashComponents[i - hitFalse]->RegisterComponent();

			// Rotating component before attaching so that we won't need to adjust emitter rotation when spawned
			splashComponents[i - hitFalse]->SetWorldLocationAndRotation(outHit.Location, FRotator(0.f, 180.f, 0.f));
			splashComponents[i - hitFalse]->AttachToComponent(waterComponents[0], FAttachmentTransformRules::KeepWorldTransform, NAME_None);
		}
		else hitFalse++;
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