#include "GameSlot.h"
#include "UnitBase.h"
#include "TBPlayerController.h"

AGameSlot::AGameSlot()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetBoxExtent(FVector(50, 50, 2));
    Box->SetCollisionResponseToAllChannels(ECR_Block);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));
    Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
    Plane->SetupAttachment(RootComponent);
    Plane->SetStaticMesh(DefaultSlotMesh.Object);

	SetState(GS_Default);
}

void AGameSlot::SetState(EGridState NewState)
{
    GridState = NewState;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        DefaultSlotMaterial(TEXT("/Script/Engine.Material'/Game/Materials/MAT_GridSlot.MAT_GridSlot'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        DefaultSlotMaterial_H(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Highlighted.MAT_GridSlot_Highlighted'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        DefaultSlotMaterial_O(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Offensive.MAT_GridSlot_Offensive'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        DefaultSlotMaterial_S(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Supportive.MAT_GridSlot_Supportive'"));

    switch (NewState)
    {
    case GS_Default:
        Plane->SetMaterial(0, DefaultSlotMaterial.Object);
        break;
    case GS_Highlighted:
        Plane->SetMaterial(0, DefaultSlotMaterial_H.Object);
        break;
    case GS_Offensive:
        Plane->SetMaterial(0, DefaultSlotMaterial_O.Object);
        break;
    case GS_Supportive:
        Plane->SetMaterial(0, DefaultSlotMaterial_S.Object);
        break;
    }
}

void AGameSlot::BeginPlay()
{
    Super::BeginPlay();

    OnClicked.AddUniqueDynamic(this, &AGameSlot::OnGridClicked);

    SetState(GS_Default);
}

void AGameSlot::SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass)
{
    FVector Location = GetActorLocation();
    AUnitBase* NewUnit = Cast<AUnitBase>(GWorld->SpawnActor(UnitClass, &Location));

    if (NewUnit) NewUnit->AssignToSlot(this);
}

void AGameSlot::OnGridClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    if (auto PlayerController = GWorld->GetFirstPlayerController<ATBPlayerController>())
        PlayerController->OnActorClicked(this, ButtonPressed);
}

void AGameSlot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}