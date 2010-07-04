#ifndef MIMHEX_IMPLEMENTATION_TIME_MANAGER_H
#define MIMHEX_IMPLEMENTATION_TIME_MANAGER_H

#include "hextypes.h"

const uint kClockDelay = 100;

struct Timer {
    void Start();
    uint Clock();
    uint State();

private:
    uint begin;
    uint last;
    uint buffer, last_buffer;
    uint request;
    uint delay;
};

enum TimeManagementType {
    kManagementPlayoutsPerMove = 0,
    kManagementTime = 1,
    kManagementPlayoutMovesPerMove = 2,
    kManagementPlayoutMovesPerGame = 3
};

const static uint kManagementTypes = 4;

struct TimeManager
{
    TimeManager();
    void NewMove();
    bool NewPlayout(uint);
    void EndMove();

    /**
     * Estimate playouts left in current move
     * Return value -1 means there is no way to predict it
     */
    int PlayoutsLeft();

    TimeManagementType management;

    uint time_left;
    uint64 playout_moves_left;
    uint playouts_per_move;
    uint64 pmoves_per_move;

private:
    Timer timer;

    uint64 resources_left;
    uint64 resources_used;
    uint current_playout;
    uint current_move;
};

#endif
