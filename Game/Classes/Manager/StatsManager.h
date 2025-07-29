#pragma once
#ifndef __STATS_MANAGER_H__
#define __STATS_MANAGER_H__

#include "cocos2d.h"

class StatsManager {
public:
    static StatsManager* getInstance() {
        static StatsManager instance;
        return &instance;
    }

    void addWin() {
        int win = cocos2d::UserDefault::getInstance()->getIntegerForKey("ai_wins", 0);
        cocos2d::UserDefault::getInstance()->setIntegerForKey("ai_wins", win + 1);
        cocos2d::UserDefault::getInstance()->flush();
    }

    void addLose() {
        int lose = cocos2d::UserDefault::getInstance()->getIntegerForKey("ai_losses", 0);
        cocos2d::UserDefault::getInstance()->setIntegerForKey("ai_losses", lose + 1);
        cocos2d::UserDefault::getInstance()->flush();
    }

    void addDraw() {
        int draw = cocos2d::UserDefault::getInstance()->getIntegerForKey("ai_draws", 0);
        cocos2d::UserDefault::getInstance()->setIntegerForKey("ai_draws", draw + 1);
        cocos2d::UserDefault::getInstance()->flush();
    }

    int getWins() { return cocos2d::UserDefault::getInstance()->getIntegerForKey("ai_wins", 0); }
    int getLoses() { return cocos2d::UserDefault::getInstance()->getIntegerForKey("ai_losses", 0); }
    int getDraws() { return cocos2d::UserDefault::getInstance()->getIntegerForKey("ai_draws", 0); }

    void resetStats() {
        auto def = cocos2d::UserDefault::getInstance();
        def->setIntegerForKey("ai_wins", 0);
        def->setIntegerForKey("ai_losses", 0);
        def->setIntegerForKey("ai_draws", 0);
        def->flush();
    }

private:
    StatsManager() {}
};

#endif