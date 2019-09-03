// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "MyTypes.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EWinType : uint8
{
	WT_None		UMETA(DisplayName = "None"),
	WT_Win		UMETA(DisplayName = "Win"),
	WT_Lose		UMETA(DisplayName = "Lose"),
	WT_Draw		UMETA(DisplayName = "Draw"),
	WT_Continue	UMETA(DisplayName = "Continue")

};

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	PT_None		UMETA(DisplayName = "None"),
	PT_Player	UMETA(DisplayName = "Player"),
	PT_AI		UMETA(DisplayName = "AI")


};


USTRUCT(BlueprintType)
struct FCellStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 x;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 y;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EPlayerType Owner = EPlayerType::PT_None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool ischecked = false;

	FCellStruct() {};
	FCellStruct(int32 ArrX, int32 ArrY) :x(ArrX), y(ArrY) {
		Owner = EPlayerType::PT_None;
	};


};