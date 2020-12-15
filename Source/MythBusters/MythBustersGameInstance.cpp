// Fill out your copyright notice in the Description page of Project Settings.


#include "MythBustersGameInstance.h"
//#include "CoreMinimal.h"
//#include <Runtime\Engine\Private\GameplayStatics.h>
//#include <Runtime\Engine\Private\GameplayStatics.cpp>
#include <EngineMinimal.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
/*#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>*/
#include <stdio.h>
//#include "NonGameState.h"
//#include "god.h"
#include <timeapi.h>



//#define SYNC_TEST    // test: turn on synctest
#define MAX_PLAYERS     64
#define NUM_PLAYERS     2
#define FRAME_DELAY     2

UMythBustersGameInstance* UMythBustersGameInstance::Instance = nullptr;

/*
 * Simple checksum function stolen from wikipedia:
 *
 *   http://en.wikipedia.org/wiki/Fletcher%27s_checksum
 */

int
fletcher32_checksum(short* data, size_t len)
{
    int sum1 = 0xffff, sum2 = 0xffff;

    while (len) {
        size_t tlen = len > 360 ? 360 : len;
        len -= tlen;
        do {
            sum1 += *data++;
            sum2 += sum1;
        } while (--tlen);
        sum1 = (sum1 & 0xffff) + (sum1 >> 16);
        sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    }

    // Second reduction step to reduce sums to 16 bits 
    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    return sum2 << 16 | sum1;
}

/*
 * vw_begin_game_callback --
 *
 * The begin game callback.  We don't need to do anything special here,
 * so just return true.
 */
bool __cdecl mb_begin_game_callback(const char*)
{
    return true;
}

/*
 * vw_on_event_callback --
 *
 * Notification from GGPO that something has happened.  Update the status
 * text at the bottom of the screen to notify the user.
 */
bool __cdecl mb_on_event_callback(GGPOEvent* info)
{
    int progress;
    switch (info->code) {
    case GGPO_EVENTCODE_CONNECTED_TO_PEER:
        UMythBustersGameInstance::Instance->ngs.SetConnectState(info->u.connected.player, Synchronizing);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Connected - Now Synchronizing...");
        break;
    case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
        progress = 100 * info->u.synchronizing.count / info->u.synchronizing.total;
        UMythBustersGameInstance::Instance->ngs.UpdateConnectProgress(info->u.synchronizing.player, progress);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Synchronizing with peer...");
        break;
    case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
        UMythBustersGameInstance::Instance->ngs.UpdateConnectProgress(info->u.synchronized.player, 100);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Synchronized !");
        break;
    case GGPO_EVENTCODE_RUNNING:
        UMythBustersGameInstance::Instance->ngs.SetConnectState(Running);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Running");
        //renderer->SetStatusText("");
        break;
    case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
        UMythBustersGameInstance::Instance->ngs.SetDisconnectTimeout(info->u.connection_interrupted.player,
            timeGetTime(),
            info->u.connection_interrupted.disconnect_timeout);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Connection lost !");
        break;
    case GGPO_EVENTCODE_CONNECTION_RESUMED:
        UMythBustersGameInstance::Instance->ngs.SetConnectState(info->u.connection_resumed.player, Running);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Connection retrieved !");
        break;
    case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
        UMythBustersGameInstance::Instance->ngs.SetConnectState(info->u.disconnected.player, Disconnected);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Disconnected from peer !");
        break;
    case GGPO_EVENTCODE_TIMESYNC:
        Sleep(1000 * info->u.timesync.frames_ahead / 60);
        break;
    }
    return true;
}


/*
 * vw_advance_frame_callback --
 *
 * Notification from GGPO we should step foward exactly 1 frame
 * during a rollback.
 */
bool __cdecl mb_advance_frame_callback(int)
{
    int inputs[NUM_PLAYERS] = { 0 };
    int disconnect_flags;

    // Make sure we fetch new inputs from GGPO and use those to update
    // the game state instead of reading from the keyboard.
    ggpo_synchronize_input(UMythBustersGameInstance::Instance->ggpo, (void*)inputs, sizeof(int) * NUM_PLAYERS, &disconnect_flags);
    UMythBustersGameInstance::Instance->MythBusters_AdvanceFrame(inputs, disconnect_flags);
    return true;
}

