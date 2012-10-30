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

class Bubble:public CCObject{
    
public:
    int bubbleID;
    int row = -1;
    int col = -1;
    int radius = 15;
    
    bool active = false;
    
    float x;
    float y;
    float vx = 0;
    float vy = 0;
    
public:
    Bubble(int id);
    CCRect rect();
    Bubble * clone();
};

#endif /* defined(__Bobble__Bubble__) */
