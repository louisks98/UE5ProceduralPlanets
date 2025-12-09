
#include "MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "StaticMeshAttributes.h"
#include "Engine/StaticMesh.h"
#include "UObject/SavePackage.h"
#include "AssetRegistry/AssetRegistryModule.h"


void MeshGenerator::GenerateTerrainMeshes() const
{
	Terrain->ResetElevation();
	Mesh->ClearAllMeshSections();
	for (int i = 0; i < 6; i++)
	{
		GenerateTerrainMesh(i, Directions[i]);
	}
}

void MeshGenerator::GenerateSimpleMeshes() const
{
	Mesh->ClearAllMeshSections();
	for (int i = 0; i < 6; i++)
	{
		GenerateSimpleMesh(i, Directions[i]);
	}
}

void MeshGenerator::GenerateTerrainMesh(const int SectionIndex, const FVector& LocalUp) const
{
	const FVector AxisX  = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	const FVector AxisY = FVector::CrossProduct(LocalUp, AxisX);
	
	TArray<FVector> Vertices;
	const TArray<int> Indices = GenerateMeshIndices();
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			const FVector2d Percent = FVector2d(x, y) / (Resolution - 1);
			FVector PointOnUnitCube = LocalUp + (Percent.X - 0.5f) * 2 * AxisX + (Percent.Y - 0.5f) * 2 * AxisY;
			FVector PointOnUnitSphere = PointOnUnitCube.GetSafeNormal();
			const float UnscaledElevation = Terrain->EvaluateUnscaledTerrain(PointOnUnitSphere);
			const float ScaledElevation = Radius * (FMath::Max(0, UnscaledElevation) + 1);
			Vertices.Add(PointOnUnitSphere * ScaledElevation);
			Normals.Add(PointOnUnitSphere);
			
			UVs.Add(FVector2D(0, UnscaledElevation));
		}
	}
	
	Mesh->CreateMeshSection(SectionIndex, Vertices, Indices, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

void MeshGenerator::GenerateSimpleMesh(const int SectionIndex, const FVector& LocalUp) const
{
	const FVector AxisX  = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	const FVector AxisY = FVector::CrossProduct(LocalUp, AxisX);
	
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	const TArray<int> Indices= GenerateMeshIndices();
	
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			const FVector2d Percent = FVector2d(x, y) / (Resolution - 1);
			FVector PointOnUnitCube = LocalUp + (Percent.X - 0.5f) * 2 * AxisX + (Percent.Y - 0.5f) * 2 * AxisY;
			FVector PointOnUnitSphere = PointOnUnitCube.GetSafeNormal();
			Vertices.Add(PointOnUnitSphere * Radius);
			Normals.Add(PointOnUnitSphere);
			
			UVs.Add(FVector2D(static_cast<float>(x) / (Resolution - 1), static_cast<float>(y) / (Resolution - 1)));
		}
	}
	
	Mesh->CreateMeshSection(SectionIndex, Vertices, Indices, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

TArray<int> MeshGenerator::GenerateMeshIndices() const
{
	TArray<int> Indices;
	for (int y = 0; y < Resolution - 1; y++)
	{
		for (int x = 0; x < Resolution - 1; x++)
		{
			const int i = x + y * Resolution;
			
			Indices.Add(i);
			Indices.Add(i + Resolution);
			Indices.Add(i + Resolution + 1);
			Indices.Add(i);
			Indices.Add(i + Resolution + 1);
			Indices.Add(i + 1);
		}
	}
	
	return Indices;
}