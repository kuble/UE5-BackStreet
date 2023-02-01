#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

#define InventoryMaxSize 6

UCLASS()
class BACKSTREET_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

//----- Global / Component ----------
public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UChildActorComponent* InventoryComponent; 

// ------- Character Action 기본 ------- 
public:
	//Input에 Binding 되어 공격을 시도 (AnimMontage를 호출)
	virtual void TryAttack();

	//AnimNotify에 Binding 되어 실제 공격을 수행
	virtual void Attack();

	virtual void StopAttack();

	virtual void TryReload();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
		, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die();

	//플레이어가 현재 해당 Action을 수행하고 있는지 반환
	UFUNCTION(BlueprintCallable)
		bool GetIsActionActive(ECharacterActionType Type) { return CharacterState.CharacterActionState == Type; }

	//플레이어의 ActionState를 Idle로 전환한다.
	UFUNCTION(BlueprintCallable)
		void ResetActionState();

	//디버프 데미지를 입힘 (일회성)
	UFUNCTION()
		float TakeDebuffDamage(float DamageAmount, uint8 DebuffType, AActor* Causer);

	//플레이어가 체력을 회복함 (일회성)
	UFUNCTION()
		void TakeHeal(float HealAmount, bool bIsTimerEvent = false, uint8 BuffDebuffType = 0);

// ------- Character Stat/State ------------------------------
public:
	//캐릭터의 상태 정보를 초기화
	UFUNCTION()
		void InitCharacterState();

	//캐릭터의 스탯을 업데이트
	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FCharacterStatStruct NewStat);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FCharacterStatStruct GetCharacterStat() { return CharacterStat; }

// ------ 무기 관련 -------------------------------------------
public:
	UFUNCTION()
		bool EquipWeapon(class AWeaponBase* TargetWeapon);

	//무기를 집는다. 인벤토리가 꽉 찼다면 false를 반환
	UFUNCTION(BlueprintCallable)
		bool PickWeapon(int32 NewWeaponID);

	//다음 무기로 전환한다. 전환에 실패하면 false를 반환
	virtual void SwitchToNextWeapon();

	//무기를 Drop한다. (월드에서 아예 사라진다.)
	virtual void DropWeapon();

	UFUNCTION(BlueprintCallable)
		class AWeaponInventoryBase* GetInventoryRef();

	//무기 Ref를 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		class AWeaponBase* GetWeaponActorRef();

	//공격Action 사이의 Interval을 관리하는 타이머를 해제
	UFUNCTION()
		void ResetAtkIntervalTimer();

// ------ 캐릭터 버프 / 디버프 ------------------------------------
public:
	//버프와 디버프를 건다
	virtual	bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);
	
	//버프 or 디버프 상태를 초기화한다
	virtual void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal);

	//특정 Buff/Debuff의 타이머를 해제한다.
	virtual void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType);
		
	//모든 Buff/Debuff의 타이머를 해제
	virtual void ClearAllBuffDebuffTimer(bool bIsDebuff);

	UFUNCTION()
		bool SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION()
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//디버프가 활성화 되어있는지 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//버프가 활성화 되어있는지 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetBuffIsActive(ECharacterBuffType BuffType);

	//버프/디버프 남은 시간을 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType);

	//버프 / 디버프 타이머 핸들의 참조자를 반환
	UFUNCTION()
		FTimerHandle& GetBuffDebuffTimerHandleRef(bool bIsDebuff, uint8 BuffDebuffType);

// ----- 캐릭터 애니메이션 -------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		TArray<UAnimMontage*> AttackAnimMontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* RollAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* ReloadAnimMontage;

// ------ 그 외 캐릭터 프로퍼티  ---------------
protected:
	//캐릭터의 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStatStruct CharacterStat;

	//캐릭터의 현재 상태
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
		FCharacterStateStruct CharacterState;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		class AWeaponInventoryBase* InventoryRef;

// ----- 타이머 관련 ---------------------------------
protected:
	virtual void ClearAllTimerHandle();

private:
	//공격 간 딜레이 핸들
	UPROPERTY()
		FTimerHandle AtkIntervalHandle;

	UPROPERTY()
		FTimerHandle ReloadTimerHandle;

	UPROPERTY()
		TArray<FTimerHandle> BuffDebuffTimerHandleList;

};