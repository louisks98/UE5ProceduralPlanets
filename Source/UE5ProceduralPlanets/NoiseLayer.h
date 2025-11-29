#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FastNoiseWrapper.h"
#include "NoiseLayer.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5PROCEDURALPLANETS_API UNoiseLayer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadOnly)
	int NoiseNumPass = 5;
	UPROPERTY(Transient, BlueprintReadWrite)
	float Strength = 0.2f;
	UPROPERTY(Transient, BlueprintReadWrite)
	float BaseRoughness = 1;
	UPROPERTY(Transient, BlueprintReadWrite)
	float Roughness = 2;
	UPROPERTY(Transient, BlueprintReadWrite)
	float Persistence = 0.5f;
	UPROPERTY(Transient, BlueprintReadWrite)
	float Height = 0.5f;
	UPROPERTY(Transient, BlueprintReadWrite)
	float WeightMultiplier = 0.8f;
	
	float EvaluateNoise(const UFastNoiseWrapper& NoiseWrapper, const FVector& Point) const;
	float EvaluateRigidNoise(const UFastNoiseWrapper& NoiseWrapper, const FVector& Point) const;

	void RandomizeNoiseParameters();
	void RandomizeNoiseRigidParameters();
};
