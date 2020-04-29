#include "Splash.h"
#include "Components/ActorComponent.h"
#include "Engine.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
//#include "Particles/ParticleSystemComponent.h"

USplash::USplash(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	bAutoActivate = true;
	SetComponentTickEnabled(true);
	bReadyForSplashes = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> particle(TEXT("ParticleSystem'/Game/Assets/Particles/1/Particles1/PS_Waterfall.PS_Waterfall'"));
	splashFX = particle.Object;
	/*for (int32 i = 0; i < splashFX->Emitters.Num(); i++)
	{
		emitters.Add(splashFX->Emitters[i]);
	}*/
}

USplash::~USplash()
{
}

/* void USplash::setRightSided(const bool & side)
{
	is_right_side = side;
} */

// Resetting boolean after delay is elapsed so that we can again spawn emitter
void USplash::resetSplash()
{
	bReadyForSplashes = true;
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

// Get depth of each instance of this component (in Unreal units)
float USplash::GetComponentDepth()
{
	return UTrueSkyWaterProbeComponent::GetDepth() * 85;
}

// Spawn Emitter with proper scale (includes timer)
void USplash::spawnSplashes()
{
	if (bReadyForSplashes)
	{
		bReadyForSplashes = false;
		if (splashFX)
		{
			// Calculate size and location of the splashes
			float size = FMath::Clamp(GetComponentDepth() / 100, 0.7f, 2.f);
			FVector zOffset(0.f, 0.f, GetComponentDepth());
			FRotator rotation = splashNormalLocal.Rotation() - FRotator(0.f, 90.f, 0.f);


			// Spawn the emitter
			UGameplayStatics::SpawnEmitterAttached
			(
				splashFX
				, this
				, NAME_None
				, splashNormalLocal + zOffset
				, rotation
				, FVector(1.f, size, size)
				, EAttachLocation::KeepRelativeOffset
				, true
				, EPSCPoolMethod::None
			);
		}
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &USplash::resetSplash, 0.5f, false);
	}
}

void USplash::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);




	/* Location tutorial
		Convert splashNormalLocal position from local to world space with taking rotation into account:
		splashNormalWorldLocation = GetComponentLocation() + GetComponentRotation().RotateVector(splashNormalLocal);
		Convert splashNormalWorldLocation position from world to local space with taking rotation into account:
		splashNormalLocalPosition = GetComponentRotation().UnRotateVector(splashNormalWorldLocation - GetComponentLocation())
	*/
	//DrawDebugLine(GetWorld(), GetComponentLocation(), GetComponentLocation() + GetComponentRotation().RotateVector(splashNormalLocal), FColor::Red, false, 0.f, 0, 0.7f);
	//DrawDebugSphere(GetWorld(), GetComponentLocation(), 50.f, 5, FColor::Red, false, 0.f, 0, 1.f);

	if (GetComponentDepth() > 0.f)
	{
		spawnSplashes();
	}

	//GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, FString::Printf(TEXT("Emitter name: %s"), *emitters[0]->GetEmitterName().ToString()));
	//GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, FString::Printf(TEXT("Emitter name: %s"), *emitters[1]->GetEmitterName().ToString()));


}