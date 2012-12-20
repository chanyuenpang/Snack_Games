#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Bubble.h"
#include "GameUI.h"
#include "GameBoard.h"
#include "IGameBoardListener.h"

USING_NS_CC;

class BobbleHero : public cocos2d::CCLayer, public IGameBoardListener{
    
    int score;
    int combo;
    
    CCLabelTTF * scoreText;
    CCLabelTTF * comboText;
    
    GameUI * ui;
    GameBoard * gameBoard;
    
public:
    
    void onShootCompleted(bool bubbleDeleted);
    void onBubbleDeleted(int num);
        
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(BobbleHero);
};

#endif // __HELLOWORLD_SCENE_H__
