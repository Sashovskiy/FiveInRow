// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_FiveIn.h"

EWinType AGM_FiveIn::isWin()
{
	TArray<FCellStruct> PCells =  FindAllCellsbyOwner(CurrentPlayer);
	if (PCells.Num() < 5)
		return EWinType::WT_Continue;
	for (FCellStruct cell : PCells)
	{
		for (int i = 0; i < 4; i++)
		{
			if(Checking(i, cell, 0) == EWinType::WT_Win)
				if (CurrentPlayer == EPlayerType::PT_AI)
				{
					return EWinType::WT_Lose;
				}
				else
				{
					return EWinType::WT_Win;
				}
		}

	}
	return EWinType::WT_Continue;
}

TArray<FCellStruct> AGM_FiveIn::FindAllCellsbyOwner(EPlayerType owner)
{
	TArray<FCellStruct> Temp;
	for (FCellStruct cell :Field)
	{
		if (cell.Owner == owner)
			Temp.Add(cell);
	}

	return Temp;
}

EWinType AGM_FiveIn::Checking(int32 CheckingTYPE, FCellStruct cell, int32 count)
{
	count++;
	if (count >= 5 )
	{
		return EWinType::WT_Win;
	}
	
	switch (CheckingTYPE)
	{
		//Cheking vertical win
	case 0:
		if(cell.y != 0)
			if (Field[(cell.y +1) * FieldSize + cell.x].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[cell.y * FieldSize + cell.x], count);
		break;
		//Cheking horizontal win
	case 1:
		if(cell.x != (FieldSize - 1))
			if (Field[cell.y * FieldSize + (cell.x+1)].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[cell.y * FieldSize + cell.x], count);
		break;
		//Cheking  first diagonal win
	case 2:
		if((cell.y != 0)&&(cell.x != (FieldSize - 1)))
			if (Field[cell.y * FieldSize + cell.x].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[cell.y * FieldSize + cell.x], count);
		break;
		//Cheking  second win
	case 3:
		if ((cell.y != 0) && (cell.x != 0))
			if (Field[cell.y * FieldSize + cell.x].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[cell.y * FieldSize + cell.x], count);
		break;

	default:
		break;
	}
	return EWinType::WT_Continue;
}

void AGM_FiveIn::Start()
{
	for (int32 i = 0; i < (FieldSize * FieldSize); i++)
	{
		Field.Add(FCellStruct((i -(8* (i / FieldSize)) ),(i/ FieldSize)));
	}
	CurrentPlayer = EPlayerType::PT_Player;
	CallEndMove.AddDynamic(this, &AGM_FiveIn::EndMove);
}

void AGM_FiveIn::EndMove()
{
	switch (isWin())
	{

	case EWinType::WT_Win:
		Winner = EPlayerType::PT_Player;
		SomeOneWin.Broadcast();
		break;
	case EWinType::WT_Lose:
		Winner = EPlayerType::PT_AI;
		SomeOneWin.Broadcast();
		break;
	case EWinType::WT_Draw:
		SomeOneWin.Broadcast();
		break;
	case EWinType::WT_Continue:
		break;
	}

	CurrentPlayer = (CurrentPlayer == EPlayerType::PT_Player) ? (EPlayerType::PT_AI) : (EPlayerType::PT_Player);

	NextTurn.Broadcast();



}
