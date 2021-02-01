// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/GameInstance.h"
#include <EngineMinimal.h>
#include "ggponet.h"
#include "god.h"
#include "NonGameState.h"
#include "MythBustersGameInstance.generated.h"


//#define SYNC_TEST    // test: turn on synctest
#define MAX_PLAYERS     2
#define NUM_PLAYERS     2
#define FRAME_DELAY     2
//#define SYNC_TEST

/*
	Class to store the whole state of the game each frame.
	It is only the necessary variables to completely describe the game.
	Useful for rollbacks. 

*/
struct MYTHBUSTERS_API SAbstractGameState
{
	TArray<SAbstractGod> Gods;
	int _framenumber = 0;
	void Init(int num_players, UGameInstance* Instance)
	{
		for (int i = 0; i < num_players; i++)
		{
			AGod* god = (AGod*)Instance->GetLocalPlayers()[i]->PlayerController->GetPawn();
			SAbstractGod AbstractGod = SAbstractGod();
			AbstractGod.Init(god);
			Gods.Add(AbstractGod);
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "GameState Initiated");

	}

	/*
		Apply the stored state (variables values) to the real game state (Used when loading a state)
	*/
	void Apply()
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Rollback !");
		for (int i = 0; i < Gods.Num(); i++)
		{
			if (Gods[i].Ref != nullptr)
			{
				Gods[i].Apply();
			}

		}


	}
	/*
		Read the real game state and store the state inside the instance variables
	*/
	void Observe()
	{
		for (int i = 0; i < Gods.Num(); i++)
		{
			if (Gods[i].Ref != nullptr)
			{
				Gods[i].Observe();
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Saving Character " + FString::FromInt(i));
			}

		}

	}
};


/**
 * The Game Instance of the game, unique instance, persistent during the entire game. Useful for GGPO online game
 */
UCLASS()
class MYTHBUSTERS_API UMythBustersGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UMythBustersGameInstance();
	~UMythBustersGameInstance();

public:

	void MythBusters_Init(unsigned short localport, int num_players, TArray<GGPOPlayer> players, int num_spectators);
	//void MythBusters_InitSpectator(unsigned short localport, int num_players, char* host_ip, unsigned short host_port);
	//void MythBusters_DrawCurrentFrame();
	void MythBusters_AdvanceFrame(SSendableInputs inputs[], int disconnect_flags);
	UFUNCTION(BlueprintCallable)
		void MythBusters_RunFrame();
	//void MythBusters_Idle(int time);
	void MythBusters_DisconnectPlayer(int player);
	void MythBusters_Exit();

	SAbstractGameState gs;
	NonGameState ngs;
	GGPOSession* ggpo;
	static UMythBustersGameInstance* Instance;
	SSendableInputs Inputs[MAX_PLAYERS];
	int PacketSize;

	unsigned char* _buffer;
	int _len;
	int _checksum;

	UFUNCTION(BlueprintCallable)
		void LoadState();
	UFUNCTION(BlueprintCallable)
		void SaveState();

	UFUNCTION(BlueprintCallable)
		void InitState();

	TArray<GGPOPlayer> Players;

	/*UFUNCTION(BlueprintCallable)
	void CreateLocalPlayer();*/

	UFUNCTION(BlueprintCallable)
		void CreateRemotePlayer(FString IPAdress);

	UFUNCTION(BlueprintCallable)
		void StartGGPO();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int GGPOPlayerIndex;

	UFUNCTION(BlueprintCallable)
		void MythBusters_Idle(int timeout);

	UFUNCTION(BlueprintCallable)
		void MythBusters_NextFrame();

	void MainThreadSleep(float time);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool rollbacking = false;
    bool InputsReadyForFrame = false;
    bool GGPOIdleDone = false;

	/*bool __cdecl mb_advance_frame_callback(int);
	bool __cdecl mb_load_game_state_callback(unsigned char* buffer, int len);
	bool __cdecl mb_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int);*/

	//UMythBustersGameInstance* Instance;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<int> SelectedGods = { -1, -1 , 0 , 0 }; // 0 = local selected god; 1 = remote selected god, 2 = just recieved remote selection ,3 = sending local selection to remote
	//char ThorSelectedCode = 0b01111111;
	//char Thor2SelectedCode = 0b01111110;
	//char GodSelectionReceived = 0b01111101;
	//char NoActionCode = 0b0000000;
	//char SendGodSelection(char act);
	//void ReadGodSelection(char act);
};
