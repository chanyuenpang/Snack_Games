//
//  GameBoard.h
//  Bobble
//
//  Created by VIP on 11/21/12.
//
//

#ifndef __Bobble__GameBoard__
#define __Bobble__GameBoard__

#include <iostream>
#include "cocos2d.h"  
#include "Bubble.h"
#include "IGameBoardListener.h"

USING_NS_CC;

class GameBoard:public CCSprite, public CCTargetedTouchDelegate{
    
    //for test
    const static int BUBBLE_ROW = 5;
    const static int BUBBLE_COL = 14;
    
    const static int BOARD_WIDTH = 609;
    const static int BOARD_HEIFHT = 768;
    
    const static int BUBBLE_LEFT_EAGE = 0;
    const static int BUBBLE_RIGHT_EAGE = 609;
    const static int BUBBLE_TOP_EAGE = 768;
    const static int BUBBLE_BOTTOM_EAGE = 0;
    
    IGameBoardListener * listener;
    
    Bubble * currentBubble = NULL;
    Bubble * nextBubble = NULL;
    
    float shootSpeed = 1800; //pix/s
    float dropSpeed = 2;
    float bubbleDropDistance = 0;
    bool shootAble = false;
    bool inPositionFix = false;
    
    int initRow = 5;
    
    CCArray *bubblesData;
    CCArray *bubblesDeleted = NULL;
    CCArray *bubblesChecked = NULL;
    CCArray *bubblesConnected = NULL;
//    CCArray *allNeighbors = NULL;
    
    void initBubble();
    void initShooter();
    float getShootAngel(CCTouch * touch);
    float getPerfectAngel(float angle);
    CCPoint * getTargetPos();
    void shootBubble(float angle);
    
    Bubble * getCollidedBubble();
    bool collisionCheck(float dt);
    bool isAvailablePostion(int row, int col);
    void acceptBubble(CCPoint * dropPos);
    void startCollosionMotion(Bubble * bubble);
    void deleteBubbles(CCArray * bubbles);
    
    void checkDeleteBubbles(Bubble * bubble);
    void getConnectedBubble(Bubble * bubble);
    void getConnectedBubbleRecFun(Bubble * bubble);
    
    CCArray * getFloatBubbles();
    CCArray * getNeighbors(Bubble * bubble);
    
    Bubble * getBubble(int row, int col);
    
    void addBubble(Bubble * bubble);
    void removeBubble(Bubble * bubble);
    void updateBubblePostion(float dt);
    
    bool checkGameOver();
    
    void drawSights(float rad);
    
    CCPoint * getDropPos(Bubble * current, Bubble * collision);
    CCPoint * getDropPostionAtTop(Bubble * bubble);
    CCPoint * getBubblePostion(Bubble * bubble);
    
public:
    GameBoard(IGameBoardListener * inListener);
    CCSpriteBatchNode *bubbleBatch;
    CCSpriteBatchNode *sightsBatch;
    void onEnter();
    bool init();
    void update(float dt);
    //virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);


};

#endif /* defined(__Bobble__GameBoard__) */
