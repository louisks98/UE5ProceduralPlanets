// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Biome.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROCEDURALPLANETS_API UBiome : public UObject
{
	GENERATED_BODY()

public:	
	UBiome();

	UPROPERTY()
	TMap<float, FLinearColor> Colors;
	
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Parameters")
	float StartHeight;


	FLinearColor EvaluateColor(float t) const;
};
