// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../public/BackStreet.h"
#include "GameFramework/GameModeBase.h"
#include "../public/DirectionEnumInfo.h"
#include "BackStreetGameModeBase.generated.h"


UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AGrid* Chapter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* CurrTile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RemainChapter; // 남은 챕터 수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 PreDir = (uint8)(EDirection::E_DOWN); // 다음 스테이지 기준으로 입구 게이트

public:
	ABackStreetGameModeBase();
	UFUNCTION(BlueprintCallable)
		void InitGame();
	UFUNCTION(BlueprintCallable)
		void InitChapter();
	UFUNCTION(BlueprintCallable)
		void NextStage(uint8 Dir);
	UFUNCTION(BlueprintCallable)
		void MoveTile(uint8 NextDir);

};
