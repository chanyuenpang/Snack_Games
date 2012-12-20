//
//  BubbleFactory.h
//  Bobble
//
//  Created by VIP on 11/15/12.
//
//

#ifndef __Bobble__BubbleFactory__
#define __Bobble__BubbleFactory__

#include <iostream>
#include "Bubble.h"

class BubbleFactory{
public:
    static BubbleFactory * _instance;
    static BubbleFactory* getInstance();
    
    Bubble * getRandomBubble();
};

#endif /* defined(__Bobble__BubbleFactory__) */
