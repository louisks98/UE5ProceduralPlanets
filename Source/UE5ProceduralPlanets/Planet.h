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
	virtual ~APlanet() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet mesh")
	int PlanetResolution;
	UPROPERTY(EditAnywhere, Category = "Planet mesh")
	UProceduralMeshComponent* PlanetMesh;
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

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUG")
	bool WriteGradientTextureOnDisk = false;
	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateInEditor();
#endif
	
	UFUNCTION(BlueprintCallable)
	void UpdateRadius(float pRadius);
	
	UFUNCTION(BlueprintCallable)
	void SetVisible(bool Visible) const;
	
	UFUNCTION(BlueprintCallable)
	void GeneratePlanet();
	UFUNCTION(BlueprintCallable)
	void GeneratePlanetAsync();
	UFUNCTION(BlueprintCallable)
	void UpdatePlanetAsync();
	UFUNCTION(BlueprintCallable)
	bool IsGeneratingPlanet() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool EditorGenerated = false;
	MeshGenerator* AsyncMeshGenerator = nullptr;
	
	bool bApplyingEnvironment = false;
	int CurrentEnvironmentFace = 0;

	void ApplyEnvironment() const;
	void StartAsyncEnvironment();
	bool UpdateAsyncEnvironment();
	void SetUpMaterial() const;
	void SetUpUVs(int SectionId) const;
};
