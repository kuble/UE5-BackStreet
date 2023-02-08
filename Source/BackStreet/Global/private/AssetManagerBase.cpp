
#include "../public/AssetManagerBase.h"
#include "../../StageSystem/public/TileBase.h"
#include "../../Item/public/ItemBase.h"
#include "../../Item/public/ProjectileBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/MissionBase.h"
#include "UObject/ConstructorHelpers.h"
#include "../../Character/public/EnemyCharacterBase.h"

AAssetManagerBase::AAssetManagerBase()
{
static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Map/Stages/Data/D_StageEnemyTypeTable.D_StageEnemyTypeTable"));
	if (DataTable.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		StageTypeTable = DataTable.Object;
	}

}

void AAssetManagerBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Being Play"));
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
}

void AAssetManagerBase::LoadItemAsset(EItemCategoryInfo Type, AActor* SpawnPoints)
{
	UE_LOG(LogTemp, Log, TEXT("Call LoadItemAsset"));

	switch (Type)
	{
	case EItemCategoryInfo::E_None:
		break;
	case EItemCategoryInfo::E_Weapon:
		if (WeaponItemAssets.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[LoadItemAsset] WeaponAsstes num %d"), WeaponItemAssets.Num());
			WeaponStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(WeaponItemAssets, FStreamableDelegate::CreateUObject(this, &AAssetManagerBase::SpawnWeaponItem, Type, SpawnPoints));
		}
		break;
	case EItemCategoryInfo::E_Bullet:
		if (ProjectileItemAssets.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[LoadItemAsset] ProjectileAsstes num %d"), ProjectileItemAssets.Num());
			WeaponStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(ProjectileItemAssets, FStreamableDelegate::CreateUObject(this, &AAssetManagerBase::SpawnProjectileItem, Type, SpawnPoints));
		}
		break;
	case EItemCategoryInfo::E_Buff:
		if (BuffAssets.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[LoadItemAsset] BuffAsstes num %d"), BuffAssets.Num());
			AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(BuffAssets, FStreamableDelegate::CreateUObject(this, &AAssetManagerBase::SpawnBuffItem,Type,SpawnPoints));
		}
		break;
	case EItemCategoryInfo::E_StatUp:
		break;
	case EItemCategoryInfo::E_Mission:
		if (MissionAssets.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[LoadItemAsset] MissionAsstes num %d"), MissionAssets.Num());
			AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(MissionAssets, FStreamableDelegate::CreateUObject(this, &AAssetManagerBase::SpawnMissionItem, Type, SpawnPoints));
		}
		break;
	default:
		break;
	}
	
}

void AAssetManagerBase::SpawnBuffItem(EItemCategoryInfo Type, AActor* SpawnPoints)
{
		int32 BuffType = FMath::RandRange(0, 5);
		if (BuffAssets[BuffType].IsValid())
		{
			UBlueprint* Gen = Cast<UBlueprint>(BuffAssets[BuffType].ResolveObject());
			AItemBase* Target = GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, SpawnPoints->GetActorLocation(), FRotator::ZeroRotator);
			// AItemBase 클래스 스폰하고 Init 하기
			Target->InitItem(Type);

		}
}

void AAssetManagerBase::SpawnWeaponItem(EItemCategoryInfo Type, AActor* SpawnPoints)
{
	if (WeaponItemAssets[0].IsValid())
	{
		UBlueprint* Gen = Cast<UBlueprint>(WeaponItemAssets[0].ResolveObject());
		AItemBase* Target = GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, SpawnPoints->GetActorLocation(), FRotator::ZeroRotator);
		// AItemBase 클래스 스폰하고 Init 하기
		Target->InitItem(Type);

	}
}


void AAssetManagerBase::SpawnProjectileItem(EItemCategoryInfo Type, AActor* SpawnPoints)
{
	if (ProjectileItemAssets[0].IsValid())
	{
		UBlueprint* Gen = Cast<UBlueprint>(ProjectileItemAssets[0].ResolveObject());
		AItemBase* Target = GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, SpawnPoints->GetActorLocation(), FRotator::ZeroRotator);
		// AItemBase 클래스 스폰하고 Init 하기
		Target->InitItem(Type);

	}
}


void AAssetManagerBase::SpawnMissionItem(EItemCategoryInfo Type, AActor* SpawnPoints)
{
		
		if (MissionAssets[0].IsValid())
		{
			UBlueprint* Gen = Cast<UBlueprint>(MissionAssets[0].ResolveObject());
			AItemBase* Target = GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, SpawnPoints->GetActorLocation(), FRotator::ZeroRotator);
			// AItemBase 클래스 스폰하고 Init 하기
			Target->InitItem(Type);

		}
	
}

