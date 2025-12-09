#include "Planet.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = true;
	PlanetMesh = CreateDefaultSubobject<UProceduralMeshComponent>("PlanetMesh");
	PlanetMesh->SetCanEverAffectNavigation(false);
	RootComponent = PlanetMesh;
	PlanetMesh->SetVisibility(true);
	PlanetMesh->SetHiddenInGame(false);
	PlanetMesh->bCastDynamicShadow = true;
	Environment = CreateDefaultSubobject<UEnvironment>("Environment");
	
	if (BuildTerrain)
		Terrain = CreateDefaultSubobject<UTerrainComponent>("Terrain");
	
	PlanetResolution = 100;
	Radius = 100;
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();

	if (EditorGenerated)
		return;
	
	DynamicTerrainMaterial = UMaterialInstanceDynamic::Create(BaseTerrainMaterial, this);
	GeneratePlanet();
}

void APlanet::GeneratePlanet() const
{
	if (BuildTerrain)
	{
		const MeshGenerator Generator = MeshGenerator(PlanetMesh, Terrain, PlanetResolution, Radius);
		Generator.GenerateTerrainMeshes();
		Terrain->RandomizeTerrain();
		Environment->RandomizeBiomes();
		ApplyEnvironment();
	}
	else
	{
		const MeshGenerator Generator = MeshGenerator(PlanetMesh, PlanetResolution, Radius);
		Generator.GenerateSimpleMeshes();

		for (int i = 0; i < PlanetMesh->GetNumSections(); i++)
		{
			PlanetMesh->SetMaterial(i, DynamicTerrainMaterial);
		}
	}
	
}

void APlanet::UpdatePlanet() const
{
	if (BuildTerrain)
	{
		const MeshGenerator Generator(PlanetMesh, Terrain, PlanetResolution, Radius);
		Generator.GenerateTerrainMeshes();
		ApplyEnvironment();
	}
	else
	{
		const MeshGenerator Generator = MeshGenerator(PlanetMesh, PlanetResolution, Radius);
		Generator.GenerateSimpleMeshes();

		for (int i = 0; i < PlanetMesh->GetNumSections(); i++)
		{
			PlanetMesh->SetMaterial(i, DynamicTerrainMaterial);
		}
	}
}

void APlanet::GenerateInEditor()
{
	DynamicTerrainMaterial = UMaterialInstanceDynamic::Create(BaseTerrainMaterial, this);
	GeneratePlanet();
	EditorGenerated = true;
}

void APlanet::ConvertToStaticMesh()
{
	MeshGenerator::GenerateStaticMesh(PlanetMesh, TEXT("/Game/Mesh/PlanetStaticMesh"));
}

void APlanet::ApplyEnvironment() const
{
	Environment->WriteTextureOnDisk = WriteGradientTextureOnDisk;
	DynamicTerrainMaterial->SetScalarParameterValue(TEXT("Min"), Terrain->GetLowestElevation());
	DynamicTerrainMaterial->SetScalarParameterValue(TEXT("Max"), Terrain->GetHighestElevation());
	
	UTexture2D* Gradient = Environment->GenerateBiomesTexture();
	DynamicTerrainMaterial->SetTextureParameterValue(TEXT("Gradient"), Gradient);
	
	for (int i = 0 ; i < PlanetMesh->GetNumSections(); i++)
	{
		FProcMeshSection* Section = PlanetMesh->GetProcMeshSection(i);
		TArray<FProcMeshVertex>& Vertices = Section->ProcVertexBuffer;
		TArray<FVector2D> UVs;
		TArray<FVector> Normals;
		TArray<FVector> Positions;
	
		for (const FProcMeshVertex& Vertex : Vertices)
		{
			const FVector2D UV = Vertex.UV0;
			const float BiomePercent = Environment->FindBiomePercentageFromPoint(Vertex.Position, Vertex.Normal);
			UVs.Add(FVector2D(BiomePercent, UV.Y));
			Normals.Add(Vertex.Normal);
			Positions.Add(Vertex.Position);
		}
	
		PlanetMesh->UpdateMeshSection(i, Positions, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>());
		PlanetMesh->SetMaterial(i, DynamicTerrainMaterial);
	}
}

void APlanet::UpdateRadius(float pRadius)
{
	Radius = pRadius;
	UpdatePlanet();
}

void APlanet::SetVisible(const bool Visible) const
{
	PlanetMesh->SetVisibility(Visible);
	PlanetMesh->SetHiddenInGame(!Visible);
}

