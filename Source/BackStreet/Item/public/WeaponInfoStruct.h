#pragma once

#include "Engine/DataTable.h"
#include "../../Character/public/CharacteInfoEnum.h"
#include "ProjectileInfoStruct.h"
#include "WeaponInfoStruct.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Hit				UMETA(DisplayName = "Hit"),
	E_Attack			UMETA(DisplayName = "Attack"),
	E_Explosion			UMETA(DisplayName = "Explosion")
};

USTRUCT(BlueprintType)
struct FWeaponStatStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FName WeaponName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FName Description;
	

	//----- 공통 Stat -------
	
	//공격 속도 Rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate = 1.0f;

	//무기 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamageRate = 1.0f;

	//----- 근접 관련 Property --------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponMeleeDamage = 0.2f;

	//근접 공격이 가능한 지? 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bCanMeleeAtk = true;

	//무기는 각 하나의 디버프만 가짐 (임시)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;


	//----- 발사체 관련 Property ------------------
	//무한 탄약인지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsInfiniteAmmo;

	//공격 시, 발사체가 있는지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bHasProjectile;

	// 무한 탄창인지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteMagazine;

	//한 탄창에 최대 발사체 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxAmmoPerMagazine; 

	//장전 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float LoadingDelayTime;
};

USTRUCT(BlueprintType)
struct FWeaponItemInfoStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bCanMeleeAtk = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamage = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bHasProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxAmmoPerMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float LoadingDelayTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileDamage = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float GravityScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsHoming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType ProjectDebuffType;

};