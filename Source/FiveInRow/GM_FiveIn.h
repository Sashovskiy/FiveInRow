// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyTypes.h"
#include "GM_FiveIn.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNextTurn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndMove);

UCLASS()
class FIVEINROW_API AGM_FiveIn : public AGameModeBase
{
	GENERATED_BODY()


	int32 FieldSize = 8;
	TArray<FCellStruct> Field;
	EPlayerType Winner = EPlayerType::PT_None;

public:

	EPlayerType CurrentPlayer;
bool SetSize(int32 Size_l);
	//Checking if one of player win
	EWinType isWin();
	TArray<FCellStruct> FindAllCellsbyOwner(EPlayerType owner);
	EWinType Checking(int32 CheckingTYPE, FCellStruct cell, int32 count);
	
	//Turn system 
	void Start();
	UFUNCTION()
	void EndMove();
	UPROPERTY(BlueprintAssignable)
	FWin SomeOneWin;
	UPROPERTY(BlueprintAssignable)
	FNextTurn NextTurn;
	UPROPERTY(BlueprintAssignable)
	FEndMove CallEndMove;
	};
