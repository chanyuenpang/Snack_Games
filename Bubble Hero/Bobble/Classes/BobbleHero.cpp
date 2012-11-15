#include "BobbleHero.h"
#include "SimpleAudioEngine.h"
#include "Bubble.h"
#include "CCDrawingPrimitives.h"
#include "Const.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* BobbleHero::scene(){
    
    CCScene *scene = CCScene::create();
    BobbleHero *layer = BobbleHero::create();
    scene->addChild(layer);
    return scene;
}

bool BobbleHero::init(){
    if ( !CCLayer::init() )   return false;

    initBubble();
    initShooter();
    
    this->scheduleUpdate();
    setTouchEnabled(true);
    return true;
}

void BobbleHero::initBubble(){
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
//                if (i % 2 == 0) {
//                    bubble->x = bubble->radius + bubble->radius * j * 2;
//                }else{
//                    bubble->x = bubble->radius + bubble->radius * (2 * j + 1);
//                }
//                
//                bubble->y = STAGE_HEIGHT + (bubble->radius + bubble->radius * (BUBBLE_ROW - 1 - i) * sqrtf(3.0)) - bubbleDropDistance;
                addBubble(bubble);
            }
        }
    }
}

void BobbleHero::initShooter(){
    CCLog("initShooter");
    if (nextBubble == NULL) {
        currentBubble = new Bubble(int (CCRANDOM_0_1() * 5+1), bubbleBatch->getTexture());
        nextBubble = new Bubble(int (CCRANDOM_0_1() * 5+1), bubbleBatch->getTexture());
        this->addChild(currentBubble);
        this->addChild(nextBubble);
    }else{
//        this->removeChild(currentBubble,true);
//        this->removeChild(nextBubble,true);
        //this->addChild(nextBubble);
        currentBubble = nextBubble;
        nextBubble = new Bubble(int (CCRANDOM_0_1() * 5+1), bubbleBatch->getTexture());
        this->addChild(nextBubble);
    }
    
    nextBubble->x = STAGE_WIDTH/2 - 4 * nextBubble->radius;
    nextBubble->y = currentBubble->radius;
    
    currentBubble->x = STAGE_WIDTH/2;
    currentBubble->y = currentBubble->radius;
    currentBubble->nextX = currentBubble->x;
    currentBubble->nextY = currentBubble->y;
    
    //this->addChild(currentBubble);
    
    shootAble = true;
}

void BobbleHero::shootBubble(float rad){
    if (shootAble) {
        shootAble = false;
        //currentBubble->active = true;
        currentBubble->vx = shootSpeed * sin(rad);
        currentBubble->vy = shootSpeed * cos(rad);
    }
}

void BobbleHero::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint point = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    
    float rad = atan2f(point.x-currentBubble->x, point.y - currentBubble->y);
    float max = MAX_SHOOT_ANGLE * PI / 180;
    if (rad > max) {
        rad = max;
    }else if (rad < -max) {
        rad = -max;
    }
    
    shootBubble(rad);
}

//void BobbleHero::draw(){
//    CCLayer::draw();
//    
//    Bubble *bubble;
//    for (int i=0; i<bubblesData->count(); i++) {
//        bubble = (Bubble*) bubblesData->objectAtIndex(i);
//        bubble->draw();
//    }
//    
//    currentBubble->draw();
//    nextBubble->draw();
//}

void BobbleHero::update(float dt){
    updateBubblePostion(dt);

    if (shootAble == true) {
        bubbleDropDistance += dropSpeed * dt;
    }
    
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
                currentBubble->y = topEage;
                //need test
                acceptBubble(new CCPoint((int) ( (currentBubble->x + Bubble::RADIUS) / 2 / Bubble::RADIUS), 0));
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
            CCLOG("bubbleCollisionCheck");
        }
    }else if(inPositionFix){        
        float offsetX = currentBubble->x - currentBubble->nextX;
        float offsetY = currentBubble->y - currentBubble->nextY;
        float dis = sqrtf(offsetX * offsetX + offsetY * offsetY);
        
        if (dis < Bubble::RADIUS / 3) {
            currentBubble->x = currentBubble->nextX;
            currentBubble->y = currentBubble->nextY;
//            Bubble * bubble = new Bubble(currentBubble->bubbleID, bubbleBatch->getTexture());
//            bubble->row = currentBubble->row;
//            bubble->col = currentBubble->col;
//            bubble->x = currentBubble->x;
//            bubble->y = currentBubble->y;
            
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
            
            if (bubblesDeleted->count() >= 3) {
                deleteBubbles(bubblesDeleted);
                CCArray * deletes = getFloatBubbles();
                deleteBubbles(deletes);
            }
            
            currentBubble = NULL;
            inPositionFix = false;
            initShooter();

        }else{
            currentBubble->x += dt * currentBubble->vx;
            currentBubble->y += dt * currentBubble->vy;
        }
    }
    
    if(checkGameOver()){
        this->unscheduleUpdate();
        shootAble = false;
        
        CCLabelTTF* pLabel = CCLabelTTF::create("GAME OVER!", "Arial", 24);
        pLabel->setPosition(CCPointMake(STAGE_WIDTH/2,STAGE_HEIGHT/2));
        this->addChild(pLabel, 1);
    }    
}

