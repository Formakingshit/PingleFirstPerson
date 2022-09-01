#include "SphereTargetHubActor.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Sets default values
ASphereTargetHubActor::ASphereTargetHubActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASphereTargetHubActor::BeginPlay()
{
    Super::BeginPlay();

    SpawnSphereTargetActors();
}

// Called every frame
void ASphereTargetHubActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

float ASphereTargetHubActor::GenerateRandomCoordinate(float FirstCoordinate, float SecondCoordinate, float ActorLocationFirstCoordinate,
    float ActorLocationSecondCoordinate, float ActorLocationCoordinate, float FromWallCoordinate, float ToWallCoordinate, float RadiusLimit)
{
    // Math info about Quadratic Equation (PSEVDOCODE)
    // (X1 - X2)^2 = RadiusLimit
    // (X1 - X2)^2 = X1^2 - 2*X1*X2 + X2^2
    //
    // X1^2 - 2*X1*X2 + X2^2 = RadiusLimit
    // X1^2 - 2*X1*X2 + X2^2 - RadiusLimit = 0  ||  X1 = ActorLocationCoordinate
    //
    // ActorLocationCoordinate^2 - 2*ActorLocationCoordinate*X2 + X2^2 - RadiusLimit = 0;
    //
    //
    // Math info about Discriminant
    // a*X^2 + b*X + c = 0
    // D = b^2 - 4*a*c
    //
    // X1 = (-b + sqrt(D)) / 2*a
    // X2 = (-b - sqrt(D)) / 2*a

    float InRadius = FMath::Pow(RadiusLimit, 2) - FMath::Pow(ActorLocationFirstCoordinate - FirstCoordinate, 2) -
                     FMath::Pow(ActorLocationSecondCoordinate - SecondCoordinate, 2);

    float A = 1;
    float B = -2 * ActorLocationCoordinate;
    float C = FMath::Pow(ActorLocationCoordinate, 2) - InRadius;

    float D = FMath::Pow(B, 2) - 4 * A * C;

    float X1 = (-B + FMath::Sqrt(D)) / 2 * A;
    float X2 = (-B - FMath::Sqrt(D)) / 2 * A;

    float FromInRadius = 0;
    float ToInRadius = 0;

    if (X1 > X2)
    {
        FromInRadius = X2;
        ToInRadius = X1;
    }
    else
    {
        FromInRadius = X1;
        ToInRadius = X2;
    }

    float From = FMath::Max(FromInRadius, FromWallCoordinate);
    float To = FMath::Min(ToInRadius, ToWallCoordinate);

    return FMath::FRandRange(From, To);
}

TArray<FVector> ASphereTargetHubActor::GenerateCoordinatesPool(float SpawnRadius, int IterationCount)
{
    TArray<FVector> Pool = {};

    // Radius of sphere
    float Radius = 60.0f;

    // Coorinates for avoiding collision with walls
    const float X_FROM_WALL = -2180.0f + Radius;
    const float X_TO_WALL = 1620.0f - Radius;

    const float Y_FROM_WALL = -1900.0f + Radius;
    const float Y_TO_WALL = 1900.0f - Radius;

    const float Z_FROM_WALL = 170.0f + Radius;

    // Randomize Axis
    const int32 MAX_COORDINATE_INDEX = 2;

    while (Pool.Num() < IterationCount)
    {
        float XCoordinate = 0.0f;
        float YCoordinate = 0.0f;
        float ZCoordinate = 0.0f;

        while (!(XCoordinate && YCoordinate && ZCoordinate))
        {
            const int32 a = FMath::RandRange(0, MAX_COORDINATE_INDEX);

            // Randomize Coordinate
            if (a == 0 && !XCoordinate)
            {
                XCoordinate = GenerateRandomCoordinate(
                    YCoordinate, ZCoordinate, ActorLocation.Y, ActorLocation.Z, ActorLocation.X, X_FROM_WALL, X_TO_WALL, SpawnRadius);
            }
            else if (a == 1 && !YCoordinate)
            {
                YCoordinate = GenerateRandomCoordinate(
                    XCoordinate, ZCoordinate, ActorLocation.X, ActorLocation.Z, ActorLocation.Y, Y_FROM_WALL, Y_TO_WALL, SpawnRadius);
            }
            else if (a == 2 && !ZCoordinate)
            {
                ZCoordinate = GenerateRandomCoordinate(
                    XCoordinate, YCoordinate, ActorLocation.X, ActorLocation.Y, ActorLocation.Z, Z_FROM_WALL, SpawnRadius, SpawnRadius);
            }
        }

        const FVector GenerateVector = FVector(XCoordinate, YCoordinate, ZCoordinate);

        // Check distance between spheres
        bool DistanceBetweenEnough = true;

        for (int32 i = 0; DistanceBetweenEnough && i < Pool.Num(); i++)
        {
            if (FVector::Dist(GenerateVector, Pool[i]) < 80)
            {
                DistanceBetweenEnough = false;
            }
        }

        if (DistanceBetweenEnough)
        {
            Pool.Add(GenerateVector);
        }
    }

    return Pool;
}

TArray<ASphereTargetActor*> ASphereTargetHubActor::GenerateSphereTargetActors(TArray<FVector> Pool, UWorld* World)
{
    TArray<ASphereTargetActor*> SphereTargetActorsTemp = {};

    for (int32 i = 0; i < Pool.Num(); ++i)
    {
        const FTransform SphereTargetTransform = FTransform(FRotator::ZeroRotator, Pool[i]);
        ASphereTargetActor* ASphereTarget = World->SpawnActor<ASphereTargetActor>(SphereTarget, SphereTargetTransform);

        SphereTargetActorsTemp.Add(ASphereTarget);
    }

    return SphereTargetActorsTemp;
}

void ASphereTargetHubActor::DestroySphereTargetActors()
{
    for (int i = 0; i < SphereTargetActors.Num(); i++)
    {
        SphereTargetActors[i]->Destroy();
    }
}

void ASphereTargetHubActor::SpawnSphereTargetActors()
{
    UWorld* World = GetWorld();
    if (World)
    {
        DestroySphereTargetActors();

        AActor* FoundActor = UGameplayStatics::GetActorOfClass(World, APingleFirstPersonCharacter::StaticClass());
        ActorLocation = FoundActor->GetActorLocation();

        const float SPAWN_SMALL_RADIUS = 1500.0f;
        const int SMALL_ITERATION_COUNT = 10;

        // Generate coordinates FVector pool for spawn
        TArray<FVector> Pool = GenerateCoordinatesPool(SpawnBigRadius, AllIterationCount - SMALL_ITERATION_COUNT);
        TArray<FVector> PoolRecuired = GenerateCoordinatesPool(SPAWN_SMALL_RADIUS, AllIterationCount);

        Pool.Append(PoolRecuired);

        // Generate spheres
        SphereTargetActors = GenerateSphereTargetActors(Pool, World);

        SpawnBigRadius += SpawnBigRadius / 20;
        AllIterationCount += AllIterationCount / 10;
    }
}
