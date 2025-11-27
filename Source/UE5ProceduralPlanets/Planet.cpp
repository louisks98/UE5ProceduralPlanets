// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetCanEverAffectNavigation(false);
	RootComponent = Mesh;
	Mesh->SetVisibility(true);
	Mesh->SetHiddenInGame(false);
	Terrain = CreateDefaultSubobject<UTerrainComponent>("Terrain");
	Environment = CreateDefaultSubobject<UEnvironment>("Environment");

	Resolution = 100;
	Radius = 100;
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();
	Environment->RandomizeBiomes();

	DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
}

void APlanet::GeneratePlanet() const
{
	Terrain->ResetElevation();
	const FVector Directions[6] = {
		FVector::UpVector,
		FVector::ForwardVector,
		FVector::BackwardVector,
		FVector::DownVector,
		FVector::LeftVector,
		FVector::RightVector
	};

	Mesh->ClearAllMeshSections();
	for (int i = 0; i < 6; i++)
	{
		GenerateMesh(i, Directions[i]);
	}

	ApplyEnvironment();
}

void APlanet::GenerateMesh(const int SectionIndex, const FVector& LocalUp) const
{
	const FVector AxisX  = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	const FVector AxisY = FVector::CrossProduct(LocalUp, AxisX);
	
	TArray<FVector> Vertices;
	TArray<int> Indices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			const FVector2d Percent = FVector2d(x, y) / (Resolution - 1);
			FVector PointOnUnitCube = LocalUp + (Percent.X - 0.5f) * 2 * AxisX + (Percent.Y - 0.5f) * 2 * AxisY;
			FVector PointOnUnitSphere = PointOnUnitCube.GetSafeNormal();
			Vertices.Add(Terrain->EvaluateTerrain(PointOnUnitSphere, Radius));
			Normals.Add(PointOnUnitSphere);
			
			UVs.Add(FVector2D(static_cast<float>(x) / (Resolution - 1), static_cast<float>(y) / (Resolution - 1)));
		}
	}

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
	
	Mesh->CreateMeshSection(SectionIndex, Vertices, Indices, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	Mesh->SetMaterial(SectionIndex, DynamicMaterial);
}

void APlanet::ApplyEnvironment() const
{
	DynamicMaterial->SetScalarParameterValue(TEXT("Min"), Terrain->GetLowestElevation());
	DynamicMaterial->SetScalarParameterValue(TEXT("Max"), Terrain->GetHighestElevation());
	
	UTexture2D* Gradient = Environment->GenerateBiomesTexture();
	DynamicMaterial->SetTextureParameterValue(TEXT("Gradient"), Gradient);
	
	for (int i = 0 ; i < Mesh->GetNumSections(); i++)
	{
		FProcMeshSection* Section = Mesh->GetProcMeshSection(i);
		TArray<FProcMeshVertex>& Vertices = Section->ProcVertexBuffer;
		TArray<FVector2D> UVs;
		TArray<FVector> Normals;
		TArray<FVector> Positions;
	
		for (const FProcMeshVertex& Vertex : Vertices)
		{
			FVector Normal = Vertex.Normal;
			float BiomePercent = Environment->FindBiomePercentageFromPoint(Normal);
			UVs.Add(FVector2D(BiomePercent, 0));
			Normals.Add(Normal);
			Positions.Add(Vertex.Position);
		}
	
		Mesh->UpdateMeshSection(i, Positions, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>());
	}
}

void APlanet::UpdateRadius(float pRadius)
{
	Radius = pRadius;
	GeneratePlanet();
}