/*
 * mb_load_game_state_callback --
 *
 * Makes our current state match the state passed in by GGPO.
 */
bool __cdecl mb_load_game_state_callback(unsigned char* buffer, int len)
{
    memcpy(&UMythBustersGameInstance::Instance->gs, buffer, len);
    UMythBustersGameInstance::Instance->gs.Apply();
    return true;
}

/*
 * vw_save_game_state_callback --
 *
 * Save the current state to a buffer and return it to GGPO via the
 * buffer and len parameters.
 */
bool __cdecl mb_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int)
{
    UMythBustersGameInstance::Instance->gs.Observe();
    *len = sizeof(UMythBustersGameInstance::Instance->gs);
    *buffer = (unsigned char*)malloc(*len);
    if (!*buffer) {
        return false;
    }
    memcpy(*buffer, &UMythBustersGameInstance::Instance->gs, *len);
    *checksum = fletcher32_checksum((short*)*buffer, *len / 2);
    return true;
}

/*
 * vw_log_game_state --
 *
 * Log the gamestate.  Used by the synctest debugging tool.
 */
 /*bool __cdecl
 vw_log_game_state(char* filename, unsigned char* buffer, int)
 {
     FILE* fp = nullptr;
     fopen_s(&fp, filename, "w");
     if (fp) {
         GameState* gamestate = (GameState*)buffer;
         fprintf(fp, "GameState object.\n");
         fprintf(fp, "  bounds: %d,%d x %d,%d.\n", gamestate->_bounds.left, gamestate->_bounds.top,
             gamestate->_bounds.right, gamestate->_bounds.bottom);
         fprintf(fp, "  num_ships: %d.\n", gamestate->_num_ships);
         for (int i = 0; i < gamestate->_num_ships; i++) {
             Ship* ship = gamestate->_ships + i;
             fprintf(fp, "  ship %d position:  %.4f, %.4f\n", i, ship->position.x, ship->position.y);
             fprintf(fp, "  ship %d velocity:  %.4f, %.4f\n", i, ship->velocity.dx, ship->velocity.dy);
             fprintf(fp, "  ship %d radius:    %d.\n", i, ship->radius);
             fprintf(fp, "  ship %d heading:   %d.\n", i, ship->heading);
             fprintf(fp, "  ship %d health:    %d.\n", i, ship->health);
             fprintf(fp, "  ship %d speed:     %d.\n", i, ship->speed);
             fprintf(fp, "  ship %d cooldown:  %d.\n", i, ship->cooldown);
             fprintf(fp, "  ship %d score:     %d.\n", i, ship->score);
             for (int j = 0; j < MAX_BULLETS; j++) {
                 Bullet* bullet = ship->bullets + j;
                 fprintf(fp, "  ship %d bullet %d: %.2f %.2f -> %.2f %.2f.\n", i, j,
                     bullet->position.x, bullet->position.y,
                     bullet->velocity.dx, bullet->velocity.dy);
             }
         }
         fclose(fp);
     }
     return true;
 }*/

 /*
  * vw_free_buffer --
  *
  * Free a save state buffer previously returned in vw_save_game_state_callback.
  */
  void __cdecl mb_free_buffer_callback(void* buffer)
  {
      free(buffer);
  }
  

  UMythBustersGameInstance::UMythBustersGameInstance()
  {
      Instance = this;
      gs = { };
      ngs = { 0 };
      ggpo = NULL;
  }

  /*
   * VectorWar_Init --
   *
   * Initialize the vector war game.  This initializes the game state and
   * the video renderer and creates a new network session.
   */
