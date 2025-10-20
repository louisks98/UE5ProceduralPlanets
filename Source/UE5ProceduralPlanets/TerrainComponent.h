// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainLayer.h"
#include "Components/ActorComponent.h"
#include "TerrainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROCEDURALPLANETS_API UTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTerrainComponent();

	UPROPERTY(EditAnywhere)
	UTerrainLayer* ContinentLayer;
	UPROPERTY(EditAnywhere)
	UTerrainLayer* MountainLayer;
	
protected:
	virtual void BeginPlay() override;

public:	
	float EvaluateTerrain(FVector Point);

		
};