void AAssetManagerBase::LoadMonsterAsset(TArray<AActor*> SpawnPoints, class ATileBase* TileRef)
{
	TArray<int32> EnemyIDList;
	int32 StageType = FMath::RandRange(0, NormalStageTypeNum - 1);
	//FStageEnemyTypeStruct StageTypeRow = ReturnStageEnemyTypeStruct(StageType);
	FString rowName = FString::FromInt(StageType);
	FStageEnemyTypeStruct* StageTypeRow = StageTypeTable->FindRow<FStageEnemyTypeStruct>(FName(rowName), rowName);
	int8 SpawnNum = FMath::RandRange(StageTypeRow->MinSpawn, StageTypeRow->MaxSpawn);

	UE_LOG(LogTemp, Log, TEXT("StageType : %d MaxSpawn : %d"), StageType, SpawnNum);

	if (StageTypeRow->ID_1001)
	{
		EnemyIDList.AddUnique(1001);

	}

	if (StageTypeRow->ID_1002)
	{
		EnemyIDList.AddUnique(1002);

	}
	if (StageTypeRow->ID_1003)
	{
		EnemyIDList.AddUnique(1003);

	}
	if (StageTypeRow->ID_1100)
	{
		EnemyIDList.AddUnique(1100);

	}
	if (StageTypeRow->ID_1101)
	{
		EnemyIDList.AddUnique(1101);

	}
	if (StageTypeRow->ID_1102)
	{
		EnemyIDList.AddUnique(1102);
	}
	
	
	//int8 SpawnNum = FMath::RandRange(StageTypeRow.MinSpawn, StageTypeRow.MaxSpawn);

	//UE_LOG(LogTemp, Log, TEXT("StageType : %d MaxSpawn : %d"), StageType,SpawnNum);

	//if (StageTypeRow.ID_1001)
	//{
	//	EnemyIDList.AddUnique(1001);
	//	
	//}
	//	
	//if (StageTypeRow.ID_1002)
	//{
	//	EnemyIDList.AddUnique(1002);
	//	
	//}
	//if (StageTypeRow.ID_1003)
	//{
	//	EnemyIDList.AddUnique(1003);
	//	
	//}
	//if (StageTypeRow.ID_1100)
	//{
	//	EnemyIDList.AddUnique(1100);
	//	
	//}
	//if (StageTypeRow.ID_1101)
	//{
	//	EnemyIDList.AddUnique(1101);
	//	
	//}
	//if (StageTypeRow.ID_1102)
	//{
	//	EnemyIDList.AddUnique(1102);
	//}
	AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(EnemyAssets, FStreamableDelegate::CreateUObject(this, &AAssetManagerBase::SpawnMonster, SpawnPoints, TileRef, EnemyIDList,SpawnNum));
}

void AAssetManagerBase::SpawnMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, TArray<int32> EnemyIDList, int8 SpawnNum)
{

	for (int32 i = 0; i < SpawnNum; i++)
	{
		int32 EnemyIDIdx = FMath::RandRange(0, EnemyIDList.Num() - 1);

		if (EnemyIDList[EnemyIDIdx] == 1001 || EnemyIDList[EnemyIDIdx] == 1002)
		{
			if (EnemyAssets[0].IsValid())
			{
				UBlueprint* Gen = Cast<UBlueprint>(EnemyAssets[0].ResolveObject());
				AEnemyCharacterBase* Target = GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
				TileRef->MonsterList.AddUnique(Target);
				Target->EnemyID = EnemyIDList[EnemyIDIdx];
				//Target->InitEnemyStat();
			}
		}
		else if (EnemyIDList[EnemyIDIdx] == 1003)
		{
			if (EnemyAssets[1].IsValid())
			{
				UBlueprint* Gen = Cast<UBlueprint>(EnemyAssets[1].ResolveObject());
				AEnemyCharacterBase* Target = GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
				TileRef->MonsterList.AddUnique(Target);
				Target->EnemyID = EnemyIDList[EnemyIDIdx];
				//Target->InitEnemyStat();
			}
		}
		else if (EnemyIDList[EnemyIDIdx] == 1100)
		{
			if (EnemyAssets[2].IsValid())
			{
				UBlueprint* Gen = Cast<UBlueprint>(EnemyAssets[2].ResolveObject());
				AEnemyCharacterBase* Target = GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
				TileRef->MonsterList.AddUnique(Target);
				Target->EnemyID = EnemyIDList[EnemyIDIdx];
				//Target->InitEnemyStat();
			}
		}
		else if (EnemyIDList[EnemyIDIdx] == 1101 || EnemyIDList[EnemyIDIdx] == 1102)
		{
			if (EnemyAssets[3].IsValid())
			{
				UBlueprint* Gen = Cast<UBlueprint>(EnemyAssets[3].ResolveObject());
				AEnemyCharacterBase* Target = GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
				TileRef->MonsterList.AddUnique(Target);
				Target->EnemyID = EnemyIDList[EnemyIDIdx];
				//Target->InitEnemyStat();
			}

		}
	}
	UE_LOG(LogTemp, Log, TEXT("finish spawn monster"));


	TileRef->BindDelegate();
}