void UMythBustersGameInstance::MythBusters_Init(unsigned short localport, int num_players, TArray<GGPOPlayer> players, int num_spectators)
{
    GGPOErrorCode result;

    // Initialize the game state
    gs.Init(num_players, this);
    ngs.num_players = num_players;

    // Fill in a ggpo callbacks structure to pass to start_session.
    GGPOSessionCallbacks cb = { 0 };
    
    cb.begin_game = mb_begin_game_callback;
    cb.advance_frame = mb_advance_frame_callback;
    cb.load_game_state = mb_load_game_state_callback;
    cb.save_game_state = mb_save_game_state_callback;
    cb.free_buffer = mb_free_buffer_callback;
    cb.on_event = mb_on_event_callback;
    //cb.log_game_state = mb_log_game_state_callback;

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Callback binded");

#if defined(SYNC_TEST)
    result = ggpo_start_synctest(&ggpo, &cb, "mythbusters", num_players, sizeof(int), 1);
#else
    result = ggpo_start_session(&ggpo, &cb, "mythbusters", num_players, sizeof(int), localport);
#endif
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, (result == GGPO_OK || result == GGPO_ERRORCODE_SUCCESS) ? "GGPO session started" : "GPPO session failed");
    // automatically disconnect clients after 3000 ms and start our count-down timer
    // for disconnects after 1000 ms.   To completely disable disconnects, simply use
    // a value of 0 for ggpo_set_disconnect_timeout.
    ggpo_set_disconnect_timeout(ggpo, 3000);
    ggpo_set_disconnect_notify_start(ggpo, 1000);

    int i;
    for (i = 0; i < num_players + num_spectators; i++) {
        GGPOPlayerHandle handle;
        result = ggpo_add_player(ggpo, &players[i], &handle);
        ngs.players[i].handle = handle;
        ngs.players[i].type = players[i].type;
        if (players[i].type == GGPO_PLAYERTYPE_LOCAL) {
            ngs.players[i].connect_progress = 100;
            ngs.local_player_handle = handle;
            ngs.SetConnectState(handle, Connecting);
            ggpo_set_frame_delay(ggpo, handle, FRAME_DELAY);
        }
        else {
            ngs.players[i].connect_progress = 0;
        }
    }
}

void UMythBustersGameInstance::CreateLocalPlayer()
{
    GGPOPlayer Player;
    Player.size = sizeof(GGPOPlayer);
    Player.type = GGPO_PLAYERTYPE_LOCAL;
    Players.EmplaceAt(0, Player);
}

void UMythBustersGameInstance::CreateRemotePlayer(FString IPAdress)
{
    GGPOPlayer Player;
    Player.size = sizeof(GGPOPlayer);
    Player.type = GGPO_PLAYERTYPE_REMOTE;
    strcpy_s(Player.u.remote.ip_address, TCHAR_TO_ANSI(*IPAdress));
    Player.u.remote.port = 8001;

    Players.EmplaceAt(1, Player);
}

void UMythBustersGameInstance::StartGGPO()
{
    MythBusters_Init(8001, NUM_PLAYERS, Players, 0);
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Start GGPO");
}

/*
 * VectorWar_InitSpectator --
 *
 * Create a new spectator session
 */
 /*void
 VectorWar_InitSpectator(HWND hwnd, unsigned short localport, int num_players, char* host_ip, unsigned short host_port)
 {
     GGPOErrorCode result;
     renderer = new GDIRenderer(hwnd);

     // Initialize the game state
     gs.Init(hwnd, num_players);
     ngs.num_players = num_players;

     // Fill in a ggpo callbacks structure to pass to start_session.
     GGPOSessionCallbacks cb = { 0 };
     cb.begin_game = vw_begin_game_callback;
     cb.advance_frame = vw_advance_frame_callback;
     cb.load_game_state = vw_load_game_state_callback;
     cb.save_game_state = vw_save_game_state_callback;
     cb.free_buffer = vw_free_buffer;
     cb.on_event = vw_on_event_callback;
     cb.log_game_state = vw_log_game_state;

     result = ggpo_start_spectating(&ggpo, &cb, "vectorwar", num_players, sizeof(int), localport, host_ip, host_port);

     ggpoutil_perfmon_init(hwnd);

     renderer->SetStatusText("Starting new spectator session");
 }*/


 /*
  * VectorWar_DisconnectPlayer --
  *
  * Disconnects a player from this session.
  */

  /*void
  VectorWar_DisconnectPlayer(int player)
  {
      if (player < ngs.num_players) {
          char logbuf[128];
          GGPOErrorCode result = ggpo_disconnect_player(ggpo, ngs.players[player].handle);
          if (GGPO_SUCCEEDED(result)) {
              sprintf_s(logbuf, ARRAYSIZE(logbuf), "Disconnected player %d.\n", player);
          }
          else {
              sprintf_s(logbuf, ARRAYSIZE(logbuf), "Error while disconnecting player (err:%d).\n", result);
          }
          renderer->SetStatusText(logbuf);
      }
  }*/


  /*
   * VectorWar_DrawCurrentFrame --
   *
   * Draws the current frame without modifying the game state.
   */
   /*void
   VectorWar_DrawCurrentFrame()
   {
       if (renderer != nullptr) {
           renderer->Draw(gs, ngs);
       }
   }*/

   /*
    * VectorWar_AdvanceFrame --
    *
    * Advances the game state by exactly 1 frame using the inputs specified
    * for player 1 and player 2.
    */
