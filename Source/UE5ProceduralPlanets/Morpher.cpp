#include "Morpher.h"

AMorpher::AMorpher()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMorpher::MorphIn()
{
	const auto MeshA = PlanetA->PlanetMesh;
	const auto MeshB = PlanetB->PlanetMesh;

	float Min, Max;
	UTexture* Gradient;
	PlanetA->DynamicTerrainMaterial->GetScalarParameterValue(FName("Min"), Min);
	PlanetA->DynamicTerrainMaterial->GetScalarParameterValue(FName("Max"), Max);
	PlanetA->DynamicTerrainMaterial->GetTextureParameterValue(FName("Gradient"), Gradient);

	MorphInMaterialInstanceA->SetScalarParameterValue(FName("Min"), Min);
	MorphInMaterialInstanceA->SetScalarParameterValue(FName("Max"), Max);
	MorphInMaterialInstanceA->SetTextureParameterValue(FName("Gradient"), Gradient);
	
	MorphInMaterialInstanceA->SetScalarParameterValue(FName("VanishingValue"), 0.0f);
	MorphOutMaterialInstanceB->SetScalarParameterValue(FName("VanishingValue"), 1.0f);

	int NumSectionA = MeshA->GetNumSections();
	int NumSectionB = MeshB->GetNumSections();
	
	for (int i = 0; i < NumSectionA; i++)
		MeshA->SetMaterial(i, MorphInMaterialInstanceA);

	for (int i = 0; i < NumSectionB; i++)
		MeshB->SetMaterial(i, MorphOutMaterialInstanceB);

	MorphingIn = true;
	PlanetA->SetVisible(true);
}

void AMorpher::MorphOut()
{
	const auto MeshA = PlanetA->PlanetMesh;
	const auto MeshB = PlanetB->PlanetMesh;

	float Min, Max;
	UTexture* Gradient;
	PlanetA->DynamicTerrainMaterial->GetScalarParameterValue(FName("Min"), Min);
	PlanetA->DynamicTerrainMaterial->GetScalarParameterValue(FName("Max"), Max);
	PlanetA->DynamicTerrainMaterial->GetTextureParameterValue(FName("Gradient"), Gradient);
	
	MorphOutMaterialInstanceA->SetScalarParameterValue(FName("Min"), Min);
	MorphOutMaterialInstanceA->SetScalarParameterValue(FName("Max"), Max);
	MorphOutMaterialInstanceA->SetTextureParameterValue(FName("Gradient"), Gradient);

	MorphOutMaterialInstanceA->SetScalarParameterValue(FName("VanishingValue"), 1.0f);
	MorphInMaterialInstanceB->SetScalarParameterValue(FName("VanishingValue"), 0.0f);

	const int NumSectionA = MeshA->GetNumSections();
	const int NumSectionB = MeshB->GetNumSections();
	for (int i  = 0; i < NumSectionA; i++)
		MeshA->SetMaterial(i, MorphOutMaterialInstanceA);

	for (int i = 0 ; i < NumSectionB; i++)
		MeshB->SetMaterial(i, MorphInMaterialInstanceB);

	MorphingOut = true;
	PlanetB->SetVisible(true);
}

void AMorpher::MorphInEditor()
{
	MorphIn();
}

void AMorpher::MorphOutEditor()
{
	MorphOut();
}

void AMorpher::BeginPlay()
{
	Super::BeginPlay();
	BaseMaterialA = PlanetA->DynamicTerrainMaterial;
	BaseMaterialB = PlanetB->DynamicTerrainMaterial;

	MorphOutMaterialInstanceA = UMaterialInstanceDynamic::Create(MorphOutMaterialA, this);
	MorphInMaterialInstanceA = UMaterialInstanceDynamic::Create(MorphInMaterialA, this);
	MorphOutMaterialInstanceB = UMaterialInstanceDynamic::Create(MorphOutMaterialB, this);
	MorphInMaterialInstanceB = UMaterialInstanceDynamic::Create(MorphInMaterialB, this);
}

void AMorpher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MorphingOut || MorphingIn)
	{
		MorphingValue += DeltaTime * 0.5f;
		if (MorphingValue > 1.0f)
		{
			auto MeshA = PlanetA->PlanetMesh;
			auto MeshB = PlanetB->PlanetMesh;
			int NumSectionA = MeshA->GetNumSections();
			int NumSectionB = MeshB->GetNumSections();
			if (MorphingIn)
			{
				for (int i = 0; i < NumSectionA; i++)
					MeshA->SetMaterial(i, BaseMaterialA);
				
				PlanetB->SetVisible(false);
			}

			if (MorphingOut)
			{
				for (int i = 0; i < NumSectionB; i++)
					MeshB->SetMaterial(i, BaseMaterialB);
				
				PlanetA->SetVisible(false);
			}
			
			MorphingIn = false;
			MorphingOut =  false;
			MorphingValue = 0.0f;
		}

		if (MorphingOut)
		{
			MorphOutMaterialInstanceA->SetScalarParameterValue(FName("VanishingValue"), 1.0f - MorphingValue);
			MorphInMaterialInstanceB->SetScalarParameterValue(FName("VanishingValue"), MorphingValue);
		}
		if (MorphingIn)
		{
			MorphInMaterialInstanceA->SetScalarParameterValue(FName("VanishingValue"), MorphingValue);
			MorphOutMaterialInstanceB->SetScalarParameterValue(FName("VanishingValue"), 1.0f - MorphingValue);
		}
	}
}


