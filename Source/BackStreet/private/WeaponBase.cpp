// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "CharacterBase.h"
#include "CollisionQueryParams.h"
#include "../public/ProjectileBase.h"

FTimerHandle MeleeAtkDelayHandle;
FCollisionQueryParams linetraceCollisionQueryParams;

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON_MESH"));
	WeaponMesh->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Attack()
{
	//�߻�ü�� �ִ� ������ �߻�
	if (WeaponStat.bHasProjectile) 
	{
		TryFireProjectile();
	}
	//���� ������ ������ ������ ���� ���� ���� ����
	if (WeaponStat.bCanMeleeAtk)
	{
		GetWorldTimerManager().SetTimer(MeleeAtkTimerHandle, this, &AWeaponBase::MeleeAttack, 0.01f, true);
		GetWorldTimerManager().SetTimer(MeleeComboTimerHandle, this, &AWeaponBase::ResetMeleeCombo, 1.0f, false, 1.0f);
		MeleeAttack();
		MeleeComboCnt = (MeleeComboCnt + 1);
	}
}

void AWeaponBase::StopAttack()
{
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
}

void AWeaponBase::InitWeaponStat(FWeaponStatStruct NewStat)
{
	WeaponStat = NewStat;
}

AProjectileBase* AWeaponBase::CreateProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Projectile�� �����ڴ� Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator SpawnRotation = OwnerCharacterRef->GetActorRotation();
	FVector SpawnLocation = OwnerCharacterRef->GetActorLocation();
	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetActorForwardVector() * 100.0f;
	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetActorRightVector() * 25.0f;
	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };

	AProjectileBase* newProjectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));
	if(IsValid(newProjectile)) newProjectile->SetSpawnInstigator(OwnerCharacterRef->GetController());
	return newProjectile;
}

bool AWeaponBase::TryReload()
{
	if (!GetCanReload()) return false;

	int32 addAmmoCnt = FMath::Min(MaxAmmoCount, WeaponStat.MaxAmmoPerMagazine);
	if (addAmmoCnt + CurrentAmmoCount > WeaponStat.MaxAmmoPerMagazine)
		addAmmoCnt = (WeaponStat.MaxAmmoPerMagazine - CurrentAmmoCount);

	CurrentAmmoCount += addAmmoCnt; 
	MaxAmmoCount -= addAmmoCnt;

	return true;
}

bool AWeaponBase::GetCanReload()
{
	if (bInfiniteAmmo || !WeaponStat.bHasProjectile) return false;
	if (MaxAmmoCount == 0 || CurrentAmmoCount == WeaponStat.MaxAmmoPerMagazine) return false;
	return true;
}

void AWeaponBase::AddAmmo(int32 Count)
{
	if (bInfiniteAmmo || MaxAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	MaxAmmoCount = (MaxAmmoCount + Count) % MAX_AMMO_LIMIT_CNT;
}

void AWeaponBase::AddMagazine(int32 Count)
{
	if (bInfiniteAmmo || MaxAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	MaxAmmoCount = (MaxAmmoCount + WeaponStat.MaxAmmoPerMagazine * Count) % MAX_AMMO_LIMIT_CNT;
}

bool AWeaponBase::TryFireProjectile()
{
	if (CurrentAmmoCount == 0)
	{
		TryReload();
		return false;
	}

	AProjectileBase* newProjectile = CreateProjectile();

	//������ �߻�ü�� Valid �ϴٸ� �߻�
	if (IsValid(newProjectile))
	{
		if(!bInfiniteAmmo) CurrentAmmoCount -= 1;
		newProjectile->ActivateProjectileMovement();
		return true;
	}
	return false;
}

void AWeaponBase::MeleeAttack()
{	
	FHitResult hitResult;
	FVector StartLocation = WeaponMesh->GetSocketLocation(FName("GrabPoint"));
	FVector EndLocation = WeaponMesh->GetSocketLocation(FName("End"));

	//LineTrace�� ���� hit �� ��ü���� ����
	GetWorld()->LineTraceSingleByChannel(hitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, linetraceCollisionQueryParams);
	
	//hit �Ǿ��ٸ�?
	if (hitResult.bBlockingHit && IsValid(OwnerCharacterRef))
	{
		//�������� �ְ�
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponDamage, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);

		//ȿ�� �̹��� ���
		if (IsValid(HitEffectParticle))
		{
			FTransform emitterSpawnTransform(FQuat(0.0f), FVector(1.0f), hitResult.Location);
			linetraceCollisionQueryParams.AddIgnoredActor(hitResult.GetActor());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);
		}
	}
}

void AWeaponBase::ResetMeleeCombo()
{
	MeleeComboCnt = 0;
	linetraceCollisionQueryParams.ClearIgnoredActors();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::InitOwnerCharacterRef(ACharacterBase* NewCharacterRef)
{
	if (!IsValid(NewCharacterRef)) return;
	OwnerCharacterRef = NewCharacterRef;
}
