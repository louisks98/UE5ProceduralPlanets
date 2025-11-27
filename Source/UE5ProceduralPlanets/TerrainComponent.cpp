// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainComponent.h"

UTerrainComponent::UTerrainComponent()
{
	NoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>("Noise");
	NoiseWrapper->SetupFastNoise(EFastNoise_NoiseType::Simplex, 1337, 1.0f);

	PrimaryComponentTick.bCanEverTick = true;
	ContinentLayer = CreateDefaultSubobject<UNoiseLayer>("ContinentLayer");
	MountainLayer = CreateDefaultSubobject<UNoiseLayer>("MountainLayer");
}

FVector UTerrainComponent::EvaluateTerrain(const FVector& Point, const float Radius) const
{
	if (!NoiseWrapper)
	{
		UE_LOG(LogTemp, Error, TEXT("NoiseWrapper or ContinentLayer is null!"));
		return Point * Radius;
	}

	if (!NoiseWrapper->IsInitialized())
	{
		UE_LOG(LogTemp, Error, TEXT("NoiseWrapper not initialized"));
		return Point * Radius;
	}

	const float ContinentElevation = ContinentLayer->EvaluateNoise(*NoiseWrapper, Point);
	const float MountainElevation = MountainLayer->EvaluateNoise(*NoiseWrapper, Point) * ContinentElevation;

	float elevation = Radius * (ContinentElevation + MountainElevation + 1);
	SetHighestAndLowestElevation(elevation);
	return Point * elevation;
}

void UTerrainComponent::BeginPlay()
{
	Super::BeginPlay();
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
