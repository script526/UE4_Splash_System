#pragma once

#include "CoreMinimal.h"
#include "TrueSkyWaterProbeComponent.h"
#include "Components/ActorComponent.h"
#include "Engine.h"
#include "UObject/UObjectGlobals.h"
#include "Splash.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALMAZ_API USplash : public UTrueSkyWaterProbeComponent
{
	GENERATED_UCLASS_BODY()

		~USplash();

	class AShip* ship;
	UParticleSystem* splashFX;

	FTimerHandle timerHandle;
	FColor DebugSphereColor;
	FVector scale;

	float GetComponentDepth();
	float compDepth;
	bool bReadyForSplashes;

	void resetSplash();
	void spawnSplashes();
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
};