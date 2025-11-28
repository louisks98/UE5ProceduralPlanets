#pragma once

#include "CoreMinimal.h"
#include "Biome.h"
#include "FastNoiseWrapper.h"
#include "NoiseLayer.h"
#include "Components/ActorComponent.h"
#include "Environment.generated.h"


struct ColorStyle
{
private:
	FLinearColor ColorA;
	FLinearColor ColorB;

public:
	ColorStyle(const FLinearColor ColorA, const FLinearColor ColorB)
	{
		this->ColorA = ColorA;
		this->ColorB = ColorB;
	}
	
	FLinearColor GetColorInRange(const FRandomStream* Rand) const
	{
		const float RandValue = Rand->GetFraction();
		return FMath::Lerp(ColorA, ColorB, RandValue);
	}
};

inline ColorStyle WaterColor = ColorStyle(FLinearColor(0.529f, 0.808f, 0.922f), FLinearColor(0.0f, 0.749f, 1.0f));
inline ColorStyle IceColors = ColorStyle(FLinearColor(0.973f, 0.973f, 1.0f), FLinearColor(0.690f, 0.816f, 0.878f));
inline ColorStyle GreenForestColors = ColorStyle(FLinearColor(0.235f, 0.702f, 0.443f), FLinearColor(0.180f, 0.490f, 0.196f));
inline ColorStyle DesertColors = ColorStyle(FLinearColor(0.831f, 0.635f, 0.416f), FLinearColor(0.475f, 0.333f, 0.282f));


struct BiomeType
{
	FString Name;
	TArray<ColorStyle> OceanColors;
	TArray<ColorStyle> LandColors;

	explicit BiomeType(FString Name, const TArray<ColorStyle>& LandColors, const TArray<ColorStyle>& WaterColors)
	{
		this->Name = Name;
		this->LandColors = LandColors;
		this->OceanColors = WaterColors;
	}
};

inline BiomeType BasicBiome = BiomeType("Basic", TArray{GreenForestColors, GreenForestColors, DesertColors, IceColors}, TArray{WaterColor, WaterColor});
inline BiomeType FullForest = BiomeType("Forest", TArray{GreenForestColors, GreenForestColors, GreenForestColors}, TArray{WaterColor, WaterColor});
inline  BiomeType Desert = BiomeType("Desert", TArray{DesertColors, DesertColors, DesertColors}, TArray{DesertColors});
inline BiomeType Beach = BiomeType("Beach", TArray{DesertColors, DesertColors}, TArray{WaterColor, WaterColor});
inline BiomeType IceBiome = BiomeType("Ice", TArray{IceColors, IceColors, IceColors}, TArray{WaterColor, WaterColor});

inline TArray BiomesTypes{BasicBiome, FullForest, Desert, Beach, IceBiome};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROCEDURALPLANETS_API UEnvironment : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnvironment();

	UPROPERTY()
	UNoiseLayer* BiomeNoiseLayer;
	UPROPERTY()
	UFastNoiseWrapper* NoiseWrapper;
	UPROPERTY(Transient, BlueprintReadOnly, Category="Biomes")
	TArray<TObjectPtr<UBiome>> Biomes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Biomes")
	float NoiseOffSet = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Biomes")
	float NoiseStrength = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Biomes")
	float BlendAmount = 0.4;
	
	UTexture2D* GenerateBiomesTexture();
	float FindBiomePercentageFromPoint(FVector Point);
	void RandomizeBiomes();
	
private:
	int const Max_Biomes = 4;
	UBiome* CreateBiome(const FRandomStream* Rand);
	static void GenerateBiomeColors(TMap<float, FLinearColor>* BiomeColors, TArray<ColorStyle> Colors, const FRandomStream* Rand);
};
