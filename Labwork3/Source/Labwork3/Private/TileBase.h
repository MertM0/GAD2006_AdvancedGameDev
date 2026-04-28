#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TileBase.generated.h"

UCLASS()
class ATileBase : public AActor
{
	GENERATED_BODY()

public:
	ATileBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* BaseMesh;

	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* InstancedMesh;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};