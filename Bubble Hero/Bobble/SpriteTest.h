

#ifndef __Bobble__SpriteTest__
#define __Bobble__SpriteTest__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class SpriteTest:public CCSprite{
    
    
public:
    SpriteTest();
    void onEnter();
    bool init();
    void update(float dt);
    //virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    //bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    
};

#endif /* defined(__Bobble__SpriteTest__) */
