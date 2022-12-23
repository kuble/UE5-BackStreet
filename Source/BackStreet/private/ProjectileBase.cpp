// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ProjectileBase.h"
#include "../public/CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SCENE_ROOT"));
	//DefaultSceneRoot->SetupAttachment(RootComponent);
	//SetRootComponent(DefaultSceneRoot);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE_COLLISION"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	SetRootComponent(SphereCollision);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PROJECTILE_MESH"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetRelativeScale3D({ 0.5f, 0.5f, 0.5f });
	 
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE_MOVEMENT"));
	ProjectileMovement->InitialSpeed = ProjectileStat.ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileStat.ProjectileSpeed;
	ProjectileMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnProjectileBeginOverlap);
}

void AProjectileBase::InitProjectile(FProjectileStatStruct NewStat, ACharacterBase* NewCharacterRef)
{
	if (IsValid(NewCharacterRef))
	{
		OwnerCharacterRef = NewCharacterRef;
		ProjectileStat = NewStat;
	}
}

void AProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == OwnerCharacterRef
		|| !OtherActor->ActorHasTag("Character")) return;
	
	FTransform TargetTransform = { FRotator(), SweepResult.Location, {1.0f, 1.0f, 1.0f} };
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, TargetTransform);
	UGameplayStatics::ApplyDamage(OtherActor, ProjectileStat.ProjectileDamage,
								  SpawnInstigator, this, nullptr);
	Destroy();
}

void AProjectileBase::ActivateProjectileMovement()
{
	ProjectileMovement->Activate();
}

void AProjectileBase::SetSpawnInstigator(AController* NewInstigator)
{
	if (IsValid(NewInstigator))
	{
		SpawnInstigator = NewInstigator;
	}
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

