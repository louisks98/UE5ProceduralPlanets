#include "NoiseLayer.h"

float UNoiseLayer::EvaluateNoise(const UFastNoiseWrapper& NoiseWrapper, const FVector& Point) const
{
	float noise = 0;
	float amplitude = 1;
	float frequency = BaseRoughness;

	for (int i = 0; i < NoiseNumPass; i++)
	{
		FVector ptn = Point * frequency;
		float n = NoiseWrapper.GetNoise3D(ptn.X, ptn.Y, ptn.Z);
		noise += (n + 1) * 0.5f * amplitude;
		amplitude *= Persistence;
		frequency *= Roughness;
	}

	noise = noise - Height;
	return  noise * Strength;
}

float UNoiseLayer::EvaluateRigidNoise(const UFastNoiseWrapper& NoiseWrapper, const FVector& Point) const
{
	float noise = 0;
	float amplitude = 1;
	float frequency = BaseRoughness;
	float weight = 1;

	for (int i = 0; i < NoiseNumPass; i++)
	{
		FVector ptn = Point * frequency;
		float n = 1 - FMath::Abs(NoiseWrapper.GetNoise3D(ptn.X, ptn.Y, ptn.Z));
		n *= n;
		n *= weight;
		weight = FMath::Clamp(n * WeightMultiplier, 0, 1);
		noise += n * amplitude;
		amplitude *= Persistence;
		frequency *= Roughness;
	}

	noise = noise - Height;
	return  noise * Strength;
}

void UNoiseLayer::RandomizeNoiseParameters()
{
	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();

	Strength = RandomStream.FRandRange(0.1f, 0.3f);
	BaseRoughness = RandomStream.FRandRange(0.5f, 1.0f);
	Roughness = RandomStream.FRandRange(BaseRoughness + 1.25f, 3.0f);
	Persistence = RandomStream.FRandRange(0.5f, 0.55f);
	Height = RandomStream.FRandRange(1.0f, 1.3f);
}

void UNoiseLayer::RandomizeNoiseRigidParameters()
{
	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();
	
	Strength = 	RandomStream.FRandRange(0.3f, 1.0f);
	BaseRoughness = RandomStream.FRandRange(1.0f, 2.0f);
	Roughness = RandomStream.FRandRange(2.0f, 3.5f);
	Persistence = RandomStream.FRandRange(0.5f, 0.65f);
	Height = RandomStream.FRandRange(0.1f, 1.0f);
	WeightMultiplier = RandomStream.FRandRange(0.5f, 1.5f);
}
