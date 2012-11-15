//
//  BubbleFactory.h
//  Bobble
//
//  Created by VIP on 11/15/12.
//
//

#include "GameData.h"

GameData* GameData::_instance = NULL; 

GameData::GameData(){
    init();
}

GameData * GameData::getInstance(){
    if (!GameData::_instance) {
        GameData::_instance = new GameData();
        _instance->gameScore = 0;
    }
    return GameData::_instance;
}

int GameData::getBestScore(){
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("bestScore");
}

void GameData::updateBestScore(int score){
    CCUserDefault::sharedUserDefault()->setIntegerForKey("bestScore", score);
}


void GameData::init(){
    this->gameScore = 0;
    this->bestCombo = 0;
    this->bestLevel = 0;
}