void UMythBustersGameInstance::MythBusters_AdvanceFrame(int inputs[], int disconnect_flags)
{
    //gs.Update(inputs, disconnect_flags);

    // update the checksums to display in the top of the window.  this
    // helps to detect desyncs.
    /*ngs.now.framenumber = gs._framenumber;
    ngs.now.checksum = fletcher32_checksum((short*)&gs, sizeof(gs) / 2);
    if ((gs._framenumber % 90) == 0) {
        ngs.periodic = ngs.now;
    }*/

    // Notify ggpo that we've moved forward exactly 1 frame.
    ggpo_advance_frame(ggpo);

    // Update the performance monitor display.
    /*GGPOPlayerHandle handles[MAX_PLAYERS];
    int count = 0;
    for (int i = 0; i < ngs.num_players; i++) {
        if (ngs.players[i].type == GGPO_PLAYERTYPE_REMOTE) {
            handles[count++] = ngs.players[i].handle;
        }
    }
    ggpoutil_perfmon_update(ggpo, handles, count);*/
}



/*
 * ReadInputs --
 *
 * Read the inputs for player 1 from the keyboard.  We never have to
 * worry about player 2.  GGPO will handle remapping his inputs
 * transparently.
 */
 /*int
 ReadInputs(HWND hwnd)
 {
     static const struct {
         int      key;
         int      input;
     } inputtable[] = {
        { VK_UP,       INPUT_THRUST },
        { VK_DOWN,     INPUT_BREAK },
        { VK_LEFT,     INPUT_ROTATE_LEFT },
        { VK_RIGHT,    INPUT_ROTATE_RIGHT },
        { 'D',         INPUT_FIRE },
        { 'S',         INPUT_BOMB },
     };
     int i, inputs = 0;

     if (GetForegroundWindow() == hwnd) {
         for (i = 0; i < sizeof(inputtable) / sizeof(inputtable[0]); i++) {
             if (GetAsyncKeyState(inputtable[i].key)) {
                 inputs |= inputtable[i].input;
             }
         }
     }

     return inputs;
 }*/

 /*
  * VectorWar_RunFrame --
  *
  * Run a single frame of the game.
  */
  /*void
  VectorWar_RunFrame(HWND hwnd)
  {
      GGPOErrorCode result = GGPO_OK;
      int disconnect_flags;
      int inputs[MAX_SHIPS] = { 0 };

      if (ngs.local_player_handle != GGPO_INVALID_HANDLE) {
          int input = ReadInputs(hwnd);
  #if defined(SYNC_TEST)
          input = rand(); // test: use random inputs to demonstrate sync testing
  #endif
          result = ggpo_add_local_input(ggpo, ngs.local_player_handle, &input, sizeof(input));
      }

      // synchronize these inputs with ggpo.  If we have enough input to proceed
      // ggpo will modify the input list with the correct inputs to use and
      // return 1.
      if (GGPO_SUCCEEDED(result)) {
          result = ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * MAX_SHIPS, &disconnect_flags);
          if (GGPO_SUCCEEDED(result)) {
              // inputs[0] and inputs[1] contain the inputs for p1 and p2.  Advance
              // the game by 1 frame using those inputs.
              VectorWar_AdvanceFrame(inputs, disconnect_flags);
          }
      }
      VectorWar_DrawCurrentFrame();
  }*/

  /*
   * VectorWar_Idle --
   *
   * Spend our idle time in ggpo so it can use whatever time we have left over
   * for its internal bookkeeping.
   */
   /*void
   VectorWar_Idle(int time)
   {
       ggpo_idle(ggpo, time);
   }*/

   /*void
   VectorWar_Exit()
   {
       memset(&gs, 0, sizeof(gs));
       memset(&ngs, 0, sizeof(ngs));

       if (ggpo) {
           ggpo_close_session(ggpo);
           ggpo = NULL;
       }
       delete renderer;
       renderer = NULL;
   }*/

