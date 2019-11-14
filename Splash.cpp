#include "Splash.h"
#include "Ship.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



USplash::USplash(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	bAutoActivate = true;
	SetComponentTickEnabled(true);
	bReadyForSplashes = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> particle(TEXT("ParticleSystem'/Game/Assets/Particles/Splashes/P_Splashes.P_Splashes'"));
	splashFX = particle.Object;
	ship = Cast<AShip>(GetOwner());
}

USplash::~USplash()
{
}

// Resetting boolean after delay is elapsed so that we can again spawn emitter
void USplash::resetSplash()
{
	bReadyForSplashes = true;
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

// Get depth of each instance of this component (in Unreal units)
float USplash::GetComponentDepth()
{
	compDepth = UTrueSkyWaterProbeComponent::GetDepth() * 85;
	return compDepth;
}

// Spawn Emitter with proper scale (includes timer)
void USplash::spawnSplashes()
{
	if (bReadyForSplashes)
	{
		bReadyForSplashes = false;
		if (splashFX)
		{
			// Calculate scale components depending on depth
			float size = FMath::Clamp(compDepth / 100, 0.1f, 5.f);

			// Spawn the emitter
			UGameplayStatics::SpawnEmitterAttached(splashFX, this, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f, size, size), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None);
		}
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &USplash::resetSplash, 0.7f, false);
	}
}

void USplash::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetComponentDepth() > 0.f)
	{
		spawnSplashes();
	}
}

