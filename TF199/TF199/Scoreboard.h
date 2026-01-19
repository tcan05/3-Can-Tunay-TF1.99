#pragma once

struct Scoreboard
{
    int blueWins = 0;
    int redWins  = 0;

    void Reset()
    {
        blueWins = 0;
        redWins = 0;
    }

    void AddWin(int winnerPlayerId)
    {
        if (winnerPlayerId == 1) blueWins++;
        else if (winnerPlayerId == 2) redWins++;
    }
};
