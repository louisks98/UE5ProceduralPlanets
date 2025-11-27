// Fill out your copyright notice in the Description page of Project Settings.


#include "Biome.h"

UBiome::UBiome()
{
	Colors.Add({0.0f, FLinearColor::White});
	StartHeight = 1.0f;
}

FLinearColor UBiome::EvaluateColor(const float t) const
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
