#pragma once

#include "CoreMinimal.h"
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
	UMaterialInterface* BaseTerrainMaterial;
UPROPERTY(BlueprintReadOnly, Category = "Planet mesh")
	UMaterialInstanceDynamic* DynamicTerrainMaterial;
	
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet settings")
	FLinearColor Color;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet settings")
	float Radius;
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Planet settings")
	UTerrainComponent* Terrain;
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Planet settings")
	UEnvironment* Environment;
	
UFUNCTION(BlueprintCallable)
	void UpdateRadius(float pRadius);
	
UFUNCTION(BlueprintCallable)
	void GeneratePlanet() const;
UFUNCTION(BlueprintCallable)
	void UpdatePlanet() const;
protected:
	virtual void BeginPlay() override;
	
private:
	const FVector Directions[6] = {
		FVector::UpVector,
		FVector::ForwardVector,
		FVector::BackwardVector,
		FVector::DownVector,
		FVector::LeftVector,
		FVector::RightVector
	};
	
	UPROPERTY()
	UProceduralMeshComponent* Mesh;
	
	void GenerateMeshes() const;
	void GenerateMesh(int SectionIndex, const FVector& LocalUp) const;
	void ApplyEnvironment() const;
};
