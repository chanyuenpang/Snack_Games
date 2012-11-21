//
//  Bubble.h
//  Bobble
//
//  Created by VIP on 10/14/12.
//
//

#ifndef __Bobble__Bubble__
#define __Bobble__Bubble__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Bubble:public CCSprite{
    
public:
    const static int RADIUS = 21;
    int bubbleID;
    int row = -1;
    int col = -1;
    int radius = RADIUS;
    
    bool active = false;
    
    float x;
    float y;
    float nextX;
    float nextY;
    float vx = 0;
    float vy = 0;
    float ax = 0;
    float ay = 0;
    
    Bubble(int id, CCTexture2D *texture);
    void collosionMotion(Bubble * current);
    void motionComplete(Bubble * bubble);
    
    virtual void draw();
    virtual void update(float dt);
    
//private:
    bool isRevert;
    float updateTime = 0;
    float oldX = 0;
    float oldY = 0;
    
    float oldVx = 0;
    float oldVy = 0;
    
};

#endif /* defined(__Bobble__Bubble__) */
