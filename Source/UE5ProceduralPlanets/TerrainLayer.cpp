// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainLayer.h"

float UTerrainLayer::EvaluateTerrain(const UFastNoiseWrapper& NoiseWrapper, const FVector& Point) const
{
	float noise = 0;
	float amplitude = 1;
	float frequency = BaseRoughness;

	for (int i = 0; i < NoiseNumPass; i++)
	{
		FVector ptn = Point * frequency + Center;
		float n = NoiseWrapper.GetNoise3D(ptn.X, ptn.Y, ptn.Z);
		noise += (n + 1) * 0.5f * amplitude;
		amplitude *= Persistence;
		frequency *= Roughness;
	}

	noise = std::max(0.0f, noise - Height);
	return  noise * Strength;
}
