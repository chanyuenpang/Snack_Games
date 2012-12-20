//
//  GameBoard.cpp
//  Bobble
//
//  Created by VIP on 11/21/12.
//
//

#include "GameBoard.h"
#include "SimpleAudioEngine.h"
#include "Bubble.h"
#include "CCDrawingPrimitives.h"
#include "Const.h"
#include "Geom.h"

using namespace cocos2d;
using namespace CocosDenshion;

GameBoard::GameBoard(IGameBoardListener * inListener){
    CCSprite::CCSprite();
    
    listener = inListener;
    this->init();
    initBubble();
    initShooter();
    this->setAnchorPoint(CCPointMake(0, 0));
    
    CCTexture2D *pTexture2D  = CCTextureCache::sharedTextureCache()->addImage("sight.png");
    sightsBatch = CCSpriteBatchNode::createWithTexture(pTexture2D);
    this->addChild(sightsBatch);
}

bool GameBoard::init(){
    if (CCSprite::init()) {
        this->scheduleUpdate();
    }
    return true;
}

void GameBoard::onEnter(){
    CCSprite::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameBoard::initBubble(){
    bubblesData = new CCArray();
    Bubble * bubble;
    int id;
    bubbleDropDistance = initRow * Bubble::RADIUS * 2;
    
    CCTexture2D *pTexture2D  = CCTextureCache::sharedTextureCache()->addImage("bubbles.png");
    bubbleBatch = CCSpriteBatchNode::createWithTexture(pTexture2D);
    this->addChild(bubbleBatch);
    
    for (int i=0; i<BUBBLE_ROW; i++) {
        for (int j=0; j<BUBBLE_COL; j++) {
            id = int (CCRANDOM_0_1() * 6);
            if (id != 0) {
                bubble = new Bubble(id, bubbleBatch->getTexture());
                bubble->row = i;
                bubble->col = j;
                addBubble(bubble);
            }
        }
    }
}

void GameBoard::initShooter(){
    CCLog("initShooter");
    if (nextBubble == NULL) {
        currentBubble = new Bubble(int (CCRANDOM_0_1() * 5+1), bubbleBatch->getTexture());
        nextBubble = new Bubble(int (CCRANDOM_0_1() * 5+1), bubbleBatch->getTexture());
        this->addChild(currentBubble);
        this->addChild(nextBubble);
    }else{
        currentBubble = nextBubble;
        nextBubble = new Bubble(int (CCRANDOM_0_1() * 5+1), bubbleBatch->getTexture());
        this->addChild(nextBubble);
    }
    
    currentBubble->x = BOARD_WIDTH/2;
    currentBubble->y = currentBubble->radius;
    currentBubble->nextX = currentBubble->x;
    currentBubble->nextY = currentBubble->y;
    
    nextBubble->x = BOARD_WIDTH/2 - 4 * nextBubble->radius;
    nextBubble->y = currentBubble->radius;
    
    currentBubble->setPosition(CCPointMake(currentBubble->x, currentBubble->y));
    nextBubble->setPosition(CCPointMake(nextBubble->x, nextBubble->y));
    
    shootAble = true;
}

void GameBoard::shootBubble(float rad){
    if (shootAble) {
        shootAble = false;
        rad = getPerfectAngel(rad);
        currentBubble->vx = shootSpeed * sin(rad);
        currentBubble->vy = shootSpeed * cos(rad);
    }
}

float GameBoard::getPerfectAngel(float angle){
    
    currentBubble->vx = shootSpeed * sin(angle);
    currentBubble->vy = shootSpeed * cos(angle);
    
    CCPoint * target = getTargetPos();
    while (target == NULL) {
        target = getTargetPos();
    }

    // y = kx +b
    currentBubble->row = target->y;
    currentBubble->col = target->x;
    CCPoint * pa = this->getBubblePostion(currentBubble);
    float k = currentBubble->vy / currentBubble->vx;
    float b = pa->y - k * pa->x;
    
    CCPoint * pb = new CCPoint( -b/k , 0 );
    
    CCPoint * pc = new CCPoint(0,0);
    CCPoint * pd = new CCPoint(0, BOARD_HEIFHT);
    
    CCPoint * pe = new CCPoint(BOARD_WIDTH,0);
    CCPoint * pf = new CCPoint(BOARD_WIDTH, BOARD_HEIFHT);
    
    if(Geom::getTwoLineIntersection(pa, pb, pc, pd)){
        
    }else if(Geom::getTwoLineIntersection(pa, pb, pe, pf)){
        
    }else{
        angle = atan2f(pa->x-BOARD_WIDTH/2, pa->y - currentBubble->radius);
    }
    
    currentBubble->x = BOARD_WIDTH/2;
    currentBubble->y = currentBubble->radius;
    currentBubble->nextX = currentBubble->x;
    currentBubble->nextY = currentBubble->y;
    
    CCLog("%f,%f",target->x, target->y);
    return angle;
}

float GameBoard::getShootAngel(cocos2d::CCTouch *touch){
    CCPoint point = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    float rad = atan2f(point.x-currentBubble->x, point.y - currentBubble->y);
    float max = MAX_SHOOT_ANGLE * PI / 180;
    if (rad > max) {
        rad = max;
    }else if (rad < -max) {
        rad = -max;
    }
    
    return rad;
}

bool GameBoard::ccTouchBegan(CCTouch *touch, CCEvent *pEvent){
    if(shootAble)   drawSights(getShootAngel(touch));
    return  true;
}

void GameBoard::ccTouchMoved(CCTouch *touch, CCEvent *pEvent){
    if(shootAble)   drawSights(getShootAngel(touch));
}

void GameBoard::ccTouchEnded(CCTouch *touch, CCEvent *pEvent){
    sightsBatch->removeAllChildrenWithCleanup(true);
    shootBubble(getShootAngel(touch));
}

void GameBoard::update(float dt){
    
    //CCLOG("t:%f",dt);
    updateBubblePostion(dt);
    
//    if (shootAble == true) {
//        bubbleDropDistance += dropSpeed * dt;
//    }
    
    for (int i=0; i<CALCULATE_EVERY_FRAME; i++) {
        if(collisionCheck(dt/CALCULATE_EVERY_FRAME))    break;
    }
    
    if(checkGameOver()){
        this->unscheduleUpdate();
        shootAble = false;
        
        CCLabelTTF* pLabel = CCLabelTTF::create("GAME OVER!", "Arial", 24);
        pLabel->setPosition(CCPointMake(STAGE_WIDTH/2, STAGE_HEIGHT/2));
        this->addChild(pLabel, 1);
    }
}

CCPoint * GameBoard::getDropPostionAtTop(Bubble *bubble){
    CCPoint * pos = new CCPoint((int) ( (bubble->x + Bubble::RADIUS) / 2 / Bubble::RADIUS), 0);
    pos->x -= 1;
    if(getBubble(pos->y, pos->x) != NULL)   pos->x += 1;
    if(getBubble(pos->y, pos->x) != NULL)   pos->x -= 2;
    
    if(getBubble(pos->y, pos->x) != NULL){
        CCLog("ERROR AT TOP!");
    }
    
    return getBubble(pos->y, pos->x) != NULL ? NULL : pos;
}

CCPoint * GameBoard::getTargetPos(){
    float dt = 0.01;
    if (currentBubble != NULL) {
        
        currentBubble->nextX = currentBubble->x + dt * currentBubble->vx;
        currentBubble->nextY = currentBubble->y + dt * currentBubble->vy;
        
        Bubble * collidedbubble =  getCollidedBubble();
        if (collidedbubble == NULL) {
            if(currentBubble->nextX < BUBBLE_LEFT_EAGE){
                currentBubble->x = BUBBLE_LEFT_EAGE + currentBubble->radius;
                currentBubble->vx *= -1;
            }
            if(currentBubble->nextX > BUBBLE_RIGHT_EAGE){
                currentBubble->x = BUBBLE_RIGHT_EAGE - currentBubble->radius;
                currentBubble->vx *= -1;
            }
            
            float topEage = STAGE_HEIGHT + (Bubble::RADIUS + Bubble::RADIUS * (BUBBLE_ROW - 1) * 2) - bubbleDropDistance;
            if (currentBubble->nextY > topEage) {
                currentBubble->y = topEage + Bubble::RADIUS;
                return getDropPostionAtTop(currentBubble);
            }
            currentBubble->x = currentBubble->nextX;
            currentBubble->y = currentBubble->nextY;
        }else{
            CCPoint * dropPos = getDropPos(currentBubble, collidedbubble);
            //todo: revert x y
            float temp = dropPos->x;
            dropPos->x = dropPos->y;
            dropPos->y = temp;
            return dropPos;
        }
    }else{
        currentBubble->x += dt * currentBubble->vx;
        currentBubble->y += dt * currentBubble->vy;
    }

    return NULL;
}

bool GameBoard::collisionCheck(float dt){
    if (currentBubble != NULL && !shootAble && !inPositionFix) {
        
        currentBubble->nextX = currentBubble->x + dt * currentBubble->vx;
        currentBubble->nextY = currentBubble->y + dt * currentBubble->vy;
        
        Bubble * collidedbubble =  getCollidedBubble();
        if (collidedbubble == NULL) {
            if(currentBubble->nextX < BUBBLE_LEFT_EAGE){
                currentBubble->x = BUBBLE_LEFT_EAGE + currentBubble->radius;
                currentBubble->vx *= -1;
            }
            if(currentBubble->nextX > BUBBLE_RIGHT_EAGE){
                currentBubble->x = BUBBLE_RIGHT_EAGE - currentBubble->radius;
                currentBubble->vx *= -1;
            }
            
            float topEage = STAGE_HEIGHT + (Bubble::RADIUS + Bubble::RADIUS * (BUBBLE_ROW - 1) * 2) - bubbleDropDistance;
            if (currentBubble->nextY > topEage) {
                
                currentBubble->y = topEage + Bubble::RADIUS;
                CCPoint * pos = getDropPostionAtTop(currentBubble);
                acceptBubble(pos);
                return true;
            }
            currentBubble->x = currentBubble->nextX;
            currentBubble->y = currentBubble->nextY;
        }else{
            CCPoint * dropPos = getDropPos(currentBubble, collidedbubble);
            CCLOG("P:%f", dropPos->y);
            CCLOG("PP:%f", dropPos->x);
            //todo: revert x y
            float temp = dropPos->x;
            dropPos->x = dropPos->y;
            dropPos->y = temp;
            acceptBubble(dropPos);
            return true;
            CCLOG("bubbleCollisionCheck");
        }
    }else if(inPositionFix){
        float offsetX = currentBubble->x - currentBubble->nextX;
        float offsetY = currentBubble->y - currentBubble->nextY;
        float dis = sqrtf(offsetX * offsetX + offsetY * offsetY);
        
        if (dis < Bubble::RADIUS / 3) {
            currentBubble->x = currentBubble->nextX;
            currentBubble->y = currentBubble->nextY;
            
            this->removeChild(currentBubble, true);
            addBubble(currentBubble);
            
            startCollosionMotion(currentBubble);
            
            if(bubblesDeleted == NULL){
                bubblesDeleted = new CCArray();
            }
            
            if(bubblesChecked == NULL){
                bubblesChecked = new CCArray();
            }
            
            bubblesDeleted->removeAllObjects();
            bubblesChecked->removeAllObjects();
            checkDeleteBubbles(currentBubble);
            
            bool bubbleDeleted = false;
            int count = bubblesDeleted->count();
            if (count >= 3) {
                deleteBubbles(bubblesDeleted);
                CCArray * deletes = getFloatBubbles();
                deleteBubbles(deletes);
                bubbleDeleted = true;
                listener->onBubbleDeleted(count);
            }
            
            listener->onShootCompleted(bubbleDeleted);
            currentBubble = NULL;
            inPositionFix = false;
            initShooter();
            
            return true;
        }else{
            currentBubble->x += dt * currentBubble->vx;
            currentBubble->y += dt * currentBubble->vy;
        }
    }
    return false;
}

void GameBoard::updateBubblePostion(float dt){
    Bubble * bubble;
    CCPoint * point;
    for (int i=0; i<bubblesData->count(); i++) {
        bubble = (Bubble*) bubblesData->objectAtIndex(bubblesData->count() - 1 - i);
        point = getBubblePostion(bubble);
        if (!bubble->active) {
            bubble->x = point->x;
            bubble->y = point->y;
            bubble->setPosition(CCPointMake(bubble->x, bubble->y));
        }else{
            bubble->update(dt);
        }
    }
}

CCPoint * GameBoard::getBubblePostion(Bubble * bubble){
    float targetX;
    float targetY;
    
    if (bubble->row % 2 == 0) {
        targetX = bubble->radius + bubble->radius * bubble->col * 2;
    }else{
        targetX = bubble->radius + bubble->radius * (2 * bubble->col + 1);
    }
    targetY = (int) (STAGE_HEIGHT + (bubble->radius + bubble->radius * (BUBBLE_ROW - 1 - bubble->row) * 2) - bubbleDropDistance);
    
    return new CCPoint(targetX, targetY);
}

void GameBoard::addBubble(Bubble *bubble){
    bubble->setPosition(CCPointMake(bubble->x, bubble->y));
    bubblesData->addObject(bubble);
    bubbleBatch->addChild(bubble);
    
    CCPoint * point = getBubblePostion(bubble);
    bubble->x = point->x;
    bubble->y = point->y;
    bubble->setPosition(CCPointMake(bubble->x, bubble->y));
}

void GameBoard::removeBubble(Bubble *bubble){
    bubblesData->removeObject(bubble);
    bubbleBatch->removeChild(bubble, true);
}


Bubble * GameBoard::getCollidedBubble(){
    Bubble *bubble;
    float distance;
    float offsetX;
    float offsetY;
    for (int i=0; i<bubblesData->count(); i++) {
        bubble = (Bubble*) bubblesData->objectAtIndex(i);
        offsetX = currentBubble->nextX - bubble->x;
        offsetY = currentBubble->nextY - bubble->y;
        distance = sqrtf( offsetX * offsetX + offsetY * offsetY );
        //CCLOG("bubbleCollisionCheck%d", distance);
        if (distance <= bubble->radius + currentBubble->radius * COLLISION_SENSITIVITY) {
            return bubble;
        }
    }
    return NULL;
}

bool GameBoard::isAvailablePostion(int row, int col){
    if(col < 0 || col >= BUBBLE_COL || row < 0) return false;
    Bubble * temp = getBubble(row, col);
    if (temp == NULL) {
        return true;
    }else{
        return false;
    }
}

CCPoint * GameBoard::getDropPos(Bubble *current, Bubble *collision){
    float offsetX = current->x - collision->x;
    //float offsetY = current->y - collision->y;
    if (collision->row % 2 == 0) {
        
        if (collision->col == 0) {
            if (offsetX >= Bubble::RADIUS) {
                if (isAvailablePostion(collision->row, collision->col+1)) {
                    return new CCPoint(collision->row, collision->col+1);
                }else{
                    if (isAvailablePostion(collision->row+1, collision->col)) {
                        return new CCPoint(collision->row+1, collision->col);
                    }
                }
            }
            
            if (offsetX <= Bubble::RADIUS) {
                if (isAvailablePostion(collision->row + 1, collision->col)) {
                    return new CCPoint(collision->row + 1, collision->col);
                }else{
                    if (isAvailablePostion(collision->row, collision->col+1)) {
                        return new CCPoint(collision->row, collision->col+1);
                    }
                }
            }
        }
        
        if (offsetX >= Bubble::RADIUS) {
            if (isAvailablePostion(collision->row, collision->col+1)) {
                return new CCPoint(collision->row, collision->col+1);
            }else{
                if (isAvailablePostion(collision->row+1, collision->col)) {
                    return new CCPoint(collision->row+1, collision->col);
                }
            }
        }
        
        if (offsetX >= 0 && offsetX <= Bubble::RADIUS) {
            if (isAvailablePostion(collision->row + 1, collision->col)) {
                return new CCPoint(collision->row + 1, collision->col);
            }else{
                if (isAvailablePostion(collision->row, collision->col+1)) {
                    return new CCPoint(collision->row, collision->col+1);
                }
            }
        }
        
        if (offsetX <= -Bubble::RADIUS) {
            if (isAvailablePostion(collision->row, collision->col-1)) {
                return new CCPoint(collision->row, collision->col-1);
            }else{
                if (isAvailablePostion(collision->row+1, collision->col-1)) {
                    return new CCPoint(collision->row+1, collision->col-1);
                }
            }
        }
        
        if (offsetX <= 0 && offsetX >= -Bubble::RADIUS) {
            if (isAvailablePostion(collision->row + 1, collision->col-1)) {
                return new CCPoint(collision->row + 1, collision->col-1);
            }else{
                if (isAvailablePostion(collision->row, collision->col-1)) {
                    return new CCPoint(collision->row, collision->col-1);
                }
            }
        }
        
    }else{
        if (offsetX >= Bubble::RADIUS) {
            if (isAvailablePostion(collision->row, collision->col+1)) {
                return new CCPoint(collision->row, collision->col+1);
            }else{
                if (isAvailablePostion(collision->row+1, collision->col+1)) {
                    return new CCPoint(collision->row+1, collision->col+1);
                }
            }
        }
        
        if (offsetX >= 0 && offsetX <= Bubble::RADIUS) {
            if (isAvailablePostion(collision->row + 1, collision->col+1)) {
                return new CCPoint(collision->row + 1, collision->col+1);
            }else{
                if (isAvailablePostion(collision->row, collision->col+1)) {
                    return new CCPoint(collision->row, collision->col+1);
                }
            }
        }
        
        if (offsetX <= -Bubble::RADIUS) {
            if (isAvailablePostion(collision->row, collision->col-1)) {
                return new CCPoint(collision->row, collision->col-1);
            }else{
                if (isAvailablePostion(collision->row+1, collision->col)) {
                    return new CCPoint(collision->row+1, collision->col);
                }
            }
        }
        
        if (offsetX <= 0 && offsetX >= -Bubble::RADIUS) {
            if (isAvailablePostion(collision->row + 1, collision->col)) {
                return new CCPoint(collision->row + 1, collision->col);
            }else{
                if (isAvailablePostion(collision->row, collision->col-1)) {
                    return new CCPoint(collision->row, collision->col-1);
                }
            }
        }
    }
    
    CCLOG("getDropBubblePos ERROR!!!");
    return new CCPoint(-1, -1);
}

void GameBoard::startCollosionMotion(Bubble *bubble){
    if (bubblesConnected != NULL && bubblesConnected->count() > 0) {
        bubblesConnected->removeAllObjects();
    }else{
        bubblesConnected = new CCArray();
    }
    
    getConnectedBubble(bubble);
    Bubble *temp;
    CCPoint *point;
    for (int i = 0; i<bubblesConnected->count(); i++) {
        point =  (CCPoint *) bubblesConnected->objectAtIndex(i);
        temp = getBubble(point->y, point->x);
        temp->collosionMotion(bubble);
    }
    
    bubble->collosionMotion(bubble);
}

void GameBoard::acceptBubble(CCPoint * dropPos){
    CCLOG("Accept!");
    //bubbleDropDistance -= 1;
    Bubble * bubble = getBubble(dropPos->y, dropPos->x);
    CCLOG("P:%f", dropPos->y);
    CCLOG("PP:%f", dropPos->x);
    if(bubble == NULL){
        currentBubble->row = dropPos->y;
        currentBubble->col = dropPos->x;
        CCPoint * point = getBubblePostion(currentBubble);
        currentBubble->nextX = point->x;
        currentBubble->nextY = point->y;
        
        //float rad = atan2f(point->x-currentBubble->x, point->y - currentBubble->y);
        currentBubble->oldVx = currentBubble->vx;
        currentBubble->oldVy = currentBubble->vy;
        currentBubble->vx = (point->x-currentBubble->x) / 0.05;  // shootSpeed * sin(rad);
        currentBubble->vy = (point->y - currentBubble->y)/ 0.05; //shootSpeed * cos(rad);
        inPositionFix = true;
    }else{
        CCLOG("ERROR!!!");
    }
}

void GameBoard::deleteBubbles(CCArray * bubbles){
    Bubble * tempBubble;
    for(int i = 0; i < bubbles->count(); i++){
        CCPoint * point = (CCPoint *) bubbles->objectAtIndex(i);
        tempBubble = getBubble(point->y, point->x);
        removeBubble(tempBubble);
    }
}

void GameBoard::checkDeleteBubbles(Bubble * bubble){
    CCArray * neighbors = getNeighbors(bubble);
    neighbors->retain();
    Bubble * tempBubble;
    bool hasChecked = false;
    CCPoint *point;
    CCPoint *checkedPoint;
    for(int i = 0; i < neighbors->count(); i++){
        point = (CCPoint *) neighbors->objectAtIndex(i);
        //CCLOG("P:%i", (int) point->y);
        //CCLOG("PP:%i", (int) point->x);
        tempBubble = getBubble(point->y, point->x);
        if (tempBubble == NULL) {
            continue;
        }
        tempBubble->retain();
        
        for (int j = 0; j < bubblesChecked->count(); j++) {
            checkedPoint = (CCPoint *) bubblesChecked->objectAtIndex(j);
            //            CCLOG("XX:%i", (int) checkedPoint->x);
            //            CCLOG("YX:%i", (int) checkedPoint->y);
            //            CCLOG("XXX:%i", (int) tempBubble->col);
            //            CCLOG("YXX:%i", (int) tempBubble->row);
            if (checkedPoint->x == tempBubble->col && checkedPoint->y == tempBubble->row) {
                hasChecked = true;
                break;
            }
        }
        
        if (hasChecked) {
            hasChecked = false;
            continue;
        }else{
            bubblesChecked->addObject(point);
        }
        
        //        CCLOG("ID:%i", (int) tempBubble->bubbleID);
        //        CCLOG("X:%i", (int) point->x);
        //        CCLOG("Y:%i", (int) point->y);
        
        if(bubble->bubbleID == tempBubble->bubbleID){
            bubblesDeleted->addObject(point);
            checkDeleteBubbles(tempBubble);
        }
    }
    return ;
}

CCArray *  GameBoard::getFloatBubbles(){
    if (bubblesConnected != NULL && bubblesConnected->count() > 0) {
        bubblesConnected->removeAllObjects();
    }else{
        bubblesConnected = new CCArray();
    }
    
    CCLOG("CL:%i", bubblesConnected->count());
    
    Bubble * bubble;
    for (int i=0; i<BUBBLE_COL; i++) {
        bubble = getBubble(0, i);
        if(bubble != NULL){
            bubblesConnected->addObject(new CCPoint(bubble->col, bubble->row));
        }
    }
    
    for (int i=0; i<BUBBLE_COL; i++) {
        bubble = getBubble(0, i);
        if(bubble != NULL){
            getConnectedBubble(bubble);
        }
    }
    
    CCLOG("CL:%i", bubblesConnected->count());
    
    if(bubblesConnected != NULL){
        CCPoint *p;
        CCLog("(==Connected==)");
        for (int i=0; i<bubblesConnected->count(); i++) {
            p =  (CCPoint *) bubblesConnected->objectAtIndex(i);
            //CCLog("(%d,%d)", (int) p->y, (int) p->x);
        }
        CCLog("(====)");
    }
    
    CCArray * out = CCArray::create();
    out->retain();
    Bubble *tempbubble;
    CCPoint * tempPoint;
    bool hasFLoat;
    for (int i=0; i<bubblesData->count(); i++) {
        
        tempbubble = (Bubble*) bubblesData->objectAtIndex(i);
        if (tempbubble->bubbleID == 0) continue;
        hasFLoat = true;
        for (int m = 0; m<bubblesConnected->count(); m++) {
            tempPoint = (CCPoint *) bubblesConnected->objectAtIndex(m);
            if (tempPoint->x == tempbubble->col && tempPoint->y == tempbubble->row) {
                hasFLoat = false;
                break;
            }
        }
        if (hasFLoat) {
            out->addObject(new CCPoint(tempbubble->col, tempbubble->row));
        }
    }
    CCPoint *p;
    CCLog("(==Out==)");
    for (int i=0; i<out->count(); i++) {
        p =  (CCPoint *) out->objectAtIndex(i);
        CCLog("(%d,%d)", (int) p->y, (int) p->x);
    }
    CCLog("(====)");
    
    return out;
}

//void GameBoard::getConnectedBubble(Bubble *bubble){
//    if (allNeighbors != NULL && allNeighbors->count() > 0) {
//        allNeighbors->removeAllObjects();
//    }else{
//        allNeighbors = new CCArray();
//    }
//    getConnectedBubbleRecFun(bubble);
//}


void GameBoard::getConnectedBubble(Bubble *bubble){
    CCArray * neighbors = getNeighbors(bubble);
    Bubble * tempBubble;
    bool hasChecked;
    CCPoint *point;
    CCPoint *checkedPoint;
    for(int i = 0; i < neighbors->count(); i++){
        hasChecked = false;
        point = (CCPoint *) neighbors->objectAtIndex(i);
        tempBubble = getBubble((int) point->y, (int) point->x);
        if (tempBubble == NULL)  continue;
        
        for (int j = 0; j < bubblesConnected->count(); j++) {
            checkedPoint = (CCPoint *) bubblesConnected->objectAtIndex(j);
            if (checkedPoint->x == tempBubble->col && checkedPoint->y == tempBubble->row) {
                //CCLOG("X:%i", (int) point->x);
                //CCLOG("Y:%i", (int) point->y);
                hasChecked = true;
                break;
            }
        }
        
        if (!hasChecked) {
//            CCLOG("X:%i", (int) point->x);
//            CCLOG("Y:%i", (int) point->y);
            bubblesConnected->addObject(point);
            getConnectedBubble(tempBubble);
        }
    }
}

CCArray * GameBoard::getNeighbors(Bubble * bubble){
    CCArray * outArray = CCArray::create();
    outArray->retain();
    //left
    if(bubble->col-1 >= 0)  outArray->addObject(new CCPoint(bubble->col-1, bubble->row));
    //right
    if(bubble->col+1 < BUBBLE_COL)  outArray->addObject(new CCPoint(bubble->col+1, bubble->row));
    
    if(bubble->row % 2 == 0){
        //topLeft
        if(bubble->col-1 >= 0 && bubble->row-1 >= 0)
            outArray->addObject(new CCPoint(bubble->col-1, bubble->row-1));
        //topRight
        if(bubble->row - 1 >= 0)
            outArray->addObject(new CCPoint(bubble->col, bubble->row-1));
        //bottomLeft
        if(bubble->col-1 >= 0)
            outArray->addObject(new CCPoint(bubble->col-1, bubble->row+1));
        //bottomRight
        outArray->addObject(new CCPoint(bubble->col, bubble->row+1));
    }else{
        //same like top
        if(bubble->row-1 >= 0)
            outArray->addObject(new CCPoint(bubble->col, bubble->row-1));
        if(bubble->col+1 < BUBBLE_COL && bubble->row - 1 >= 0)
            outArray->addObject(new CCPoint(bubble->col+1, bubble->row-1));
        outArray->addObject(new CCPoint(bubble->col, bubble->row+1));
        if(bubble->col+1 < BUBBLE_COL)
            outArray->addObject(new CCPoint(bubble->col+1, bubble->row+1));
    }
    
    return outArray;
}

Bubble * GameBoard::getBubble(int row, int col){
    Bubble *bubble;
    for (int i=0; i<bubblesData->count(); i++) {
        bubble = (Bubble*) bubblesData->objectAtIndex(i);
        
        if (bubble->row == row && bubble->col == col) {
            return bubble;
        }
    }
    return NULL;
}

void GameBoard::drawSights(float rad){
    CCArray * points = CCArray::create();
    float startX = currentBubble->x;
    float startY = currentBubble->y;
    points->addObject(new CCPoint(startX, startY));
    
    float step = 30;
    float dx = step * sin(rad);
    float dy = step * cos(rad);
    
    CCPoint * p;
    bool available;
    do {
        p = new CCPoint(startX + dx, startY + dy);
        if (p->x >= BUBBLE_RIGHT_EAGE) {
            dx *= -1;
            p->x = 2*BUBBLE_RIGHT_EAGE-p->x;
            if(p->x+Bubble::RADIUS >= BUBBLE_RIGHT_EAGE) p->x = BUBBLE_RIGHT_EAGE-Bubble::RADIUS;
        }
        if (p->x <= BUBBLE_LEFT_EAGE) {
            dx *= -1;
            p->x = BUBBLE_LEFT_EAGE - p->x;
            if(p->x+Bubble::RADIUS >= BUBBLE_LEFT_EAGE) p->x = BUBBLE_LEFT_EAGE + Bubble::RADIUS;
        }
        
        startX = p->x;
        startY = p->y;
        available = true;
        Bubble * bubble;
        CCPoint bubblePos;
        float dis;
        
        if (p->y >= BUBBLE_TOP_EAGE) {
            p->y = BUBBLE_TOP_EAGE - Bubble::RADIUS;
            available = false;
        }
        
        if(available){
            for (int i=0; i<bubblesData->count(); i++) {
                bubble = (Bubble*) bubblesData->objectAtIndex(bubblesData->count() - 1 - i);
                bubblePos = bubble->getPosition();
                dis = sqrtf(powf(bubblePos.x - p->x, 2)+powf(bubblePos.y - p->y,2));
                if(dis <= Bubble::RADIUS){
                    available = false;
                    break;
                }
            }
        }
        
        if(available)   points->addObject(p);
        
    } while (available);
    
    CCPoint *pp;
    CCSprite *sp;
    CCTexture2D *pTexture2D  = sightsBatch->getTexture();
    sightsBatch->removeAllChildrenWithCleanup(true);
    for (int i=0; i<points->count(); i++) {
        pp = (CCPoint*) points->objectAtIndex(i);
        sp = CCSprite::createWithTexture(pTexture2D);
        sp->setScale(0.2);
        sp->setPosition(CCPointMake(pp->x, pp->y));
        sightsBatch->addChild(sp);
    }
}

bool GameBoard::checkGameOver(){
    Bubble * bubble;
    for (int i=0; i<bubblesData->count(); i++) {
        bubble = (Bubble*) bubblesData->objectAtIndex(bubblesData->count() - 1 - i);
        if (bubble != NULL && bubble->y <= 10 * Bubble::RADIUS) {
            return true;
        }
    }
    return false;
}

