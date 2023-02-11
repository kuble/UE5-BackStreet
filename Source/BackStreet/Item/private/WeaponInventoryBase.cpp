// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/WeaponInventoryBase.h"
#include "../public/WeaponBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#define INVALID_INVENTORY_IDX -1

AWeaponInventoryBase::AWeaponInventoryBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
}

void AWeaponInventoryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponInventoryBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponInventoryBase::InitInventory()
{
	if (!IsValid(GetOwner()) || !GetOwner()->ActorHasTag("Character"))
	{
		UE_LOG(LogTemp, Warning, TEXT("AInventory는 CharacterBase 이외의 클래스에서는 소유할 수 없습니다."));
		return;
	}
	OwnerCharacterRef = Cast<ACharacterBase>(GetOwner());
	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());

	InventoryArray.Empty();

	//미리 클래스 / Id 정보를 초기화 함 
	for (int32 classIdx = 0; classIdx < WeaponIDList.Num(); classIdx++)
	{
		WeaponClassInfoMap.Add(WeaponIDList[classIdx], WeaponClassList[classIdx]);
	}
}

void AWeaponInventoryBase::EquipWeapon(int32 InventoryIdx)
{
	if (!InventoryArray.IsValidIndex(InventoryIdx)) return;

	AWeaponBase* newWeapon = SpawnWeaponActor(InventoryArray[InventoryIdx].WeaponID);
	if (IsValid(newWeapon))
	{
		SetCurrentWeaponRef(newWeapon);
		OwnerCharacterRef->EquipWeapon(GetCurrentWeaponRef());
		SetCurrentIdx(InventoryIdx);
		SyncCurrentWeaponInfo();
	}
}

bool AWeaponInventoryBase::AddWeapon(int32 NewWeaponID)
{
	if (!IsValid(OwnerCharacterRef)) return false;
	if (!WeaponClassInfoMap.Contains(NewWeaponID)) return false;

	FWeaponStatStruct newWeaponStat = GamemodeRef->GetWeaponStatInfoWithID(NewWeaponID);

	//먼저, 원거리 무기의 중복 여부를 판단. 중복된다면 Ammo를 추가
	int32 duplicateIdx = CheckWeaponDuplicate(NewWeaponID);
	if (newWeaponStat.bHasProjectile && duplicateIdx != -1)
	{
		InventoryArray[duplicateIdx].WeaponState.TotalAmmoCount += 25;
		return false;
	}
	
	//그렇지 않다면 무기 용량 체크를 진행하고 무기 추가를 진행
	else if (newWeaponStat.WeaponWeight == 0 || newWeaponStat.WeaponWeight + CurrentCapacity <= MaxCapacity)
	{
		InventoryArray.Add({ NewWeaponID, newWeaponStat, FWeaponStateStruct() });
		if (GetCurrentWeaponCount() == 1) //기존 인벤토리가 비어있었다면? 바로 장착까지 해줌
		{
			EquipWeapon(InventoryArray.Num() - 1);
		}
		CurrentCapacity += newWeaponStat.WeaponWeight;
		SortInventory();

		return true;
	}
	return false;
}

void AWeaponInventoryBase::RemoveWeapon(int32 InventoryIdx)
{
	if (!InventoryArray.IsValidIndex(InventoryIdx)) return;

	InventoryArray.RemoveAt(GetCurrentIdx());
	if (GetCurrentWeaponCount() >= 1)
	{
		SetCurrentIdx(0);
		EquipWeapon(0);
		SortInventory();
	}
}

bool AWeaponInventoryBase::SwitchToNextWeapon()
{
	const int32 nextIdx = GetNextInventoryIdx();
	if (GetCurrentWeaponCount() <= 1 || nextIdx == INVALID_INVENTORY_IDX) return false;
	if (!InventoryArray.IsValidIndex(nextIdx)) return false;
	if (!IsValid(OwnerCharacterRef)) return false;

	RestoreCurrentWeapon(); //현재 무기를 Destroy하고, 정보만 List에 저장한다.
	EquipWeapon(nextIdx);
	SortInventory();

	return false;
}

