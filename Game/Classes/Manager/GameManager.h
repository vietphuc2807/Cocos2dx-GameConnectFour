#pragma once
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <string>

struct PlayerInfo {
    std::string name;
    std::string fruit;
};

class GameManager {
public:
    static GameManager* getInstance() {
        static GameManager instance;
        return &instance;
    }

    PlayerInfo player1;
    PlayerInfo player2;

	PlayerInfo playerWithAI;
    PlayerInfo playerAI;
private:
    GameManager() {}
};

#endif // __GAME_MANAGER_H__#pragma once
