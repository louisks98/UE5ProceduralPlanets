#include "Environment.h"
#include "NoiseLayer.h"

UEnvironment::UEnvironment()
{
	NoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>("Noise");
	NoiseWrapper->SetupFastNoise(EFastNoise_NoiseType::Simplex, 8888, 1.0f);
	BiomeNoiseLayer = CreateDefaultSubobject<UNoiseLayer>("BiomeNoiseLayer");
}

UTexture2D* UEnvironment::GenerateBiomesTexture()
{
	const int32 Width = 128;
	const int32 Height = Biomes.Num();
	
	UTexture2D* Texture = UTexture2D::CreateTransient(Width * 2, Height, PF_B8G8R8A8);
	if (!Texture) return nullptr;

	Texture->CompressionSettings = TC_VectorDisplacementmap;
	Texture->SRGB = false;
	Texture->Filter = TF_Bilinear;
	Texture->AddressX = TA_Clamp;
	Texture->AddressY = TA_Clamp;
	Texture->UpdateResource();
	
	FTexture2DMipMap& MipMap = Texture->GetPlatformData()->Mips[0];
	FByteBulkData& ImageData = MipMap.BulkData;
	uint8* RawData = static_cast<uint8*>(ImageData.Lock(LOCK_READ_WRITE));
	
	for (int32 y = 0; y < Height; y++)
	{
		UBiome* Biome = Biomes[y];
		for (int32 x = 0; x < Width * 2; x++)
		{
			float t;
			FLinearColor GradientColor;
			if (x < Width)
			{
				t = static_cast<float>(x) / static_cast<float>(Width - 1);
				GradientColor = Biome->EvaluateOceanColor(t);
			}
			else
			{
				t = static_cast<float>(x - Width) / static_cast<float>(Width - 1);
				GradientColor = Biome->EvaluateLandColor(t);
			}

			FColor ByteColor = GradientColor.ToFColor(false);

			int32 PixelIndex = (y * Width * 2 + x) * 4;
			RawData[PixelIndex + 0] = ByteColor.B;
			RawData[PixelIndex + 1] = ByteColor.G;
			RawData[PixelIndex + 2] = ByteColor.R;
			RawData[PixelIndex + 3] = ByteColor.A;
		}
	}

	ImageData.Unlock();
	Texture->UpdateResource();

	return Texture;
}

float UEnvironment::FindBiomePercentageFromPoint(FVector Point)
{
	float HeightPercent = (Point.Z + 1) / 2.0f;
	HeightPercent += (BiomeNoiseLayer->EvaluateNoise(*NoiseWrapper, Point) - NoiseOffSet) * NoiseStrength;
	
	int CurrentBiomeIndex = 0;
	for (int i = Biomes.Num() - 1; i >= 0; i--)
	{
		if (HeightPercent >= Biomes[i]->StartHeight)
		{
			CurrentBiomeIndex = i;
			break;
		}
	}
	
	float BiomeIndex = CurrentBiomeIndex;
	if (CurrentBiomeIndex < Biomes.Num() - 1)
	{
		float CurrentHeight = Biomes[CurrentBiomeIndex]->StartHeight;
		float NextHeight = Biomes[CurrentBiomeIndex + 1]->StartHeight;
		float BlendZoneSize = (NextHeight - CurrentHeight) * BlendAmount;

		if (BlendZoneSize > 0.001f)
		{
			float DistanceIntoBlend = HeightPercent - (NextHeight - BlendZoneSize);
			if (DistanceIntoBlend > 0)
			{
				float BlendFactor = FMath::Clamp(DistanceIntoBlend / BlendZoneSize, 0.0f, 1.0f);
				BiomeIndex = FMath::Lerp(static_cast<float>(CurrentBiomeIndex), static_cast<float>(CurrentBiomeIndex + 1), BlendFactor);
			}
		}
	}

	return BiomeIndex / FMath::Max(1, Biomes.Num() - 1);
}



void UEnvironment::RandomizeBiomes()
{
	Biomes.Empty();

	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();
	const int NbBiomes = RandomStream.RandRange(1, Max_Biomes);
	float Height = 0;
	for (int32 i = 0; i < NbBiomes; i++)
	{
		UBiome* Biome = CreateBiome(&RandomStream);
		Biome->StartHeight = Height;
		Biomes.Add(Biome);
		Height += 1.0f / NbBiomes;
	}
}

UBiome* UEnvironment::CreateBiome(const FRandomStream* Rand)
{
	UBiome* Biome = NewObject<UBiome>(this);
	Biome->LandColors.Empty();
	Biome->OceanColors.Empty();

	const int BiomeTypeIndex = Rand->RandRange(0, BiomesTypes.Num() - 1);
	const BiomeType Type = BiomesTypes[BiomeTypeIndex];
	GenerateBiomeColors(&Biome->LandColors, Type.LandColors, Rand);
	GenerateBiomeColors(&Biome->OceanColors, Type.OceanColors, Rand);

	return Biome;
}

void UEnvironment::GenerateBiomeColors(TMap<float, FLinearColor>* BiomeColors, TArray<ColorStyle> Colors, const FRandomStream* Rand)
{
	const int NbColors = Colors.Num();
	float MinStepRange = 0.0f;
	float MaxStepRange = 1.0f / NbColors;
	
	for (int32 i = 0; i < NbColors; i++)
	{
		ColorStyle Style = Colors[i];
		FLinearColor Color = Style.GetColorInRange(Rand);
		float Step = Rand->FRandRange(MinStepRange, MaxStepRange);
		BiomeColors->Add({Step, Color});
		MinStepRange = Step + MaxStepRange / (i + 1);
		MaxStepRange += 1.0f / NbColors;
	}
}






