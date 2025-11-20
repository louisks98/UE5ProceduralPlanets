// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainLayer.h"
#include "Components/ActorComponent.h"
#include "FastNoiseWrapper.h"
#include "TerrainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROCEDURALPLANETS_API UTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTerrainComponent();

UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTerrainLayer* ContinentLayer;
UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTerrainLayer* MountainLayer;
	
	FVector EvaluateTerrain(const FVector& Point, float Radius) const;

	float GetHighestElevation() const {return  HighestElevation;}
	float GetLowestElevation() const {return LowestElevation;}
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UFastNoiseWrapper* NoiseWrapper;

	mutable float HighestElevation = std::numeric_limits<float>::lowest();
	mutable float LowestElevation = std::numeric_limits<float>::max();

	void SetHighestAndLowestElevation(float elevation) const;
};
