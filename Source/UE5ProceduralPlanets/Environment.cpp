// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment.h"

UEnvironment::UEnvironment()
{
	NoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>("Noise");
	NoiseWrapper->SetupFastNoise(EFastNoise_NoiseType::Simplex, 8888, 1.0f);
	BiomeNoiseLayer = CreateDefaultSubobject<UNoiseLayer>("BiomeNoiseLayer");

	UBiome* biome = CreateDefaultSubobject<UBiome>("Biome1");
	biome->Color1 = FLinearColor(0.805,0.684,0.550,1.0);
	biome->Color2 = FLinearColor(0.831,0.635,0.416,1.0);
	biome->Color3 = FLinearColor(0.631,0.435,0.29,1.0);
	biome->Color4 = FLinearColor(0.459,0.333,0.302,1.0);
	biome->Color5 = FLinearColor(0.271,0.271,0.322,1.0);
	biome->Color6 = FLinearColor(0.545,0.271,0.075,1.0);
	biome->StartHeight = 0.0f;
	Biomes.Add(biome);

	UBiome* biome2 = CreateDefaultSubobject<UBiome>("Biome2");
	biome2->Color1 = FLinearColor(0.29,0.686,0.314,1.0);
	biome2->Color2 = FLinearColor(0.18,0.48,0.196,1.0);
	biome2->Color3 = FLinearColor(0.659,0.557,0.369,1.0);
	biome2->Color4 = FLinearColor(0.365,0.251,0.216,1.0);
	biome2->Color5 = FLinearColor(0.243,0.243,0.243,1.0);
	biome2->Color6 = FLinearColor(0.545,0.271,0.075,1.0);
	biome2->StartHeight = 0.25f;
	Biomes.Add(biome2);

	UBiome* biome3 = CreateDefaultSubobject<UBiome>("Biome3");
	biome3->Color1 = FLinearColor(1.0,0.271,0.0,1.0);
	biome3->Color2 = FLinearColor(0.78,0.0,0.224,1.0);
	biome3->Color3 = FLinearColor(0.545,0.0,0.0,1.0);
	biome3->Color4 = FLinearColor(0.235,0.0,0.0,1.0);
	biome3->Color5 = FLinearColor(1.0,0.843,0.0,1.0);
	biome3->Color6 = FLinearColor(0.439,0.502,0.565,1.0);
	biome3->StartHeight = 0.5f;
	Biomes.Add(biome3);

	UBiome* biome4 = CreateDefaultSubobject<UBiome>("Biome4");
	biome4->Color1 = FLinearColor(0.0,0.749,1.0,1.0);
	biome4->Color2 = FLinearColor(0.961,0.961,0.863,1.0);
	biome4->Color3 = FLinearColor(0.235,0.702,0.443,1.0);
	biome4->Color4 = FLinearColor(0.545,0.271,0.075,1.0);
	biome4->Color5 = FLinearColor(0.412,0.412,0.412,1.0);
	biome4->Color6 = FLinearColor(0.118,0.565,1.0,1.0);
	biome4->StartHeight = 0.75f;
	Biomes.Add(biome4);
	
}

UTexture2D* UEnvironment::GenerateBiomesTexture()
{
	const int32 Width = 128;
	const int32 Height = Biomes.Num();
	
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
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
		for (int32 x = 0; x < Width; x++)
		{
			float t = static_cast<float>(x) / static_cast<float>(Width - 1);
			FLinearColor GradientColor = Biome->EvaluateGradient(t);
			FColor ByteColor = GradientColor.ToFColor(false);
			
			int32 PixelIndex = (y * Width + x) * 4;
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
				BiomeIndex = FMath::Lerp((float)CurrentBiomeIndex, (float)(CurrentBiomeIndex + 1), BlendFactor);
			}
		}
	}

	return BiomeIndex / FMath::Max(1, Biomes.Num() - 1);
}