void AAssetManagerBase::LoadMissionMonsterAsset(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, class UMissionBase* MissionRef)
{
	AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(BuffAssets, FStreamableDelegate::CreateUObject(this, &AAssetManagerBase::SpawnMissionMonster, SpawnPoints, TileRef,MissionRef));
}

void AAssetManagerBase::SpawnMissionMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, class UMissionBase* MissionRef)
{
	int32 MaxSpawn = FMath::RandRange(1, 3);

	//for (int32 i = 0; i < MaxSpawn; i++)
	//{
	//	if (MonsterAssets[0].IsValid())
	//	{
	//		UBlueprint* Gen = Cast<UBlueprint>(MonsterAssets[0].ResolveObject());
	//		AEnemyCharacterBase* Target = GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
	//		Target->Tags.AddUnique("MissionMonster");
	//		TileRef->MonsterList.AddUnique(Target);
	//		MissionRef->MonsterList.AddUnique(Target);

	//	}
	//}

	TileRef->BindDelegate();
}

void AAssetManagerBase::LoadWeaponAssets()
{
	WeaponStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(WeaponAssets, FStreamableDelegate::CreateUObject(this, &AAssetManagerBase::CheckWeaponLoad));
}

void AAssetManagerBase::CheckWeaponLoad()
{
	UE_LOG(LogTemp, Log, TEXT("Complete WaeponLoad"));
}

AWeaponBase* AAssetManagerBase::SpawnWeaponwithID(int32 ID)
{
	UBlueprint* Gen;
	AWeaponBase* Target;

	if (ID == 200)  // 캔
	{
		Gen = Cast<UBlueprint>(WeaponAssets[0].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
		//Target->ProjectilePath = WeaponAssets[8];
	}
	else if (ID == 204) // 벽돌
	{
		Gen = Cast<UBlueprint>(WeaponAssets[0].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
		//Target->ProjectilePath = WeaponAssets[7];
	}
	else if (ID == 100) // 유리병
	{
		Gen = Cast<UBlueprint>(WeaponAssets[5].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
	}
	else if (ID == 101) // 나무막대
	{
		Gen = Cast<UBlueprint>(WeaponAssets[10].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
	}
	else if (ID == 102) // 페트병
	{
		Gen = Cast<UBlueprint>(WeaponAssets[6].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
	}
	else if (ID == 103) // 테이저 스틱
	{
		Gen = Cast<UBlueprint>(WeaponAssets[9].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
	}
	else if (ID == 151) // 에너지 블래스터
	{
		Gen = Cast<UBlueprint>(WeaponAssets[4].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
		//Target->ProjectilePath = WeaponAssets[3];
	}
	else // 비비탄 총
	{
		Gen = Cast<UBlueprint>(WeaponAssets[2].ResolveObject());
		Target = GetWorld()->SpawnActor<AWeaponBase>(Gen->GeneratedClass, this->GetActorLocation(), FRotator::ZeroRotator);
		//Target->ProjectilePath = WeaponAssets[1];
	}

	return Target;
}

FStageEnemyTypeStruct AAssetManagerBase::ReturnStageEnemyTypeStruct(int32 StageType)
{
	if (StageTypeTable !=  nullptr)
	{
		FString rowName = FString::FromInt(StageType);
		FStageEnemyTypeStruct* TypeInfo = StageTypeTable->FindRow<FStageEnemyTypeStruct>(FName(rowName), rowName);


		FStageEnemyTypeStruct Data;
		
		Data.ID_1001 = TypeInfo->ID_1001;
		Data.ID_1002 = TypeInfo->ID_1002;
		Data.ID_1003 = TypeInfo->ID_1003;
		Data.ID_1100 = TypeInfo->ID_1100;
		Data.ID_1101 = TypeInfo->ID_1101;
		Data.ID_1102 = TypeInfo->ID_1102;
		Data.ID_1200 = TypeInfo->ID_1200;
		Data.MaxSpawn = TypeInfo->MaxSpawn;
		Data.MinSpawn = TypeInfo->MinSpawn;

		return Data;
	}
	FStageEnemyTypeStruct Data;
	return Data;

}



//
//AProjectileBase* AAssetManagerBase::SpawnProjectilewithPath(FSoftObjectPath path, FTransform SpawnTransform, FActorSpawnParameters SpawnParams)
//{
//	UBlueprint* Gen;
//	AProjectileBase* Target;
//	Gen = Cast<UBlueprint>(path.ResolveObject());
//	Target = GetWorld()->SpawnActor<AProjectileBase>(Gen->GeneratedClass, SpawnTransform, SpawnParams);
//
//	return Target;
//}