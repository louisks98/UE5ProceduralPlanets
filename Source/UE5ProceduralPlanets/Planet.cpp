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
	
	Resolution = 10;
	Radius = 100;
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();
	DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
	GeneratePlanet();
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void APlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, Resolution))
		{
			GeneratePlanet();
		}
	}
}
#endif

void APlanet::GeneratePlanet(bool Update) const
{
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
		GenerateMesh(i, Directions[i], Update);
	}
}

void APlanet::GenerateMesh(const int SectionIndex, const FVector& LocalUp, const bool Update) const
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
			Vertices.Add(PointOnUnitSphere * Radius);
			Normals.Add(PointOnUnitSphere);
			
			UVs.Add(FVector2D(static_cast<float>(x) / (Resolution - 1), static_cast<float>(y) / (Resolution - 1)));
		}
	}

	if (!Update)
	{
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
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Section %d: %d vertices, %d triangles"), SectionIndex, Vertices.Num(), Indices.Num() / 3);
	if (Update)
		Mesh->UpdateMeshSection(SectionIndex, Vertices, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>());
	else
		Mesh->CreateMeshSection(SectionIndex, Vertices, Indices, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	
	Mesh->SetMaterial(SectionIndex, DynamicMaterial);
}

void APlanet::UpdateRadius(float pRadius)
{
	Radius = pRadius;
	UE_LOG(LogTemp, Warning, TEXT("Changed radius to: %f"), Radius);
	GeneratePlanet();
}

void APlanet::UpdateColor(FLinearColor pColor)
{
	Color = pColor;
	UE_LOG(LogTemp, Warning, TEXT("Changed color to: 0x%08X"), Color.ToFColorSRGB().ToPackedARGB());
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);

	for (int i = 0; i < 6; i++)
		Mesh->SetMaterial(i, DynamicMaterial);

}

