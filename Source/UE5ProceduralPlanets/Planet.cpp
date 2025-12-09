#include "Planet.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
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

APlanet::~APlanet()
{
	if (AsyncMeshGenerator)
	{
		delete AsyncMeshGenerator;
		AsyncMeshGenerator = nullptr;
	}
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();

	if (EditorGenerated)
		return;
	
	DynamicTerrainMaterial = UMaterialInstanceDynamic::Create(BaseTerrainMaterial, this);
	GeneratePlanet();
}

void APlanet::GeneratePlanet()
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

void APlanet::GeneratePlanetAsync()
{
	if (AsyncMeshGenerator)
	{
		delete AsyncMeshGenerator;
		AsyncMeshGenerator = nullptr;
	}
	
	if (BuildTerrain)
	{
		Terrain->RandomizeTerrain();
		Environment->RandomizeBiomes();
		AsyncMeshGenerator = new MeshGenerator(PlanetMesh, Terrain, PlanetResolution, Radius);
	}
	else
		AsyncMeshGenerator = new MeshGenerator(PlanetMesh, PlanetResolution, Radius);

	AsyncMeshGenerator->StartAsyncGeneration();
}

void APlanet::UpdatePlanetAsync()
{
	if (AsyncMeshGenerator)
	{
		delete AsyncMeshGenerator;
		AsyncMeshGenerator = nullptr;
	}
	
	if (BuildTerrain)
		AsyncMeshGenerator = new MeshGenerator(PlanetMesh, Terrain, PlanetResolution, Radius);
	else
		AsyncMeshGenerator = new MeshGenerator(PlanetMesh, PlanetResolution, Radius);

	AsyncMeshGenerator->StartAsyncGeneration();
}

bool APlanet::IsGeneratingPlanet() const
{
	if (AsyncMeshGenerator && AsyncMeshGenerator->IsGenerating())
		return true;

	return bApplyingEnvironment;
}

void APlanet::GenerateInEditor()
{
	DynamicTerrainMaterial = UMaterialInstanceDynamic::Create(BaseTerrainMaterial, this);
	GeneratePlanet();
	EditorGenerated = true;
}

void APlanet::ApplyEnvironment() const
{
	SetUpMaterial();
	for (int i = 0 ; i < PlanetMesh->GetNumSections(); i++)
		SetUpUVs(i);
}

void APlanet::StartAsyncEnvironment()
{
	if (bApplyingEnvironment)
		return;

	bApplyingEnvironment = true;
	CurrentEnvironmentFace = 0;

	SetUpMaterial();
}

bool APlanet::UpdateAsyncEnvironment()
{
	if (!bApplyingEnvironment)
		return true;

	if (CurrentEnvironmentFace < PlanetMesh->GetNumSections())
	{
		SetUpUVs(CurrentEnvironmentFace);
		CurrentEnvironmentFace++;
		return false;
	}
	
	bApplyingEnvironment = false;
	CurrentEnvironmentFace = 0;
	return true;
}

void APlanet::SetUpMaterial() const
{
	Environment->WriteTextureOnDisk = WriteGradientTextureOnDisk;
	DynamicTerrainMaterial->SetScalarParameterValue(TEXT("Min"), Terrain->GetLowestElevation());
	DynamicTerrainMaterial->SetScalarParameterValue(TEXT("Max"), Terrain->GetHighestElevation());

	UTexture2D* Gradient = Environment->GenerateBiomesTexture();
	DynamicTerrainMaterial->SetTextureParameterValue(TEXT("Gradient"), Gradient);
}

void APlanet::SetUpUVs(int SectionId) const
{
	FProcMeshSection* Section = PlanetMesh->GetProcMeshSection(SectionId);
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
	
	PlanetMesh->UpdateMeshSection(SectionId, Positions, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>());
	PlanetMesh->SetMaterial(SectionId, DynamicTerrainMaterial);
}

void APlanet::UpdateRadius(float pRadius)
{
	Radius = pRadius;
	UpdatePlanetAsync();
}

void APlanet::SetVisible(const bool Visible) const
{
	PlanetMesh->SetVisibility(Visible);
	PlanetMesh->SetHiddenInGame(!Visible);
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AsyncMeshGenerator && AsyncMeshGenerator->IsGenerating())
	{
		if (AsyncMeshGenerator->UpdateAsyncGeneration())
		{
			delete AsyncMeshGenerator;
			AsyncMeshGenerator = nullptr;
			
			if (BuildTerrain)
				StartAsyncEnvironment();
			else
			{
				for (int i = 0; i < PlanetMesh->GetNumSections(); i++)
					PlanetMesh->SetMaterial(i, DynamicTerrainMaterial);
			}
		}
	}
	
	if (bApplyingEnvironment)
	{
		UpdateAsyncEnvironment();
	}
}



