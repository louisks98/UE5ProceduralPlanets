// Fill out your copyright notice in the Description page of Project Settings.


#include "Biome.h"

UBiome::UBiome()
{
	Color1 = FLinearColor(0.125f, 0.474f, 1.0f, 1.0f);
	Color2 = FLinearColor(0.0369f, 0.136f, 0.0307f, 1.0f);
	Color3 = FLinearColor(0.147f, 0.283f, 0.0452f, 1.0f);
	Color4 = FLinearColor(0.474f, 0.262f, 0.0296f, 1.0f);
	Color5 = FLinearColor(0.25f, 0.138f, 0.016f, 1.0f);
	Color6 = FLinearColor::White;
	StartHeight = 1.0f;
}

FLinearColor UBiome::EvaluateGradient(const float t) const
{
	FLinearColor OutColor = Color1;
	if(t > 0.01)
	{
		const float a = (t - 0.01) / (0.096 - 0.01);
		OutColor = FMath::Lerp(Color1, Color2, a);
	}
	if(t > 0.096)
	{
		const float a = (t - 0.096) / (0.306 - 0.096);
		OutColor = FMath::Lerp(Color2, Color3, a);
	}
	if(t > 0.306)
	{
		const float a = (t - 0.306) / (0.595 - 0.306);
		OutColor = FMath::Lerp(Color3, Color4, a);
	}
	if(t > 0.595)
	{
		const float a = (t - 0.595) / (0.772 - 0.595);
		OutColor = FMath::Lerp(Color4, Color5, a);
	}
	if(t > 0.772)
	{
		const float a = (t - 0.772) / (0.885 - 0.772);
		OutColor = FMath::Lerp(Color5, Color6, a);
	}
	if(t > 0.885)
	{
		OutColor = Color6;
	}

	return OutColor;
}
