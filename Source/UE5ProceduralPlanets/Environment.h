// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Biome.h"
#include "Components/ActorComponent.h"
#include "Environment.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROCEDURALPLANETS_API UEnvironment : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnvironment();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Biomes")
	TArray<TObjectPtr<UBiome>> Biomes;
	UPROPERTY()
	UNoiseLayer* BiomeNoiseLayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Biomes")
	float NoiseOffSet = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Biomes")
	float NoiseStrength = 1.0f;
	UPROPERTY()
	UFastNoiseWrapper* NoiseWrapper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Biomes")
	float BlendAmount = 0.05;

	UTexture2D* GenerateBiomesTexture();

	float FindBiomePercentageFromPoint(FVector Point);
};
