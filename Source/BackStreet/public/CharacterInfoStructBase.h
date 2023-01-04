#pragma once

#include "Engine/DataTable.h"
#include "CharacterInfoStructBase.generated.h"

UENUM(BlueprintType)
enum class ECharacterDebuffType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Flame				UMETA(DisplayName = "Flame"),
	E_Poison			UMETA(DisplayName = "Poison"),
	E_Slow				UMETA(DisplayName = "Slow"),
	E_Sleep				UMETA(DisplayName = "Sleep"),
	E_AttackDown		UMETA(DisplayName = "AttackDown"),
	E_DefenseDown		UMETA(DisplayName = "DefenseDown"),
	E_SlowAtk			UMETA(DisplayName = "SlowAtk"),
	E_SlowProjectile	UMETA(DisplayName = "SlowProjectile")
};

UENUM(BlueprintType)
enum class ECharacterBuffType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Healing			UMETA(DisplayName = "Healing"),
	E_AttackUp			UMETA(DisplayName = "AttackUp"),
	E_DefenseUp			UMETA(DisplayName = "DefenseUp"),
	E_SpeedUp			UMETA(DisplayName = "SpeedUp"),
	E_FastAtk			UMETA(DisplayName = "FastAtk"),
	E_FastProjectile	UMETA(DisplayName = "FastProjectile"),
	E_Invincibility		UMETA(DisplayName = "Invincibility"),
	E_InfiniteAmmo		UMETA(DisplayName = "InfiniteAmmo")
};

UENUM(BlueprintType)
enum class EAIBehaviorType : uint8
{
	E_Idle			UMETA(DisplayName = "Idle"),
	E_Patrol		UMETA(DisplayName = "Patrol"),
	E_Detect		UMETA(DisplayName = "Detect"),
	E_Chase			UMETA(DisplayName = "Chase"),
	E_Attack		UMETA(DisplayName = "Attack"),
	E_Return		UMETA(DisplayName = "Return"),
	E_Sleep			UMETA(DisplayName = "Sleep")
};

UENUM(BlueprintType)
enum class ECharacterActionType : uint8
{
	E_Idle			UMETA(DisplayName = "Idle"),
	E_Attack		UMETA(DisplayName = "Attack"),
	E_Defense		UMETA(DisplayName = "Defense"),
	E_Roll			UMETA(DisplayName = "Roll"),
	E_Jump			UMETA(DisplayName = "Jump"),
	E_Reload		UMETA(DisplayName = "Reload"),
	E_Sleep			UMETA(DisplayName = "Sleep"),
	E_Die			UMETA(DisplayName = "Die")
};

USTRUCT(BlueprintType)
struct FCharacterStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsInvincibility = false;

	UPROPERTY(BlueprintReadOnly)
		bool bInfiniteAmmo = false;

	//PlayerMaxHP는 1.0f
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0.5f, UIMax = 10.0f))
		float CharacterMaxHP = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0.1f, UIMax = 10.0f))
		float CharacterAtkMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0.2f, UIMax = 1.0f))
		float CharacterAtkSpeed = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 100.0f, UIMax = 1000.0f))
		float CharacterMoveSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = -1.0f, UIMax = 1.0f))
		float CharacterDefense = 0.0f;
};

USTRUCT(BlueprintType)
struct FCharacterStateStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//캐릭터의 디버프 상태 (Bit-Field로 표현)
	UPROPERTY(BlueprintReadOnly)
		int32 CharacterDebuffState = (1<<10);

	//캐릭터의 버프 상태
	UPROPERTY(BlueprintReadOnly)
		int32 CharacterBuffState = (1 << 10);

	//공격을 할 수 있는 상태인지?
	UPROPERTY(BlueprintReadOnly)
		bool bCanAttack = false;

	//캐릭터의 행동 정보
	UPROPERTY(BlueprintReadWrite)
		ECharacterActionType CharacterActionState;

	//PlayerMaxHP는 1.0f
	UPROPERTY(BlueprintReadOnly)
		float CharacterCurrHP;
};

