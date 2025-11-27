// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Biome.h"
#include "Environment.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TerrainComponent.h"
#include "Planet.generated.h"

UCLASS()
class UE5PROCEDURALPLANETS_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:
	APlanet();
	
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet mesh")
	int Resolution;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet mesh")
	UMaterialInterface* BaseMaterial;
UPROPERTY(BlueprintReadOnly, Category = "Planet mesh")
	UMaterialInstanceDynamic* DynamicMaterial;
	
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet settings")
	FLinearColor Color;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet settings")
	float Radius;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet settings")
	UTerrainComponent* Terrain;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet settings")
	UEnvironment* Environment;
	
UFUNCTION(BlueprintCallable)
	void UpdateRadius(float pRadius);
	
UFUNCTION(BlueprintCallable)
	void GeneratePlanet() const;
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	UProceduralMeshComponent* Mesh;
	
	void GenerateMesh(int SectionIndex, const FVector& LocalUp) const;
	void ApplyEnvironment() const;
};
