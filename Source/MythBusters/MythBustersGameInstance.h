// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/GameInstance.h"
#include <EngineMinimal.h>
#include "ggponet.h"
#include "god.h"
#include "NonGameState.h"
#include "MythBustersGameInstance.generated.h"


struct AbstractCharacter
{
    AGod* ref;
    FTransform transform;
    FVector2D velocity;
    float damage;
};

struct AbstractGameState
{
    TArray<AbstractCharacter> characters;
    void Init(int num_players, UGameInstance* Instance)
    {
        for (int i = 0; i < num_players; i++)
        {
            AGod* god = dynamic_cast<AGod*>(UGameplayStatics::GetPlayerPawn(Instance->GetWorld(), i));
            AbstractCharacter AbstractGod = AbstractCharacter();
            AbstractGod.ref = god;
            AbstractGod.transform = god->GetActorTransform();
            AbstractGod.velocity = god->GetGodMovementComponent()->Velocity;
            AbstractGod.damage = god->GodDamage;
            characters.Add(AbstractGod);
        }
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "GameState Initiated");

    }
    void Apply()
    {
        for (int i = 0; i < characters.Num(); i++)
        {
            if (characters[i].ref != nullptr)
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Loading Character " + FString::FromInt(i));
                characters[i].ref->SetActorTransform(characters[i].transform);
                characters[i].ref->GetGodMovementComponent()->Velocity = characters[i].velocity;
                characters[i].ref->GodDamage = characters[i].damage;
            }

        }

    }
    void Observe()
    {
        for (int i = 0; i < characters.Num(); i++)
        {
            if (characters[i].ref != nullptr)
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Saving Character " + FString::FromInt(i));
                characters[i].transform = characters[i].ref->GetActorTransform();
                characters[i].velocity = characters[i].ref->GetGodMovementComponent()->Velocity;
                characters[i].damage = characters[i].ref->GodDamage;
            }

        }
    }
};


/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UMythBustersGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UMythBustersGameInstance();

public:
	/*static void Static_MythBusters_Init(unsigned short localport, int num_players, TArray<GGPOPlayer> players, int num_spectators);
	static void Static_MythBusters_InitSpectator(unsigned short localport, int num_players, char* host_ip, unsigned short host_port);
	static void Static_MythBusters_DrawCurrentFrame();
	static void Static_MythBusters_AdvanceFrame(int inputs[], int disconnect_flags);
	static void Static_MythBusters_RunFrame();
	static void Static_MythBusters_Idle(int time);
	static void Static_MythBusters_DisconnectPlayer(int player);
	static void Static_MythBusters_Exit();*/

    /*static bool __cdecl mb_begin_game_callback(const char*);
    static bool __cdecl mb_on_event_callback(GGPOEvent* info);
    static bool __cdecl mb_advance_frame_callback(int);
    static bool __cdecl mb_load_game_state_callback(unsigned char* buffer, int len);
    static bool __cdecl mb_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int);
    static void __cdecl mb_free_buffer_callback(void* buffer);*/

    void MythBusters_Init(unsigned short localport, int num_players, TArray<GGPOPlayer> players, int num_spectators);
    //void MythBusters_InitSpectator(unsigned short localport, int num_players, char* host_ip, unsigned short host_port);
    //void MythBusters_DrawCurrentFrame();
    void MythBusters_AdvanceFrame(int inputs[], int disconnect_flags);
    //void MythBusters_RunFrame();
    //void MythBusters_Idle(int time);
    //void MythBusters_DisconnectPlayer(int player);
    //void MythBusters_Exit();

    AbstractGameState gs;
    NonGameState ngs;
    GGPOSession* ggpo;
    static UMythBustersGameInstance* Instance;

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

	/*bool __cdecl mb_advance_frame_callback(int);
	bool __cdecl mb_load_game_state_callback(unsigned char* buffer, int len);
	bool __cdecl mb_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int);*/

	//UMythBustersGameInstance* Instance;
};
