// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "CharacterBase.h"
#include "GameFramework/Character.h"
#include "MainCharacterBase.generated.h"

UCLASS()
class BACKSTREET_API AMainCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ------- 컴포넌트 ----------
public:
	//플레이어 메인 카메라 붐
	UPROPERTY(VisibleDefaultsOnly)
		USpringArmComponent* CameraBoom;

	//플레이어의 메인 카메라
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* FollowingCamera;

// ------- Character Action ------- 
public:
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void Dash();

	UFUNCTION(BlueprintImplementableEvent)
		void Roll();

	UFUNCTION()
		virtual void TryReload() override;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
		virtual void TryAttack() override;

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
		virtual void StopAttack() override;

	UFUNCTION()
		void RotateToCursor();

// -------
public: 
	//버프 or 디버프 상태를 지정
	UFUNCTION(BlueprintCallable)
		virtual	bool SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f) override;

	//버프 or 디버프 상태를 초기화한다
	UFUNCTION(BlueprintCallable)
		virtual void ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal) override;

	//특정 Debuff의 타이머를 해제한다.
	UFUNCTION(BlueprintCallable)
		virtual void ClearBuffTimer(bool bIsDebuff, uint8 BuffType) override;

	//모든 Buff/Debuff의 타이머를 해제
	UFUNCTION(BlueprintCallable)
		virtual void ClearAllBuffTimer(bool bIsDebuff) override;

// -------- VFX -----------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		class UNiagaraComponent* BuffNiagaraEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		TArray<class UNiagaraSystem*> BuffNiagaraEffectList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		TArray<class UNiagaraSystem*> DebuffNiagaraEffectList;

	UFUNCTION()
		void DeactivateBuffNiagara();

// ------- 그 외 -----------
public:
	//UFUNCTION()
	virtual void ClearAllTimerHandle() override;

private:
	UPROPERTY()
		class AMainCharacterController* PlayerControllerRef;

	//공격 시, 마우스 커서의 위치로 캐릭터가 바라보는 로직을 초기화하는 타이머
	//초기화 시에는 다시 Movement 방향으로 캐릭터의 Rotation을 Set
	UPROPERTY()
		FTimerHandle RotationFixTimerHandle;

	//공격 반복 작업 타이머
	UPROPERTY()
		FTimerHandle AttackLoopTimerHandle;

	//구르기 딜레이 타이머
	UPROPERTY()
		FTimerHandle RollTimerHandle;
};
