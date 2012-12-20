//
//  MainUI.cpp
//  Bobble
//
//  Created by VIP on 12/13/12.
//
//

#include "GameUI.h"
#include "Const.h"

bool GameUI::init(){
    if ( !CCLayer::init() )   return false;
    
    
    CCSprite * background = CCSprite::create("bg_pink_forest.jpg");
    background->setPosition(CCPointMake(STAGE_WIDTH/2, STAGE_HEIGHT/2));
    this->addChild(background);
    
    CCSprite * boardFrame = CCSprite::create("bg_board.png");
    boardFrame->setPosition(CCPointMake(STAGE_WIDTH/2, STAGE_HEIGHT/2 - 30));
    this->addChild(boardFrame);
    
    CCSprite * footBG = CCSprite::create("bg_bot.png");
    footBG->setPosition(CCPointMake(STAGE_WIDTH/2, footBG->boundingBox().size.height/2));
    this->addChild(footBG);
    
    CCSprite * footFrame = CCSprite::create("bg_under.png");
    footFrame->setPosition(CCPointMake(STAGE_WIDTH/2, footFrame->boundingBox().size.height/2 - 35));
    this->addChild(footFrame);
    
    CCSprite * topBar = CCSprite::create("bg_top.png");
    topBar->setPosition(CCPointMake(STAGE_WIDTH/2, STAGE_HEIGHT - topBar->boundingBox().size.height/2));
    this->addChild(topBar);
    
    CCSprite * inkFrame = CCSprite::create("outer_ink.png");
    inkFrame->setPosition(CCPointMake(STAGE_WIDTH/2, STAGE_HEIGHT/2));
    this->addChild(inkFrame);
    
    CCSprite * user = CCSprite::create("banner_me.png");
    user->setPosition(CCPointMake(95, STAGE_HEIGHT-user->boundingBox().size.height/2));
    this->addChild(user);
    
    CCSprite * enemy = CCSprite::create("banner_enemy.png");
    enemy->setPosition(CCPointMake(STAGE_WIDTH - 95, STAGE_HEIGHT-enemy->boundingBox().size.height/2));
    this->addChild(enemy);
    
    return true;

}