void BobbleHero::updateBubblePostion(float dt){
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

CCPoint * BobbleHero::getBubblePostion(Bubble * bubble){
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

void BobbleHero::addBubble(Bubble *bubble){
    bubble->setPosition(CCPointMake(bubble->x, bubble->y));
    bubblesData->addObject(bubble);
    bubbleBatch->addChild(bubble);
    
    CCPoint * point = getBubblePostion(bubble);
    bubble->x = point->x;
    bubble->y = point->y;
    bubble->setPosition(CCPointMake(bubble->x, bubble->y));
}

void BobbleHero::removeBubble(Bubble *bubble){
    bubblesData->removeObject(bubble);
    bubbleBatch->removeChild(bubble, true);
}


Bubble * BobbleHero::getCollidedBubble(){
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

bool BobbleHero::isAvailablePostion(int row, int col){
    if(col < 0 || col > BUBBLE_COL || row < 0) return false;
    Bubble * temp = getBubble(row, col);
    if (temp == NULL || temp->bubbleID == 0) {
        return true;
    }else{
        return false;
    }
}

CCPoint * BobbleHero::getDropPos(Bubble *current, Bubble *collision){
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

void BobbleHero::startCollosionMotion(Bubble *bubble){
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

void BobbleHero::acceptBubble(CCPoint * dropPos){
    CCLOG("Accept!");
    //bubbleDropDistance -= 1;
    Bubble * bubble = getBubble(dropPos->y, dropPos->x);
    CCLOG("P:%f", dropPos->y);
    CCLOG("PP:%f", dropPos->x);
    if(bubble == NULL){
//        bubble = new Bubble(currentBubble->bubbleID, bubbleBatch->getTexture());
//        bubble->row = dropPos->y;
//        bubble->col = dropPos->x;
//        bubble->x = currentBubble->x;
//        bubble->y = currentBubble->y;
//        addBubble(bubble);
//        this->removeChild(currentBubble, true);

        //bubble->positionFix(getBubblePostion(bubble));
        
        currentBubble->row = dropPos->y;
        currentBubble->col = dropPos->x;
        CCPoint * point = getBubblePostion(currentBubble);
        currentBubble->nextX = point->x;
        currentBubble->nextY = point->y;

        float rad = atan2f(point->x-currentBubble->x, point->y - currentBubble->y);
        
        currentBubble->oldVx = currentBubble->vx;
        currentBubble->oldVy = currentBubble->vy;
        currentBubble->vx = shootSpeed * sin(rad);
        currentBubble->vy = shootSpeed * cos(rad);
        inPositionFix = true;
    }else{
        CCLOG("ERROR!!!");
    }
//    if (bubble->row % 2 == 0) {
//        bubble->x = bubble->radius + bubble->radius * bubble->col * 2;
//    }else{
//        bubble->x = bubble->radius + bubble->radius * (2 * bubble->col + 1);
//    }
//    bubble->y = STAGE_HEIGHT + (bubble->radius + bubble->radius * (BUBBLE_ROW - 1 - bubble->row) * 2) - bubbleDropDistance;
    
//    CCLOG("X:%fY:%f", bubble->x, bubble->y);
//    
//    startCollosionMotion(bubble);
//    
//    if(bubblesDeleted == NULL){
//        bubblesDeleted = new CCArray();
//    }
//    
//    if(bubblesChecked == NULL){
//        bubblesChecked = new CCArray();
//    }
//    
//    bubblesDeleted->removeAllObjects();
//    bubblesChecked->removeAllObjects();
//    checkDeleteBubbles(bubble);
//    
//    if (bubblesDeleted->count() >= 3) {
//        deleteBubbles(bubblesDeleted);
//        CCArray * deletes = getFloatBubbles();
//        deleteBubbles(deletes);
//    }
    //initShooter();
}

void BobbleHero::deleteBubbles(CCArray * bubbles){
    Bubble * tempBubble;
    for(int i = 0; i < bubbles->count(); i++){
        CCPoint * point = (CCPoint *) bubbles->objectAtIndex(i);
        tempBubble = getBubble(point->y, point->x);
        removeBubble(tempBubble);
    }
}

void BobbleHero::checkDeleteBubbles(Bubble * bubble){
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

CCArray * BobbleHero::getFloatBubbles(){
    free(bubblesConnected);
    bubblesConnected = CCArray::create();
    bubblesConnected->retain();
    
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

void BobbleHero::getConnectedBubble(Bubble *bubble){
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
            CCLOG("X:%i", (int) point->x);
            CCLOG("Y:%i", (int) point->y);
            bubblesConnected->addObject(point);
            getConnectedBubble(tempBubble);
        }
    }
    
    return ;
}

CCArray * BobbleHero::getNeighbors(Bubble * bubble){
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

Bubble * BobbleHero::getBubble(int row, int col){
    Bubble *bubble;

    for (int i=0; i<bubblesData->count(); i++) {
        bubble = (Bubble*) bubblesData->objectAtIndex(i);
        
        if (bubble->row == row && bubble->col == col) {
            return bubble;
        }
    }
    return NULL;
}

bool BobbleHero::checkGameOver(){
    Bubble * bubble;
    for (int i=0; i<bubblesData->count(); i++) {
        bubble = (Bubble*) bubblesData->objectAtIndex(bubblesData->count() - 1 - i);
        
        if (bubble != NULL && bubble->y <= Bubble::RADIUS) {
            return true;
        }
    }
    return false;
}

