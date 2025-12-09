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

	FLinearColor GetColorInRange(const FRandomStream* Rand, const FLinearColor PreviousColor) const
	{
		const float RandValue = Rand->GetFraction();
		return FMath::Lerp(PreviousColor, ColorB, RandValue);
	}
};

inline ColorStyle WaterColor = ColorStyle(FLinearColor(0.0f,0.26f,0.51f), FLinearColor(0.0f,0.102f,0.204f));
inline ColorStyle IceColors = ColorStyle(FLinearColor(0.427f, 0.895f, 1.0f), FLinearColor(0.973f, 0.973f, 1.0f));
inline ColorStyle GreenForestColors = ColorStyle(FLinearColor(0.29f, 0.404f, 0.255f), FLinearColor(0.133f, 0.192f, 0.114f));
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

inline BiomeType BasicBiome = BiomeType("Basic", TArray{GreenForestColors, GreenForestColors, DesertColors, DesertColors, IceColors}, TArray{WaterColor, WaterColor, WaterColor});
inline BiomeType FullForest = BiomeType("Forest", TArray{GreenForestColors, GreenForestColors, GreenForestColors}, TArray{WaterColor, WaterColor, WaterColor});
inline  BiomeType Desert = BiomeType("Desert", TArray{DesertColors, DesertColors, DesertColors}, TArray{DesertColors});
inline BiomeType Beach = BiomeType("Beach", TArray{DesertColors, DesertColors, DesertColors}, TArray{WaterColor, WaterColor, WaterColor});
inline BiomeType IceBiome = BiomeType("Ice", TArray{IceColors, IceColors, IceColors}, TArray{WaterColor, WaterColor, WaterColor});

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

	bool WriteTextureOnDisk = false;
	
	UTexture2D* GenerateBiomesTexture();
	float FindBiomePercentageFromPoint(FVector Position, const FVector& Normal);
	void RandomizeBiomes();
	
private:
	int const Max_Biomes = 4;
	UBiome* CreateBiome(const FRandomStream* Rand);
	static void GenerateBiomeColors(TMap<float, FLinearColor>* BiomeColors, TArray<ColorStyle> Colors, const FRandomStream* Rand);
};
