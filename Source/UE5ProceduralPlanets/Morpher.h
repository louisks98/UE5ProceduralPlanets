#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "Planet.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Morpher.generated.h"

UCLASS()
class UE5PROCEDURALPLANETS_API AMorpher : public AActor
{
	GENERATED_BODY()
	
public:	
	AMorpher();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* MorphSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MorphOutMaterialA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MorphInMaterialA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MorphOutMaterialB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MorphInMaterialB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* FireOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlanet* PlanetA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlanet* PlanetB;

	UFUNCTION(BlueprintCallable)
	void MorphIn();
	UFUNCTION(BlueprintCallable)
	void MorphOut();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void MorphInEditor();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void MorphOutEditor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* BaseMaterialA;
	UPROPERTY()
	UMaterialInstanceDynamic* BaseMaterialB;

	UPROPERTY()
	UMaterialInstanceDynamic* MorphOutMaterialInstanceA;
	UPROPERTY()
	UMaterialInstanceDynamic* MorphInMaterialInstanceA;
	UPROPERTY()
	UMaterialInstanceDynamic* MorphOutMaterialInstanceB;
	UPROPERTY()
	UMaterialInstanceDynamic* MorphInMaterialInstanceB;

	UPROPERTY()
	UMaterialInstanceDynamic* FireOverlayMaterialInstance;

	bool MorphingIn = false;
	bool MorphingOut = false;
	float MorphingValue = 0.0f;
};
