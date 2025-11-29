#include "Planet.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetCanEverAffectNavigation(false);
	RootComponent = Mesh;
	Mesh->SetVisibility(true);
	Mesh->SetHiddenInGame(false);
	Mesh->bCastDynamicShadow = true;
	Terrain = CreateDefaultSubobject<UTerrainComponent>("Terrain");
	Environment = CreateDefaultSubobject<UEnvironment>("Environment");

	Resolution = 100;
	Radius = 100;
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();
	
	DynamicTerrainMaterial = UMaterialInstanceDynamic::Create(BaseTerrainMaterial, this);
	DynamicTerrainMaterial->SetVectorParameterValue(TEXT("Color"), Color);
	
	GeneratePlanet();
}

void APlanet::GeneratePlanet() const
{
	GenerateMeshes();
	Terrain->RandomizeTerrain();
	Environment->RandomizeBiomes();
	ApplyEnvironment();
}

void APlanet::UpdatePlanet() const
{
	GenerateMeshes();
	ApplyEnvironment();
}

void APlanet::GenerateMeshes() const
{
	Terrain->ResetElevation();
	Mesh->ClearAllMeshSections();
	for (int i = 0; i < 6; i++)
	{
		GenerateMesh(i, Directions[i]);
	}
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
			const float UnscaledElevation = Terrain->EvaluateUnscaledTerrain(PointOnUnitSphere);
			const float ScaledElevation = Radius * (FMath::Max(0, UnscaledElevation) + 1);
			Vertices.Add(PointOnUnitSphere * ScaledElevation);
			Normals.Add(PointOnUnitSphere);
			
			UVs.Add(FVector2D(0, UnscaledElevation));
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
	Mesh->SetMaterial(SectionIndex, DynamicTerrainMaterial);
}

void APlanet::ApplyEnvironment() const
{
	DynamicTerrainMaterial->SetScalarParameterValue(TEXT("Min"), Terrain->GetLowestElevation());
	DynamicTerrainMaterial->SetScalarParameterValue(TEXT("Max"), Terrain->GetHighestElevation());
	
	UTexture2D* Gradient = Environment->GenerateBiomesTexture();
	DynamicTerrainMaterial->SetTextureParameterValue(TEXT("Gradient"), Gradient);
	
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
			const FVector2D UV = Vertex.UV0;
			float BiomePercent = Environment->FindBiomePercentageFromPoint(Normal);
			UVs.Add(FVector2D(BiomePercent, UV.Y));
			Normals.Add(Normal);
			Positions.Add(Vertex.Position);
		}
	
		Mesh->UpdateMeshSection(i, Positions, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>());
	}
}

void APlanet::UpdateRadius(float pRadius)
{
	Radius = pRadius;
	UpdatePlanet();
}
