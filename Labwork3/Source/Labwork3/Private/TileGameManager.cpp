#include "TileGameManager.h"
#include "TilePlayerController.h"
#include "GameFramework/PlayerInput.h"

ATileGameManager::ATileGameManager() : 
	GridSize(100),
	GridOffset(0,0,0.5f),
	MapExtendsInGrids(25)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TilePreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TilePreview"));
	TilePreviewMesh->SetupAttachment(GridSelection);

	TilePreviewMesh->SetStaticMesh(PlaneMesh.Object);
	TilePreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<ATilePlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;
	}

	TileScales.Empty();
	for (ATileBase* Tile : TileTypes)
	{
		if (Tile && Tile->InstancedMesh)
		{
			FVector OldScale = Tile->InstancedMesh->GetRelativeScale3D();
			TileScales.Add(OldScale);

			if (!OldScale.Equals(FVector(1.0f, 1.0f, 1.0f)))
			{
				for (int32 i = 0; i < Tile->InstancedMesh->GetInstanceCount(); ++i)
				{
					FTransform LocalTransform;
					Tile->InstancedMesh->GetInstanceTransform(i, LocalTransform, false);
					LocalTransform.SetScale3D(LocalTransform.GetScale3D() * OldScale);
					Tile->InstancedMesh->UpdateInstanceTransform(i, LocalTransform, false, false, true);
				}
				Tile->InstancedMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
		else
		{
			TileScales.Add(FVector(1.0f, 1.0f, 1.0f));
		}
	}

	ChangeCurrentTile(0);
}

void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
	if (TileTypes.Num() == 0) return;

	FVector GridLoc = GridOffset;
	GridLoc.X = FMath::GridSnap(Location.X, GridSize);
	GridLoc.Y = FMath::GridSnap(Location.Y, GridSize);
	GridLoc.Z = Location.Z;

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

			FTransform RotMatrix(FRotator(0, TileRotation, 0));

			FTransform TileTransform(GridLoc + GridOffset);

			FTransform InstanceTransform = RotMatrix * SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform;
			if (TileScales.IsValidIndex(CurrentTileIndex))
			{
				InstanceTransform.SetScale3D(TileScales[CurrentTileIndex]);
			}

			SelectedTile->InstancedMesh->AddInstance(InstanceTransform, true);
		}

		UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %f,%f,%f"),
			HitActor ? *HitActor->GetActorLabel() : TEXT("None"),
			Location.X,
			Location.Y,
			Location.Z);
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollDown))
	{
		ChangeCurrentTile(1);
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollUp))
	{
		ChangeCurrentTile(-1);
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else if (Input->WasJustPressed(EKeys::RightMouseButton))
	{
		TileRotation = (TileRotation + 90) % 360;
		ChangeCurrentTile(0);
	} 
	else 
	{
		GridSelection->SetWorldLocation(GridLoc + GridOffset);
		ChangeCurrentTile(0);
	}
}

void ATileGameManager::ChangeCurrentTile(int ScrollAmount)
{
	if (TileTypes.Num() == 0) return;

	if (ScrollAmount > 0)
	{
		CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();
	}
	else if (ScrollAmount < 0) 
	{
		CurrentTileIndex--;
		if (CurrentTileIndex < 0) CurrentTileIndex = TileTypes.Num() - 1;
	}

	TilePreviewMesh->SetStaticMesh(TileTypes[CurrentTileIndex]->BaseMesh);

	FTransform TileRelTransform = TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeTransform();
	FTransform TileSelRotTransform = FTransform(FRotator(0.0, TileRotation, 0.0));

	FTransform FinalTransform = TileSelRotTransform * TileRelTransform;
	if (TileScales.IsValidIndex(CurrentTileIndex))
	{
		FinalTransform.SetScale3D(TileScales[CurrentTileIndex]);
	}

	TilePreviewMesh->SetRelativeTransform(FinalTransform);
}