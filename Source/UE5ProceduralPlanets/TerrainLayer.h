// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FastNoiseWrapper.h"
#include "TerrainLayer.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5PROCEDURALPLANETS_API UTerrainLayer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NoiseNumPass = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseRoughness = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Roughness = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Persistence = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Center;
	
	float EvaluateTerrain(const UFastNoiseWrapper& NoiseWrapper, const FVector& Point) const;
};
