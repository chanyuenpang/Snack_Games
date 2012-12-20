#include "BobbleHero.h"
#include "SimpleAudioEngine.h"
#include "Bubble.h"
#include "CCDrawingPrimitives.h"
#include "Const.h"

#include "SpriteTest.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* BobbleHero::scene(){
    
    CCScene *scene = CCScene::create();
    BobbleHero *layer = BobbleHero::create();
    scene->addChild(layer);
    return scene;
}

bool BobbleHero::init(){
    if ( !CCLayer::init() )   return false;
    
    ui = GameUI::create();
    this->addChild(ui);
    
    gameBoard = new GameBoard(this);
    gameBoard->setPosition(CCPointMake(210, 70));
    this->addChild(gameBoard);
    
    score = 0;
    combo = 0;
    
    scoreText = CCLabelTTF::create("Score:0", "Arial", 24);
    comboText = CCLabelTTF::create("Combo:0", "Arial", 24);
    
    scoreText->setPosition(CCPointMake(800, 20));
    comboText->setPosition(CCPointMake(800, 60));
    
    this->addChild(scoreText);
    this->addChild(comboText);
    
    return true;
}

void BobbleHero::onShootCompleted(bool bubbleDeleted){
    if(bubbleDeleted){
        combo ++;
    }else{
        combo = 0;
    }
    
    char comboStr[100];
    sprintf(comboStr, "Combo:%i", combo);
    comboText->setString(comboStr);
}

void BobbleHero::onBubbleDeleted(int num){
    score += num;
    char str[100];
    sprintf(str, "Score:%i", score);
    scoreText->setString(str);
}


