#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Bubble.h"

USING_NS_CC;

class BobbleHero : public cocos2d::CCLayer{
    
    //for test
    const static int BUBBLE_ROW = 15;
    const static int BUBBLE_COL = 9;
    
    const static int BUBBLE_LEFT_EAGE = 0;
    const static int BUBBLE_RIGHT_EAGE = 270;
    const static int BUBBLE_TOP_EAGE = 480;
    const static int BUBBLE_BOTTOM_EAGE = 0;
    
    Bubble * currentBubble = NULL;
    Bubble * nextBubble = NULL;
    
    
    float shootSpeed = 500; //pix/s
    float dropSpeed = 2;
    float bubbleDropDistance = 0;
    bool shootAble = false;
    bool inPositionFix = false;
    
    int initRow = 5;
    
    CCArray *bubblesData;
    CCArray *bubblesDeleted = NULL;
    CCArray *bubblesChecked = NULL;
    CCArray *bubblesConnected = NULL;
    
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    void initBubble();
    void initShooter();
    void shootBubble(float angle);
    
    Bubble * getCollidedBubble();
    bool isAvailablePostion(int row, int col);
    CCPoint * getDropPos(Bubble * current, Bubble * collision);
    void acceptBubble(CCPoint * dropPos);
    void startCollosionMotion(Bubble * bubble);
    void deleteBubbles(CCArray * bubbles);
    
    void checkDeleteBubbles(Bubble * bubble);
    void getConnectedBubble(Bubble * bubble);
    
    CCArray * getFloatBubbles();
    CCArray * getNeighbors(Bubble * bubble);
    
    Bubble * getBubble(int row, int col);
    
    void addBubble(Bubble * bubble);
    void removeBubble(Bubble * bubble);
    void updateBubblePostion(float dt);
    
    bool checkGameOver();
    
    //utils
    CCPoint * getBubblePostion(Bubble * bubble);
    
public:
    CCSpriteBatchNode *bubbleBatch;

    virtual bool init();
    static cocos2d::CCScene* scene();
    virtual void update(float dt);
    //virtual void draw();
    CREATE_FUNC(BobbleHero);
};

#endif // __HELLOWORLD_SCENE_H__
