// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Global/public/BackStreet.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StageGenerator.generated.h"
// 이런 define 코드 BackStreet,h에 모아서 정의하는게 깔끔하지 않을까 싶음

#define ChapterSize 3

/**
 * 
 */
UCLASS()
class BACKSTREET_API UStageGenerator : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
		TArray<class AStageData*> CreateMaze();

private:
	UFUNCTION()
		void InitStageData(FVector Vector, int32 Ypos, int32 Xpos, class AStageData* Target);

	UFUNCTION()
		void RecursiveBacktracking();

	UFUNCTION()		
		class AStageData* GetRandomNeighbourTile(class AStageData* Tile);

	UFUNCTION()
		void VisitTile(class AStageData* CurrentTilePara, class AStageData* NextTilePara);

	UFUNCTION()
		class AStageData* GetTile(int32 XPosition, int32 YPosition);

private:
	UPROPERTY()
		TArray< class AStageData* > Tracks;
	UPROPERTY()
		TArray<class AStageData*> Stages;

};
