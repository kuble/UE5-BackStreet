﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/GateBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../public/ChapterManagerBase.h"
#include "../public/TransitionManager.h"
#include "../public/StageData.h"
#include "Math/Color.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGateBase::AGateBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	OverlapVolume->SetupAttachment(RootComponent);
	Mesh->SetupAttachment(RootComponent);

}

// Called every frame
void AGateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AGateBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGateBase::InitGate()
{
	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());
	CheckHaveToActive();
	if(!MoveStageDelegate.IsBound())
		MoveStageDelegate.BindUFunction(GamemodeRef->GetChapterManagerRef()->GetTransitionManager(), FName("MoveStage"));
	AddGate();

}

void AGateBase::AddGate()
{
	AStageData* stage = GamemodeRef->GetChapterManagerRef()->GetCurrentStage();
	stage->GateList.Add(this);
}

void AGateBase::EnterGate()
{
	if (this->ActorHasTag(FName("StartGate")))
	{
		InitGate();
	}
	RequestMoveStage();
}

void AGateBase::ActivateChapterGate()
{
	UE_LOG(LogTemp, Log, TEXT("AGateBase:ActivateChapterGate"));
	Mesh->SetMaterial(0, GateMaterialList[1]);

}

void AGateBase::ActivateNormalGate()
{
	UE_LOG(LogTemp, Log, TEXT("AGateBase:ActivateNormalGate"));
	Mesh->SetMaterial(0, GateMaterialList[0]);

}

void AGateBase::DeactivateGate()
{
	UE_LOG(LogTemp, Log, TEXT("AGateBase:DeactivateGate"));
	Mesh->SetMaterial(0, GateMaterialList[2]);

}

void AGateBase::RequestMoveStage()
{
	if (this->ActorHasTag(FName("StartGate")))
	{
		GamemodeRef->FadeOutDelegate.Broadcast();

		GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
				MoveStageDelegate.Execute(EDirection::E_Start);
				GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			}), 1.0f, false, 1.0f);
		
	}
	else if (this->ActorHasTag(FName("ChapterGate")))
	{
		if (GamemodeRef->GetChapterManagerRef()->IsChapterClear())
		{
			GamemodeRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
				MoveStageDelegate.Execute(EDirection::E_Chapter);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
		}else
		{
			GamemodeRef->PrintSystemMessageDelegate.Broadcast(FName(TEXT("미션을 클리어해주세요.")), FColor::White);
		}
	}
	else
	{
		 if (this->ActorHasTag(FName("UP")))
		{
			 GamemodeRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
				MoveStageDelegate.Execute(EDirection::E_UP);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Up GateInfo"));
		}
		else if (this->ActorHasTag(FName("DOWN")))
		{
			 GamemodeRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
				MoveStageDelegate.Execute(EDirection::E_DOWN);
				GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Down GateInfo"));
		}
		else if (this->ActorHasTag(FName("RIGHT")))
		{
			 GamemodeRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
				MoveStageDelegate.Execute(EDirection::E_RIGHT);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Right GateInfo"));
		}
		else if (this->ActorHasTag(FName("LEFT")))
		{
			 GamemodeRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
				MoveStageDelegate.Execute(EDirection::E_LEFT);
				GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Left GateInfo"));
		}

	}
}

void AGateBase::CheckHaveToActive()
{
	ABackStreetGameModeBase* gameModeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());
	AStageData* stage = gameModeRef->GetChapterManagerRef()->GetCurrentStage();

	if (stage != nullptr)
	{
		if (this->Tags[0].IsEqual(FName(TEXT("StartGate"))))
		{
			return;
		}
		else if (this->Tags[0].IsEqual(FName(TEXT("ChapterGate"))))
		{
			if (stage->GetStageType() != EStageCategoryInfo::E_Boss)
			{
				Destroy();
			}
			if (gameModeRef->GetChapterManagerRef()->IsChapterClear())
			{
				ActivateChapterGate();
			}
			else
			{
				DeactivateGate();
			}
		
		}
		else
		{
			
			for (int i = 0; i < 4; i++)
			{
				if (!stage->GateInfo[i])
				{
					switch (i)
					{
					case 0:
						if (this->Tags[1].IsEqual(FName(TEXT("UP"))))
							Destroy();
						break;
					case 1:
						if (this->Tags[1].IsEqual(FName(TEXT("DOWN"))))
							Destroy();
						break;
					case 2:
						if (this->Tags[1].IsEqual(FName(TEXT("LEFT"))))
							Destroy();
						break;
					case 3:
						if (this->Tags[1].IsEqual(FName(TEXT("RIGHT"))))
							Destroy();
						break;
					default:
						break;
					}
		
				}
			}

			ActivateNormalGate();
		}
	}

}
