//
//  Bubble.cpp
//  Bobble
//
//  Created by VIP on 10/14/12.
//
//

#include "Bubble.h"

Bubble::Bubble(int id){
    this->bubbleID = id;
}

CCRect Bubble::rect(){
    return CCRectMake(x, y, 2*radius, 2*radius);
}