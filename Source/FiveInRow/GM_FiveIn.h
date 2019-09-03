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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartGame);

UCLASS()
class FIVEINROW_API AGM_FiveIn : public AGameModeBase
{
	GENERATED_BODY()





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	int32 FieldSize = 8;
	TArray<FCellStruct> Field;
	EPlayerType Winner = EPlayerType::PT_None;



public:

	EPlayerType CurrentPlayer;
UFUNCTION(BlueprintCallable)
bool SetSize(int32 Size_l);
UFUNCTION(BlueprintCallable)
int32 GetSize() const { return FieldSize; };
UFUNCTION(BlueprintCallable)
TArray<FCellStruct> GetFieldData() const { return Field; };
UFUNCTION(BlueprintCallable)
EPlayerType GetCurrentPlayer() const { return CurrentPlayer; };
UFUNCTION(BlueprintCallable)
EPlayerType GetWinner() const { return Winner; };

	//Checking if one of player win
	EWinType isWin();
	TArray<FCellStruct> FindAllCellsbyOwner(EPlayerType owner);
	EWinType Checking(int32 CheckingTYPE, FCellStruct cell, int32 count);
	
	//Turn system 
	UFUNCTION(BlueprintCallable)
	void Start();
	UFUNCTION()
	void EndMove();
	UPROPERTY(BlueprintAssignable)
	FWin SomeOneWin;
	UPROPERTY(BlueprintAssignable)
	FNextTurn NextTurn;
	UPROPERTY(BlueprintAssignable)
	FEndMove CallEndMove;
	UPROPERTY(BlueprintAssignable)
		FStartGame StartNEwGame;

	//Move Sustem
	UFUNCTION(BlueprintCallable)
	bool makeMove(FCellStruct cell_l, bool& isAiMove);

	};
