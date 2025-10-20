// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TerrainComponent.h"
#include "Planet.generated.h"

UCLASS()
class UE5PROCEDURALPLANETS_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet mesh")
	int Resolution;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet mesh")
	UMaterialInterface* BaseMaterial;
UPROPERTY(BlueprintReadOnly, Category = "Planet mesh")
	UMaterialInstanceDynamic* DynamicMaterial;
	
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet settings")
	FLinearColor Color;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet settings")
	float Radius;
UPROPERTY()
	UTerrainComponent* Terrain;

UFUNCTION(BlueprintCallable)
	void UpdateColor(FLinearColor pColor);
UFUNCTION(BlueprintCallable)
	void UpdateRadius(float pRadius);
	
	APlanet();
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	UProceduralMeshComponent* Mesh;
	
	void GeneratePlanet(bool Update = false) const;
	void GenerateMesh(int SectionIndex, const FVector& LocalUp, bool Update = false) const;
};
