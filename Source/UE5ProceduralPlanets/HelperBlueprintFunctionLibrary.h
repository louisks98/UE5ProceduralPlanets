// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelperBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROCEDURALPLANETS_API UHelperBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="HelperFunctionLibrary")
	static FLinearColor HexToColor(FString HexString);
	
	UFUNCTION(BlueprintCallable, Category="HelperFunctionLibrary")
	static FString ColorToHex(FLinearColor Color);
};