void UMythBustersGameInstance::LoadState()
{
    //Static_MythBusters_LoadState(_buffer, _len);
    mb_load_game_state_callback(_buffer, _len);
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "State loaded");
}

void UMythBustersGameInstance::SaveState()
{
    //Static_MythBusters_SaveState(&_buffer, &_len, &_checksum, 0);
    mb_save_game_state_callback(&_buffer, &_len, &_checksum, 0);
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "State Saved");
}

void UMythBustersGameInstance::InitState()
{
    gs = {  };
    gs.Init(2, this);
    SaveState();
}
/*
#pragma region StaticMethod

void UMythBustersGameInstance::Static_MythBusters_Init(unsigned short localport, int num_players, TArray<GGPOPlayer> players, int num_spectators)
{
    if (GEngine->GetWorld() != nullptr && GEngine->GetWorld()->GetGameInstance() != nullptr)
    {
        //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));

        if (Instance != nullptr)
        {
            Instance->MythBusters_Init(localport, num_players, players, num_spectators);
        }
        else
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_Init)"));
        }
    }
    
}
void UMythBustersGameInstance::Static_MythBusters_InitSpectator(unsigned short localport, int num_players, char* host_ip, unsigned short host_port)
{
    //UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World())
    //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));
    if (Instance != nullptr)
    {
        //GGPOManagerInstance->MythBusters_InitSpectator(localport, num_players, host_ip, host_port);
    }
    else
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_InitSpectator)"));
    }
}
void UMythBustersGameInstance::Static_MythBusters_DrawCurrentFrame()
{
    //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));
    if (Instance != nullptr)
    {
    //GGPOManagerInstance->MythBusters_DrawCurrentFrame();
    }
    else
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_DrawCurrentFrame)"));
    }
 }
void UMythBustersGameInstance::Static_MythBusters_AdvanceFrame(int inputs[], int disconnect_flags)
{
    //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));
    if (Instance != nullptr)
    {
        Instance->MythBusters_AdvanceFrame(inputs, disconnect_flags);
    }
    else
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_AdvanceFrame)"));
    }
}
void UMythBustersGameInstance::Static_MythBusters_RunFrame()
{
    //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));
    if (Instance != nullptr)
    {
        //GGPOManagerInstance->MythBusters_RunFrame();
    }
    else
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_RunFrame)"));
    }
}
void UMythBustersGameInstance::Static_MythBusters_Idle(int time)
{
    //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));
    if (Instance != nullptr)
    {
        //GGPOManagerInstance->MythBusters_Idle(time);
    }
    else
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_Idle)"));
    }
}
void UMythBustersGameInstance::Static_MythBusters_DisconnectPlayer(int player)
{
    //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));
    if (Instance != nullptr)
    {
        //GGPOManagerInstance->MythBusters_DisconnectPlayer(player);
    }
    else
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_DisconnectPlayer)"));
    }
}
void UMythBustersGameInstance::Static_MythBusters_Exit()
{
    //UMythBustersGameInstance* Instance = dynamic_cast<UMythBustersGameInstance*>(UGameplayStatics::GetGameInstance(GEngine->GetWorldContexts()[0].World()));
    if (Instance != nullptr)
    {
        //GGPOManagerInstance->MythBusters_Exit();
    }
    else
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("GGPOManagerInstance is null ! (on Static_MythBusters_Exit)"));
    }
}
#pragma endregion
*/