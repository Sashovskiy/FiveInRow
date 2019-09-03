// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_FiveIn.h"
#include "Engine.h"	
#include "Runtime/Engine/Classes/Sound/SoundCue.h"

bool AGM_FiveIn::SetSize(int32 Size_l)
{
	if(Size_l < 8)
	return false;

	FieldSize = Size_l;
	return true;
}

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
					UE_LOG(LogTemp, Warning, TEXT("Lose"))
					return EWinType::WT_Lose;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Win"))
					return EWinType::WT_Win;
				}
		}

	}
	if (isDraw())
		return EWinType::WT_Draw;

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
	UE_LOG(LogTemp, Warning, TEXT("Type: %d count: %d"), CheckingTYPE ,count)
	if (count >= 5 )
	{
		return EWinType::WT_Win;
	}
	
	switch (CheckingTYPE)
	{
		//Cheking vertical win
	case 0:
		if(cell.y != (FieldSize - 1))
			if (Field[(cell.y +1) * FieldSize + cell.x].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[(cell.y + 1) * FieldSize + cell.x], count);
		break;
		//Cheking horizontal win
	case 1:
		if(cell.x != (FieldSize - 1))
			if (Field[cell.y * FieldSize + (cell.x+1)].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[cell.y * FieldSize + (cell.x + 1)], count);
		break;
		//Cheking  first diagonal win
	case 2:
		if((cell.y != (FieldSize - 1))&&(cell.x != (FieldSize - 1)))
			if (Field[(cell.y +1) * FieldSize + (cell.x +1)].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[(cell.y + 1) * FieldSize + (cell.x + 1)], count);
		break;
		//Cheking  second win
	case 3:
		if ((cell.y != (FieldSize - 1)) && (cell.x != 0))
			if (Field[(cell.y+1) * FieldSize + (cell.x - 1)].Owner == CurrentPlayer)
				return Checking(CheckingTYPE, Field[(cell.y + 1) * FieldSize + (cell.x - 1)], count);
		break;

	default:
		break;
	}
	return EWinType::WT_Continue;
}

bool AGM_FiveIn::isDraw()
{
	for (FCellStruct cell : Field)
	{
		if (cell.Owner == EPlayerType::PT_None)
			return false;

	}

	return true;
}

void AGM_FiveIn::Start()
{
	for (int32 i = 0; i < FieldSize; i++)
	{
		for (int32 j = 0; j < FieldSize; j++)
		{
			Field.Add(FCellStruct(j,i ));
		}//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Start")));
	}
	CurrentPlayer = EPlayerType::PT_Player;
	CallEndMove.AddDynamic(this, &AGM_FiveIn::EndMove);
	StartNEwGame.Broadcast();
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

	if (CurrentPlayer == EPlayerType::PT_AI) 
	{
		/*bool isAI;
		makeMove(FindCellToMove(), isAI);
		USoundCue* AIPlaceSound = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Engine/VREditor/Sounds/VR_open_Cue.VR_open_Cue'"));
		UGameplayStatics::PlaySound2D(this, AIPlaceSound);*/
		AiMove.Broadcast(FindCellToMove());
		
	}
		

}

bool AGM_FiveIn::makeMove(FCellStruct cell_l, bool& isAiMove)
{
	isAiMove = (CurrentPlayer == EPlayerType::PT_AI);

	if(cell_l.Owner != EPlayerType::PT_None)
	return false;

	if (Field[cell_l.y*FieldSize + cell_l.x].Owner != EPlayerType::PT_None)
		return false;

	Field[cell_l.y * FieldSize + cell_l.x].Owner = CurrentPlayer;

	CallEndMove.Broadcast();
	return true;
}

FCellStruct AGM_FiveIn::FindCellToMove()
{
	bool isFree = false;
	TArray<FCellStruct> EnemyCells = FindAllCellsbyOwner(EPlayerType::PT_Player);
	
//	FCellStruct cell = EnemyCells[FMath::RandRange(0, (EnemyCells.Num() - 1))];
	
	for (FCellStruct cell : EnemyCells)
	{
		FCellStruct temp = RandCellNear(cell);
		if (temp.x != -1)
			return temp;
	}
	return FCellStruct(-1, -1);
}

FCellStruct AGM_FiveIn::RandCellNear(FCellStruct EnemeCell)
{

		if (EnemeCell.y != (FieldSize - 1))
			if (Field[(EnemeCell.y + 1) * FieldSize + EnemeCell.x].Owner == EPlayerType::PT_None)
				return  Field[(EnemeCell.y + 1) * FieldSize + EnemeCell.x];

		if (EnemeCell.x != (FieldSize - 1))
			if (Field[EnemeCell.y * FieldSize + (EnemeCell.x + 1)].Owner == EPlayerType::PT_None)
				return  Field[EnemeCell.y * FieldSize + (EnemeCell.x + 1)];

		if ((EnemeCell.y != (FieldSize - 1)) && (EnemeCell.x != (FieldSize - 1)))
			if (Field[(EnemeCell.y + 1) * FieldSize + (EnemeCell.x + 1)].Owner == EPlayerType::PT_None)
				return Field[(EnemeCell.y + 1) * FieldSize + (EnemeCell.x + 1)];
		if ((EnemeCell.y != (FieldSize - 1)) && (EnemeCell.x != 0))
			if (Field[(EnemeCell.y + 1) * FieldSize + (EnemeCell.x - 1)].Owner == EPlayerType::PT_None)
				return Field[(EnemeCell.y + 1) * FieldSize + (EnemeCell.x - 1)];

		return FCellStruct(-1,-1);

	
}


void AGM_FiveIn::BeginPlay()
{
	Super::BeginPlay();

	



}