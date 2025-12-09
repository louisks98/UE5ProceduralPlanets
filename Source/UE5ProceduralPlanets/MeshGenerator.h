#pragma once

#include "CoreMinimal.h"
#include "TerrainComponent.h"
#include "MeshDescription.h"


class UProceduralMeshComponent;

class UE5PROCEDURALPLANETS_API MeshGenerator
{
public:
	MeshGenerator(UProceduralMeshComponent* Mesh, UTerrainComponent* Terrain, const int Resolution, const float Radius)
		: Mesh(Mesh),
		  Terrain(Terrain),
		  Resolution(Resolution),
		  Radius(Radius)
	{}

	MeshGenerator(UProceduralMeshComponent* Mesh, const int Resolution, const float Radius)
		: Mesh(Mesh),
		  Terrain(nullptr),
		  Resolution(Resolution),
		  Radius(Radius)
	{}
	
	void GenerateTerrainMeshes() const;
	void GenerateSimpleMeshes() const;
	
	void StartAsyncGeneration() const;
	bool UpdateAsyncGeneration() const;
	bool IsGenerating() const { return bIsGenerating; }

private:
	const FVector Directions[6] = {
		FVector::UpVector,
		FVector::ForwardVector,
		FVector::BackwardVector,
		FVector::DownVector,
		FVector::LeftVector,
		FVector::RightVector
	};

	UProceduralMeshComponent* Mesh;
	UTerrainComponent* Terrain;
	int Resolution;
	int Radius;
	
	mutable bool bIsGenerating = false;
	mutable int CurrentFace = 0;

	void GenerateTerrainMesh(int SectionIndex, const FVector& LocalUp) const;
	void GenerateSimpleMesh(int SectionIndex, const FVector& LocalUp) const;
	TArray<int> GenerateMeshIndices() const;
	
};
