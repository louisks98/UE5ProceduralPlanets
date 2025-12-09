#pragma once

#include "CoreMinimal.h"
#include "Environment.h"
#include "MeshGenerator.h"
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
	int PlanetResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet mesh")
	UMaterialInterface* BaseTerrainMaterial;
	UPROPERTY(BlueprintReadOnly, Category = "Planet mesh")
	UMaterialInstanceDynamic* DynamicTerrainMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet settings")
	bool BuildTerrain = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet settings")
	float Radius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Planet settings")
	UTerrainComponent* Terrain;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Planet settings")
	UEnvironment* Environment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet mesh")
	UProceduralMeshComponent* PlanetMesh;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUG")
	bool WriteGradientTextureOnDisk = false;
	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateInEditor();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ConvertToStaticMesh();
#endif
	
	UFUNCTION(BlueprintCallable)
	void UpdateRadius(float pRadius);
	
	UFUNCTION()
	void SetVisible(bool Visible) const;
	
	UFUNCTION(BlueprintCallable)
	void GeneratePlanet() const;
	UFUNCTION(BlueprintCallable)
	void UpdatePlanet() const;
	
protected:
	virtual void BeginPlay() override;
	
private:
	bool EditorGenerated = false;
	
	void ApplyEnvironment() const;
};
