#pragma once

#include "CoreMinimal.h"
#include "NoiseLayer.h"
#include "Components/ActorComponent.h"
#include "FastNoiseWrapper.h"
#include "TerrainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROCEDURALPLANETS_API UTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTerrainComponent();

	UPROPERTY(Transient, BlueprintReadOnly)
	UNoiseLayer* ContinentLayer;
	UPROPERTY(Transient, BlueprintReadOnly)
	UNoiseLayer* MountainLayer;
	UPROPERTY(Transient, BlueprintReadOnly)
	UNoiseLayer* DetailLayer;
	
	
	float EvaluateUnscaledTerrain(const FVector& Point) const;

	float GetHighestElevation() const {return  HighestElevation;}
	float GetLowestElevation() const {return LowestElevation;}
	void ResetElevation() const;
	void RandomizeTerrain() const;

private:
	UPROPERTY()
	UFastNoiseWrapper* NoiseWrapper;

	mutable float HighestElevation = std::numeric_limits<float>::lowest();
	mutable float LowestElevation = std::numeric_limits<float>::max();

	void SetHighestAndLowestElevation(float elevation) const;
};
