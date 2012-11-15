//
//  BubbleFactory.cpp
//  Bobble
//
//  Created by VIP on 10/15/12.
//
//

#include "BubbleFactory.h"

BubbleFactory* BubbleFactory::_instance = NULL;

BubbleFactory * BubbleFactory::getInstance(){
    if (!BubbleFactory::_instance) {
        BubbleFactory::_instance = new BubbleFactory();
    }
    
    return BubbleFactory::_instance;
}