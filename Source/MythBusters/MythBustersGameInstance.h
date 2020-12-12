// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ggponet.h"
#include "MythBustersGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UMythBustersGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	void MythBusters_Init(unsigned short localport, int num_players, GGPOPlayer* players, int num_spectators);
	//void MythBusters_InitSpectator(unsigned short localport, int num_players, char* host_ip, unsigned short host_port);
	//void MythBusters_DrawCurrentFrame();
	void MythBusters_AdvanceFrame(int inputs[], int disconnect_flags);
	//void MythBusters_RunFrame();
	//void MythBusters_Idle(int time);
	//void MythBusters_DisconnectPlayer(int player);
	//void MythBusters_Exit();

public:

	static void Static_MythBusters_Init(unsigned short localport, int num_players, GGPOPlayer* players, int num_spectators);
	static void Static_MythBusters_InitSpectator(unsigned short localport, int num_players, char* host_ip, unsigned short host_port);
	static void Static_MythBusters_DrawCurrentFrame();
	static void Static_MythBusters_AdvanceFrame(int inputs[], int disconnect_flags);
	static void Static_MythBusters_RunFrame();
	static void Static_MythBusters_Idle(int time);
	static void Static_MythBusters_DisconnectPlayer(int player);
	static void Static_MythBusters_Exit();

	unsigned char* _buffer;
	int _len;
	int _checksum;

    UFUNCTION(BlueprintCallable)
    void LoadState();
    UFUNCTION(BlueprintCallable)
    void SaveState();

	UFUNCTION(BlueprintCallable)
	void InitState();


	/*bool __cdecl mb_advance_frame_callback(int);
	bool __cdecl mb_load_game_state_callback(unsigned char* buffer, int len);
	bool __cdecl mb_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int);*/

	//UMythBustersGameInstance* Instance;
};
