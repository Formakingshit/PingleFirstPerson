// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereTargetActor.h"
#include "../PingleFirstPersonCharacter.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "SphereTargetHubActor.generated.h"

UCLASS()
class PINGLEFIRSTPERSON_API ASphereTargetHubActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASphereTargetHubActor();

    void SetActorInitialLocation(FVector ActorInitialLocation) { ActorLocation = ActorInitialLocation; }
    FVector GetActorInitialLocation() { return ActorLocation; }

    void SetHitInRadiusCounter(int NewHitInRadiusCounter) { HitInRadiusCounter = NewHitInRadiusCounter; }
    int GetHitInRadiusCounter() { return HitInRadiusCounter; }
    void IncreaseByOneHitInRadiusCounter() { HitInRadiusCounter++; }

    void SpawnSphereTargetActors();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ASphereTargetActor> SphereTarget;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    float GenerateRandomCoordinate(float FirstCoordinateVector, float SecondCoordinateVector, float ActorLocationFirstCoordinate,
        float ActorLocationSecondCoordinate, float ActorLocationCoordinate, float FromWallCoordinate, float ToWallCoordinate,
        float RadiusLimit);

    TArray<FVector> GenerateCoordinatesPool(float SpawnRadius, int IterationCount);
    TArray<ASphereTargetActor*> GenerateSphereTargetActors(TArray<FVector> Pool, UWorld* World);
    void DestroySphereTargetActors();

    TArray<ASphereTargetActor*> SphereTargetActors;
    FVector ActorLocation;
    int HitInRadiusCounter = 0;

    float SpawnBigRadius = 2000.0f;
    int AllIterationCount = 15;
};
