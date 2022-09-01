// Fill out your copyright notice in the Description page of Project Settings.

#include "SphereTargetActor.h"
#include "Components/SphereComponent.h"
#include "SphereTargetHubActor.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASphereTargetActor::ASphereTargetActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
    SetRootComponent(BaseMesh);

    BaseMesh->OnComponentHit.AddDynamic(this, &ASphereTargetActor::OnHit);
}

// Called when the game starts or when spawned
void ASphereTargetActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASphereTargetActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASphereTargetActor::OnHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ASphereTargetHubActor* FoundSphereTargetHubActor =
        Cast<ASphereTargetHubActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASphereTargetHubActor::StaticClass()));

    FVector ActorInitLocation = FoundSphereTargetHubActor->GetActorInitialLocation();
    if (FVector::Distance(ActorInitLocation, HitComponent->GetRelativeLocation()) < 1500)
    {
        FoundSphereTargetHubActor->IncreaseByOneHitInRadiusCounter();
    }

    int HitInRadiusCounter = FoundSphereTargetHubActor->GetHitInRadiusCounter();
    if (HitInRadiusCounter >= 10)
    {
        FoundSphereTargetHubActor->SetHitInRadiusCounter(0);
        FoundSphereTargetHubActor->SpawnSphereTargetActors();
    }

    HitComponent->DestroyComponent();
}