void AWeaponInventoryBase::SyncCurrentWeaponInfo()
{
	if (!IsValid(OwnerCharacterRef)) return;
	if (GetCurrentWeaponCount() == 0) return;
	if (!InventoryArray.IsValidIndex(GetCurrentIdx()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SyncCurrentWeaponInfo : Invalid Index"));
		return;
	}

	if (!IsValid(GetCurrentWeaponRef()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SyncCurrentWeaponInfo : Weapon Not Found"));
		return;
	}
	InventoryArray[GetCurrentIdx()].WeaponStat = GetCurrentWeaponRef()->GetWeaponStat();
	InventoryArray[GetCurrentIdx()].WeaponState = GetCurrentWeaponRef()->GetWeaponState();
}

AWeaponBase* AWeaponInventoryBase::SpawnWeaponActor(int32 WeaponID)
{
	if (!WeaponClassInfoMap.Contains(WeaponID)) return nullptr;
	
	UClass* targetClass = *(WeaponClassInfoMap.Find(WeaponID));
	if (!targetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Class가 Invalid 합니다."));
		return nullptr;
	}
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = OwnerCharacterRef;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector spawnLocation = OwnerCharacterRef->GetActorLocation();
	FRotator spawnRotation = FRotator();
	AWeaponBase* newWeapon = Cast<AWeaponBase>(GetWorld()->SpawnActor(targetClass, &spawnLocation, &spawnRotation, spawnParams));

	return newWeapon;
}

void AWeaponInventoryBase::RestoreCurrentWeapon()
{
	if (GetCurrentWeaponCount() <= 1) return;
	if (!IsValid(OwnerCharacterRef)) return;

	SyncCurrentWeaponInfo();
	GetCurrentWeaponRef()->Destroy();
}

void AWeaponInventoryBase::ClearInventory()
{
	RemoveCurrentWeapon();
	InventoryArray.Empty();
}

void AWeaponInventoryBase::SortInventory()
{
	if (InventoryArray.Num() <= 1) return;

	//Inventory의 최대 Len이 10이기 때문에
	//O(n^2)이어도 상수 시간에 근접한 결과를 냄
	for (int32 selIdx = 0; selIdx < GetCurrentWeaponCount()-1; selIdx++)
	{
		FInventoryItemInfoStruct tempInfo = FInventoryItemInfoStruct();
		for (int32 compIdx = selIdx + 1; compIdx < GetCurrentWeaponCount(); compIdx++)
		{
			const int32 selWeight = InventoryArray[selIdx].WeaponStat.WeaponWeight;
			const int32 compWeight = InventoryArray[compIdx].WeaponStat.WeaponWeight;
			if (selWeight > compWeight)
			{
				tempInfo = InventoryArray[selIdx];
				InventoryArray[selIdx] = InventoryArray[compIdx];
				InventoryArray[compIdx] = tempInfo;

				if (selWeight == GetCurrentIdx())
				{
					SetCurrentIdx(compIdx);
				}
			}
		}
	}
}

void AWeaponInventoryBase::RemoveCurrentWeapon()
{
	if (!IsValid(GetCurrentWeaponRef())) return;
	CurrentWeaponRef->Destroy();
	RemoveWeapon(GetCurrentIdx());
}

int32 AWeaponInventoryBase::CheckWeaponDuplicate(int32 TargetWeaponID)
{
	for (int32 inventoryIdx = 0; inventoryIdx < InventoryArray.Num(); inventoryIdx++)
	{
		auto& weaponInfo = InventoryArray[inventoryIdx];
		if (weaponInfo.WeaponID == TargetWeaponID)
		{
			return inventoryIdx;
		}
	}
	return -1;
}

AWeaponBase* AWeaponInventoryBase::GetCurrentWeaponRef()
{
	if (!IsValid(CurrentWeaponRef)) return nullptr;
	return CurrentWeaponRef;
}

FInventoryItemInfoStruct AWeaponInventoryBase::GetCurrentWeaponInfo()
{
	if(GetCurrentWeaponCount() == 0) return FInventoryItemInfoStruct();
	return InventoryArray[GetCurrentIdx()];
}

int32 AWeaponInventoryBase::GetNextInventoryIdx()
{
	if (GetCurrentWeaponCount() == 0) return 0;
	return (GetCurrentIdx() + 1) % InventoryArray.Num();
}

void AWeaponInventoryBase::SetCurrentWeaponRef(AWeaponBase* NewWeapon)
{
	if (!IsValid(NewWeapon)) return;
	CurrentWeaponRef = NewWeapon;
	CurrentWeaponRef->WeaponDestroyDelegate.BindUFunction(this, FName("RemoveCurrentWeapon"));
	CurrentWeaponRef->SetOwnerCharacter(OwnerCharacterRef);
}
