
#include "SpriteTest.h"


SpriteTest::SpriteTest(){
    this->init();
    this->initWithFile("Default.png");
    this->setAnchorPoint(CCPointMake(0, 0));
    
}

bool SpriteTest::init(){
    if (CCSprite::init()) {
        //this->scheduleUpdate();
    }
    return true;
}

void SpriteTest::onEnter(){
    //this->scheduleUpdate();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void SpriteTest::update(float dt){
    CCLOG("AAAAA");
}