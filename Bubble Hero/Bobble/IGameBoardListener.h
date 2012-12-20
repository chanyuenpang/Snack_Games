//
//  IGameBoardListener.h
//  Bobble
//
//  Created by VIP on 12/13/12.
//
//

#ifndef __Bobble__IGameBoardListener__
#define __Bobble__IGameBoardListener__

#include <iostream>

class IGameBoardListener{
public:
    virtual void onShootCompleted(bool bubbleDeleted) = 0;
    virtual void onBubbleDeleted(int num) = 0;
};

#endif /* defined(__Bobble__IGameBoardListener__) */
