// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterController.h"
#include "../public/MainCharacterBase.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetMathLibrary.h"

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

	this->bShowMouseCursor = true;

	PlayerRef = Cast<AMainCharacterBase>(GetCharacter());
}

FRotator AMainCharacterController::GetAimingRotation()
{
	//�ӽ� �ڵ�) ���� Attach���ڸ��� Gamemode�� ���� UI, Controller ������ Delegate Broadcast
	//auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	//if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
	//{
		//return GetRightAnalogRotation();
	//	return FRotator();
	//}
	return GetRotationToCursor();
}

FRotator AMainCharacterController::GetRotationToCursor()
{
	if (!IsValid(PlayerRef)) return FRotator();

	FRotator retRotation;
	
	//Trace�� ������ ���콺�� World Location
	FVector traceStartLocation, traceEndLocation, mouseDirection;
	FHitResult hitResult;

	DeprojectMousePositionToWorld(traceStartLocation, mouseDirection);

	//���콺 Ŀ�� ��ġ���� �ٴ������� INF��ŭ�� ��ġ�� Trace�� ������ ����
	traceEndLocation = traceStartLocation + mouseDirection * 250000.0f;

	//ī�޶󿡼� Ŀ���� �ٴ� ��ġ���� LineTrace�� ���� -> ���� Ŀ���� ���� ��ȣ�ۿ� ��ġ�� hitResult.Location�� ���
	GetWorld()->LineTraceSingleByChannel(hitResult, traceStartLocation, traceEndLocation
		, ECollisionChannel::ECC_Camera);

	//hit�� �����ߴٸ�
	if (hitResult.bBlockingHit)
	{
		retRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), hitResult.Location);
		retRotation = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, retRotation.Yaw + 270.0f);
		return retRotation;
	}

	return FRotator();
}

bool AMainCharacterController::GetActionKeyIsDown(FName MappingName)
{
	TArray<FInputActionKeyMapping> actionKeyMappingList;
	UInputSettings::GetInputSettings()->GetActionMappingByName(MappingName, actionKeyMappingList);

	for (FInputActionKeyMapping& inputKey : actionKeyMappingList)
	{
		if (IsInputKeyDown(inputKey.Key))
		{
			return true;
		}
	}
	return false;
}