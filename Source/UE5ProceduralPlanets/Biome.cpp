#include "Biome.h"

UBiome::UBiome()
{
	LandColors.Add({0.0f, FLinearColor::White});
	StartHeight = 1.0f;
}

FLinearColor UBiome::EvaluateLandColor(const float t) const
{
	return EvaluateColor(LandColors, t);
}

FLinearColor UBiome::EvaluateOceanColor(float t) const
{
	return EvaluateColor(OceanColors, t);
}

FLinearColor UBiome::EvaluateColor(const TMap<float, FLinearColor>& Colors, float t)
{
	TArray<float> Keys;
	Colors.GetKeys(Keys);
	Keys.Sort();
	FLinearColor OutColor = Colors[Keys[0]];

	for (int i = Keys.Num() - 1; i >= 0; i--)
	{
		float Step = Keys[i];
		if (t > Step)
		{
			if (i + 1 == Keys.Num())
				return Colors[Step];

			const float NextStep = Keys[i + 1];
			const FLinearColor Color = Colors[Step];
			const FLinearColor NextColor = Colors[NextStep];
			const float a = (t - Step) / (NextStep - Step);
			OutColor = FMath::Lerp(Color,NextColor , a);
			break;
		}
	}
	
	return OutColor;
}
