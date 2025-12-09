#include "TerrainComponent.h"

UTerrainComponent::UTerrainComponent()
{
	NoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>("Noise");
	NoiseWrapper->SetupFastNoise(EFastNoise_NoiseType::Simplex, 1337, 1.0f);

	PrimaryComponentTick.bCanEverTick = true;
	ContinentLayer = CreateDefaultSubobject<UNoiseLayer>("ContinentLayer");
	MountainLayer = CreateDefaultSubobject<UNoiseLayer>("MountainLayer");
	DetailLayer = CreateDefaultSubobject<UNoiseLayer>("DetailLayer");
	
}

float UTerrainComponent::EvaluateUnscaledTerrain(const FVector& Point) const
{
	if (!NoiseWrapper)
	{
		UE_LOG(LogTemp, Error, TEXT("NoiseWrapper or ContinentLayer is null!"));
		return 1.0f;
	}

	if (!NoiseWrapper->IsInitialized())
	{
		UE_LOG(LogTemp, Error, TEXT("NoiseWrapper not initialized"));
		return 1.0f;
	}

	const float ContinentElevation = ContinentLayer->EvaluateNoise(*NoiseWrapper, Point);
	const float MountainElevation = MountainLayer->EvaluateRigidNoise(*NoiseWrapper, Point);
	const float DetailElevation = DetailLayer->EvaluateRigidNoise(*NoiseWrapper, Point);

	const float Elevation = ContinentElevation + MountainElevation * ContinentElevation;
	SetHighestAndLowestElevation(Elevation);
	const float DetailStrength = 0.05f + (abs(MountainElevation) * 0.1f);
	return Elevation + DetailElevation * DetailStrength;
}

void UTerrainComponent::SetHighestAndLowestElevation(float elevation) const
{
	if (elevation > HighestElevation)
		HighestElevation = elevation;
	if (elevation < LowestElevation)
		LowestElevation = elevation;
}

void UTerrainComponent::ResetElevation() const
{
	HighestElevation = std::numeric_limits<float>::lowest();
	LowestElevation = std::numeric_limits<float>::max();
}

void UTerrainComponent::RandomizeTerrain() const
{
	ContinentLayer->RandomizeNoiseParameters();
	MountainLayer->RandomizeNoiseRigidParameters();
}
