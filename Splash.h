#pragma once

#include "CoreMinimal.h"
#include "TrueSkyWaterProbeComponent.h"
#include "Splash.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALMAZ_API USplash : public UTrueSkyWaterProbeComponent
{
	GENERATED_UCLASS_BODY()

		~USplash();

	class UParticleSystem* splashFX;
	// TArray<UParticleEmitter*> emitters;

	struct FTimerHandle timerHandle;
	FColor DebugSphereColor;
	FVector scale;
	FVector splashNormalLocal; // vector of a normal in local space of 'this' component

	bool is_right_side = false;

	float GetComponentDepth();
	bool bReadyForSplashes;

	// void setRightSided(const bool &side);

	void resetSplash();
	void spawnSplashes();
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
};