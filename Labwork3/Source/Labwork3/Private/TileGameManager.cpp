#include "TileGameManager.h"
#include "TilePlayerController.h"
#include "GameFramework/PlayerInput.h"

ATileGameManager::ATileGameManager() :
	GridSize(100),
	GridOffset(0, 0, 0.5f),
	MapExtendsInGrids(25),
	CurrentTileIndex(0)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TilePreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TilePreview"));
	TilePreview->SetupAttachment(GridSelection);
	TilePreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentTileRotation = FRotator::ZeroRotator;
}

void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<ATilePlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;
	}

	if (TileTypes.Num() > 0 && TileTypes[0] != nullptr)
	{
		TilePreview->SetStaticMesh(TileTypes[0]->BaseMesh);
		TilePreview->SetRelativeTransform(TileTypes[0]->InstancedMesh->GetRelativeTransform() * FTransform(CurrentTileRotation));
	}
}

void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
	if (TileTypes.Num() == 0) return;

	FVector GridLoc = GridOffset;
	GridLoc.X += FMath::GridSnap(Location.X, GridSize);
	GridLoc.Y += FMath::GridSnap(Location.Y, GridSize);
	GridLoc.Z += Location.Z;

	UPlayerInput* Input = GWorld->GetFirstPlayerController()->PlayerInput;

	if (Input->WasJustPressed(EKeys::LeftMouseButton))
	{
		int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
		int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

		if (GridX < 0 || GridY < 0 || GridX >= MapExtendsInGrids * 2 || GridY >= MapExtendsInGrids * 2) return;
		if (Map[GridX][GridY] != nullptr) return;

		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
			Map[GridX][GridY] = SelectedTile;

			FTransform TileTransform(CurrentTileRotation, GridLoc + GridOffset);

			SelectedTile->InstancedMesh->AddInstance(
				SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform,
				true);

			UE_LOG(LogTemp, Warning, TEXT("Hit: %s %f, %f, %f"),
				HitActor ? *HitActor->GetActorLabel() : TEXT("None"),
				Location.X, Location.Y, Location.Z);
		}
	}
	else if (Input->WasJustPressed(EKeys::RightMouseButton))
	{
		CurrentTileRotation.Yaw += 90.0f;
		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			TilePreview->SetRelativeTransform(TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeTransform() * FTransform(CurrentTileRotation));
		}
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollDown))
	{
		CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();

		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			TilePreview->SetStaticMesh(TileTypes[CurrentTileIndex]->BaseMesh);
			TilePreview->SetRelativeTransform(TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeTransform() * FTransform(CurrentTileRotation));
		}
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollUp))
	{
		CurrentTileIndex--;
		if (CurrentTileIndex < 0)
		{
			CurrentTileIndex = TileTypes.Num() - 1;
		}

		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			TilePreview->SetStaticMesh(TileTypes[CurrentTileIndex]->BaseMesh);
			TilePreview->SetRelativeTransform(TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeTransform() * FTransform(CurrentTileRotation));
		}
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else
	{
		GridSelection->SetWorldLocation(GridLoc + GridOffset);
	}
}