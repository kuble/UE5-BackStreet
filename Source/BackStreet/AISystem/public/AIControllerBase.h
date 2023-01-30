// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

//-- 기본 함수 및 컴포넌트 ---------------
public:
	AAIControllerBase();

	UFUNCTION()
		virtual void OnPossess(APawn* PossessedPawn) override;

	UFUNCTION()
		virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|AI")
		class UAIPerceptionComponent* AIPerceptionComponent;

//--Perception 관련 ---------------------
protected:
	//시야 범위
	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
		float SightRadius;

	//최대 시야 수명
	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
		float MaxSightAge;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
		class UAISenseConfig_Sight* SightPerceptionConfig;

public:
	UFUNCTION()
		void InitAIPerceptionSystem();

	UFUNCTION()
		void UpdateTargetPerception(AActor* Actor, FAIStimulus Stimulus);

//--자원 관리 ---------------------
public:
	//게임 종료 이벤트에 바인딩해서 사용
	UFUNCTION()
		void ClearAllTimerHandle();

//--기타 Property-----------------------
protected:
	//실행할 BT를 지정
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;

	//AI가 현재 내리고 있는 BT 액션 상태를 나타앰
	UPROPERTY(BlueprintReadWrite)
		EAIBehaviorType AIBehaviorState;

private:
	//캐릭터 감지 Time-Out 시, 기억을 지우는 로직을 수행
	//해당 함수에 바인딩하여 사용할 타이머 핸들
	UPROPERTY()
		FTimerHandle SightLossTimerHandle;
};
