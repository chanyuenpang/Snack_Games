//
//  MainUI.h
//  Bobble
//
//  Created by VIP on 12/13/12.
//
//

#ifndef __Bobble__MainUI__
#define __Bobble__MainUI__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class GameUI:public CCLayer{
public:
    virtual bool init();
    CREATE_FUNC(GameUI);
};
#endif /* defined(__Bobble__MainUI__) */
