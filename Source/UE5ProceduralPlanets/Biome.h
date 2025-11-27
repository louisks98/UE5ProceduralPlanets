// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoiseLayer.h"
#include "Components/ActorComponent.h"
#include "Biome.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROCEDURALPLANETS_API UBiome : public UObject
{
	GENERATED_BODY()

public:	
	UBiome();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor Color1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor Color2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor Color3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor Color4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor Color5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor Color6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float StartHeight;


	FLinearColor EvaluateGradient(float t) const;
};
