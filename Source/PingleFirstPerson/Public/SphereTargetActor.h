// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "SphereTargetActor.generated.h"

UCLASS()
class PINGLEFIRSTPERSON_API ASphereTargetActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASphereTargetActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* BaseMesh;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};
