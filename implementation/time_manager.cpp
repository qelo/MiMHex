#include "time_manager.h"

#include "conditional_assert.h"
#include "params.h"

void Timer::Start()
{
    begin = Clock();
    last = begin;
    buffer = 8;
    last_buffer = buffer;
    delay = 0;
    request = 0;
}

uint Timer::Clock()
{
    uint64 now = clock();
    return now * 1000 / CLOCKS_PER_SEC;
}

uint Timer::State()
{
    if (request == buffer - 1) {
        uint now = Clock();
        delay = now - last;
        if (delay < kClockDelay) {
            last_buffer = buffer;
            buffer *= 2;
        }
        last = now;
        request = 0;
    } else
        ++request;
    return last + request * delay / last_buffer - begin;
}

TimeManager::TimeManager()
    : management(kManagementPlayoutsPerMove),
    time_left(60000), playout_moves_left(50000000),
    playouts_per_move(100000), pmoves_per_move(1000000),
    resources_left(0), resources_used(0), current_move(0)
{
}

void TimeManager::NewMove()
{
    switch (management) {
        case kManagementPlayoutsPerMove:
            resources_left = playouts_per_move;
            break;
        case kManagementPlayoutMovesPerGame:
            ASSERT(Hex::Params::resources_part <= 1.0);
            resources_left = Hex::Params::resources_part * playout_moves_left;
            break;
        case kManagementTime:
            ASSERT(Hex::Params::resources_part <= 1.0);
            resources_left = Hex::Params::resources_part * time_left;
            timer.Start();
            break;
        case kManagementPlayoutMovesPerMove:
            resources_left = pmoves_per_move;
    }

    resources_used = 0;
}

bool TimeManager::NewPlayout(uint empties)
{
    if (resources_used >= resources_left)
        return false;

    switch (management) {
        case kManagementPlayoutsPerMove:
            ++resources_used;
            break;
        case kManagementPlayoutMovesPerMove:
        case kManagementPlayoutMovesPerGame:
            resources_used += empties;
            break;
        case kManagementTime:
            resources_used = timer.State();
            break;
    }
    return true;
}

void TimeManager::EndMove()
{
    switch (management) {
        case kManagementPlayoutMovesPerGame:
            playout_moves_left = (resources_used > playout_moves_left ? 0 : playout_moves_left - resources_used);
            break;
        case kManagementTime: {
            uint used = timer.State();
            time_left = (used > time_left ? 0 : time_left - used);
            break;
        }
        default:
            break;
    }
    ++current_move;
}
