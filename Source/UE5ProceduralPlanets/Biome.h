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
	TMap<float, FLinearColor> TopLandColors;
	UPROPERTY()
	TMap<float, FLinearColor> LandColors;
	UPROPERTY()
	TMap<float, FLinearColor> OceanColors;
	
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Parameters")
	float StartHeight;

	FLinearColor EvaluateTopLandColor(float t) const;
	FLinearColor EvaluateLandColor(float t) const;
	FLinearColor EvaluateOceanColor(float t) const;

private:
	static FLinearColor EvaluateColor(const TMap<float, FLinearColor>& Colors, float t);
};


