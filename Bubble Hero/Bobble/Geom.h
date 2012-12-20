//
//  Geom.h
//  Bobble
//
//  Created by VIP on 12/9/12.
//
//

#ifndef __Bobble__Geom__
#define __Bobble__Geom__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Geom{
public:
    static CCPoint * getTwoLineIntersection(CCPoint* pa, CCPoint* pb, CCPoint* pc, CCPoint* pd);
};


#endif /* defined(__Bobble__Geom__) */
