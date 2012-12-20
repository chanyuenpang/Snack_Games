//
//  BubbleFactory.h
//  Bobble
//
//  Created by VIP on 11/15/12.
//
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "cocos2d.h"
#include "string.h"

using namespace cocos2d;

struct LevelData{
    int rows;
    int cols;
    int displayRows;
    CCArray data;
};

class GameData{

public:
    static GameData * _instance;
    int gameScore;
    int bestLevel;
    int bestCombo;
    
public:
    static GameData* getInstance();
    LevelData getLevelData(int level);
    int getBestScore();
    void updateBestScore(int score);
    void init();
    
private:
    GameData();
    ~GameData();
};

#endif