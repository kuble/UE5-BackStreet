// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"
#define MAX_AMMO_LIMIT_CNT 2000

UCLASS()
class BACKSTREET_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		uint8 WeaponID;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void InitWeapon(class ACharacterBase* NewOwnerCharacterRef);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//------ Global -------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		uint8 WeaponID;

	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		UParticleSystem* HitEffectParticle;

	//Weapon의 종합 Stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FWeaponStatStruct WeaponStat;

	//공격 처리
	UFUNCTION(BlueprintCallable)
		void Attack();

	//공격 마무리 처리
	UFUNCTION(BlueprintCallable)
		void StopAttack();

	//Weapon Stat 초기화
	UFUNCTION(BlueprintCallable)
		void UpdateWeaponStat(FWeaponStatStruct NewStat);

	//공격 범위를 반환
	UFUNCTION(BlueprintCallable)
		float GetAttackRange();

	//------ Projectile 관련-------------
public:
	//발사체를 생성
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	//발사체 초기화 및 발사를 시도 
	UFUNCTION()
		bool TryFireProjectile();

	//장전을 시도. 현재 상태에 따른 성공 여부를 반환
	UFUNCTION(BlueprintCallable)
		bool TryReload();

	//남은 탄환의 개수를 반환 - Stat.TotalAmmoCount
	UFUNCTION(BlueprintCallable)
		int32 GetLeftAmmoCount() { return TotalAmmoCount; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetCanReload();

	//탄환의 개수를 더함 (TotalAmmoCount까지)
	UFUNCTION(BlueprintCallable)
		void AddAmmo(int32 Count);

	//탄창의 개수를 더함 (+= MaxAmmoPerMagazine*Count)
	UFUNCTION()
		void AddMagazine(int32 Count);

	UFUNCTION()
		void SetInfiniteAmmoMode(bool NewMode) { WeaponStat.bIsInfiniteAmmo = NewMode; }

protected:
	//SoftObjRef로 대체 예정
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

	//현재 탄창에 있는 발사체 수
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentAmmoCount = 1;

	//가지고 있는 최대 발사체 수
	UPROPERTY(BlueprintReadOnly)
		int32 TotalAmmoCount = 0;

//-------- Melee 관련 ------------
public:
	//현재 Combo 수를 반환 
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentComboCnt() { return ComboCnt; }

	//근접 공격을 수행
	UFUNCTION()
		void MeleeAttack();

	//Melee Combo 초기화
	UFUNCTION()
		void ResetCombo();

	UFUNCTION()
		TArray<FVector> GetCurrentMeleePointList();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Debug")
		float SphereTraceRadius = 5.0f;

protected:
	//현재 MeleeCombo 수
	UPROPERTY(BlueprintReadOnly)
		int32 ComboCnt = 0;

private:
	//캐릭터 Ref
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		FTimerHandle MeleeAtkTimerHandle;

	UPROPERTY()
		FTimerHandle AutoReloadTimerHandle;

	UPROPERTY()
		FTimerHandle MeleeComboTimerHandle;

	UPROPERTY()
		float MeleeAtkComboRemainTime = 1.0f;

	UPROPERTY()
		TArray<FVector> MeleePrevTracePointList;

	//UPROPERTY()
	FCollisionQueryParams MeleeLineTraceQueryParams;
};