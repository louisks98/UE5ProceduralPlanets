// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperBlueprintFunctionLibrary.h"

FLinearColor UHelperBlueprintFunctionLibrary::HexToColor(FString HexString)
{
	return FColor::FromHex(HexString);
}

FString UHelperBlueprintFunctionLibrary::ColorToHex(FLinearColor Color)
{
	return Color.ToFColorSRGB().ToHex();
}