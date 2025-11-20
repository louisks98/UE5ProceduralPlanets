// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainComponent.h"

UTerrainComponent::UTerrainComponent()
{
	NoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>("Noise");
	NoiseWrapper->SetupFastNoise(EFastNoise_NoiseType::Simplex, 1337, 1.0f);

	PrimaryComponentTick.bCanEverTick = true;
	ContinentLayer = CreateDefaultSubobject<UTerrainLayer>("ContinentLayer");
	MountainLayer = CreateDefaultSubobject<UTerrainLayer>("MountainLayer");
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

	const float ContinentElevation = ContinentLayer->EvaluateTerrain(*NoiseWrapper, Point);
	const float MountainElevation = MountainLayer->EvaluateTerrain(*NoiseWrapper, Point) * ContinentElevation;

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