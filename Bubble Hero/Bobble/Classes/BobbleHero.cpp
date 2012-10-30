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
    bubblesData = new CCArray(BUBBLE_ROW);
    
    CCArray *temp;
    for (int i=0; i<BUBBLE_ROW; i++) {
        temp = new CCArray(BUBBLE_COL);
        
        for (int j=0; j<BUBBLE_COL; j++) {
            if (i < currentRow) {
                temp->addObject(new Bubble(int (CCRANDOM_0_1() * 6)));
            }else{
                temp->addObject(new Bubble(0));
            }
        }
        
        bubblesData->addObject(temp);
    }
    
}

void BobbleHero::initShooter(){
    CCLog("initShooter");
    if (nextBubble == NULL) {
        nextBubble = new Bubble(int (CCRANDOM_0_1() * 5+1));
        currentBubble = new Bubble(int (CCRANDOM_0_1() * 5+1));
    }else{
        currentBubble = nextBubble;
        nextBubble = new Bubble(int (CCRANDOM_0_1() * 5+1));
    }
    
    nextBubble->x = STAGE_WIDTH/2 - 4 * nextBubble->radius;
    nextBubble->y = currentBubble->radius;
    
    currentBubble->x = STAGE_WIDTH/2;
    currentBubble->y = currentBubble->radius;
    
    shootAble = true;
}

void BobbleHero::shootBubble(float rad){
    if (shootAble) {
        shootAble = false;
        //currentBubble->active = true;
        currentBubble->vx = initSpeed * sin(rad);
        currentBubble->vy = initSpeed * cos(rad);
    }
}

void BobbleHero::addRow(int row){
    CCArray *temp;
    for (int i=0; i<row; i++) {
        temp = new CCArray(BUBBLE_COL);
        for (int j=0; j<BUBBLE_COL; j++) {
            temp->addObject(new Bubble(int (CCRANDOM_0_1() * 6)));
        }
        
        bubblesData->removeLastObject();
        bubblesData->insertObject(temp, 0);
    }
}

void BobbleHero::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint point = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    
    float rad = atan2f(point.x-currentBubble->x, point.y - currentBubble->y);
    
    shootBubble(rad);
}

void BobbleHero::draw(){
    
    CCLayer::draw();
        
    CCArray *tempArr;
    Bubble *bubble;
    CCPoint point;
        
    for (int i=0; i<BUBBLE_ROW; i++) {
        tempArr = (CCArray*) bubblesData->objectAtIndex(i);
        //CCLOG("i:%d", i);
        for (int j=0; j<BUBBLE_COL; j++) {
           // CCLOG("j:%d", j);
            bubble = (Bubble*) tempArr->objectAtIndex(j);
            
            if (i % 2 == 0) {
                point = CCPointMake(bubble->radius + bubble->radius * j * 2, STAGE_HEIGHT - (bubble->radius + bubble->radius * i * 2));
            }else{
                point = CCPointMake(bubble->radius + bubble->radius * (2 * j + 1), STAGE_HEIGHT - (bubble->radius + bubble->radius * i * 2));
            }
            bubble->row = i;
            bubble->col = j;
            bubble->x = point.x;
            bubble->y = point.y;
            
            if (bubble->bubbleID != 0) {
                if (bubble->bubbleID == 1) ccDrawColor4F(0, 1, 0, 1);
                if (bubble->bubbleID == 2) ccDrawColor4F(1, 0, 0, 1);
                if (bubble->bubbleID == 3) ccDrawColor4F(0, 0, 1, 1);
                if (bubble->bubbleID == 4) ccDrawColor4F(1, 1, 0, 1);
                if (bubble->bubbleID == 5) ccDrawColor4F(1, 0, 1, 1);
                if (bubble->bubbleID == 6) ccDrawColor4F(0, 1, 1, 1);
                if (bubble->bubbleID == 7) ccDrawColor4F(1, 1, 1, 1);
                
                ccDrawCircle(point, bubble->radius,0,50,false);
            }

        }
    }
    
    if (currentBubble != NULL) {
        if (currentBubble->bubbleID == 1) ccDrawColor4F(0, 1, 0, 1);
        if (currentBubble->bubbleID == 2) ccDrawColor4F(1, 0, 0, 1);
        if (currentBubble->bubbleID == 3) ccDrawColor4F(0, 0, 1, 1);
        if (currentBubble->bubbleID == 4) ccDrawColor4F(1, 1, 0, 1);
        if (currentBubble->bubbleID == 5) ccDrawColor4F(1, 0, 1, 1);
        if (currentBubble->bubbleID == 6) ccDrawColor4F(0, 1, 1, 1);
        if (currentBubble->bubbleID == 7) ccDrawColor4F(1, 1, 1, 1);
        
        ccDrawCircle(CCPointMake(currentBubble->x, currentBubble->y), bubble->radius,0,50,false);
    }
    
    if (nextBubble != NULL) {
        if (nextBubble->bubbleID == 1) ccDrawColor4F(0, 1, 0, 1);
        if (nextBubble->bubbleID == 2) ccDrawColor4F(1, 0, 0, 1);
        if (nextBubble->bubbleID == 3) ccDrawColor4F(0, 0, 1, 1);
        if (nextBubble->bubbleID == 4) ccDrawColor4F(1, 1, 0, 1);
        if (nextBubble->bubbleID == 5) ccDrawColor4F(1, 0, 1, 1);
        if (nextBubble->bubbleID == 6) ccDrawColor4F(0, 1, 1, 1);
        if (nextBubble->bubbleID == 7) ccDrawColor4F(1, 1, 1, 1);
        
        ccDrawCircle(CCPointMake(nextBubble->x, nextBubble->y), bubble->radius,0,50,false);
    }
}

void BobbleHero::update(float dt){
    //CCLOG("BBB");
    static float time = 0;
    time += dt;
    if (time > 20) {
        addRow(2);
        time -= 20;
    }
    
    if (currentBubble != NULL && shootAble == false) {
        if (!bubbleCollisionCheck()) {
            //CCLOG("No Coll");
            currentBubble->x += dt * currentBubble->vx;
            currentBubble->y += dt * currentBubble->vy;
            
            if(currentBubble->x < BUBBLE_LEFT_EAGE){
                currentBubble->x = BUBBLE_LEFT_EAGE + currentBubble->radius;
                currentBubble->vx *= -1;
            }
            if(currentBubble->x > BUBBLE_RIGHT_EAGE){
                currentBubble->x = BUBBLE_RIGHT_EAGE - currentBubble->radius;
                currentBubble->vx *= -1;
            }
            
            if (currentBubble->y > BUBBLE_TOP_EAGE - currentBubble->radius) {
                currentBubble->y = BUBBLE_TOP_EAGE - currentBubble->radius;
                acceptBubble();
            }
        }else{
            acceptBubble();
            CCLOG("bubbleCollisionCheck");
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


bool BobbleHero::bubbleCollisionCheck(){
    CCArray *tempArr;
    Bubble *bubble;
    float distance;
    float offsetX;
    float offsetY;
    for (int i=0; i<BUBBLE_ROW; i++) {
        tempArr = (CCArray*) bubblesData->objectAtIndex(i);
        for (int j=0; j<BUBBLE_COL; j++) {
            bubble = (Bubble*) tempArr->objectAtIndex(j);
            
            if (bubble->bubbleID == 0) {
                continue;
            }

            offsetX = currentBubble->x - bubble->x;
            offsetY = currentBubble->y - bubble->y;
            distance = sqrtf( offsetX * offsetX + offsetY * offsetY );
            
            //CCLOG("bubbleCollisionCheck%d", distance);
            if (distance <= bubble->radius + currentBubble->radius - 5) {
                return true;
            }
        }
    }
    
    return false;
}

void BobbleHero::acceptBubble(){
        
    float radius = currentBubble->radius;

    int row = (STAGE_HEIGHT - currentBubble->y - radius) / radius / 2;
    
    if((STAGE_HEIGHT - currentBubble->y) - row * currentBubble->radius * 2 > radius){
        row ++;
    }
    
    int col = (currentBubble->x) / radius / 2 ;
    
    if (row % 2 != 0) {
        col = (currentBubble->x - radius) / radius / 2 ;
    }
    
//    if( abs( currentBubble->x - col * currentBubble->radius * 2 - radius) > radius/2){
//        if(currentBubble->x - col * currentBubble->radius * 2 - radius > 0){
//            col++;
//        }else{
//            col--;
//        }
//    }
    
    Bubble * bubble = NULL;
    CCArray *arr = NULL;
    
    arr = (CCArray*) bubblesData->objectAtIndex(row);
    if(NULL != arr) bubble = (Bubble*) arr->objectAtIndex(col);
    
    if(bubble->bubbleID == 0){
        bubble->bubbleID = currentBubble->bubbleID;
        currentBubble->row = row;
        currentBubble->col = col;
    }else{
        CCLOG("AAAAAAA");
    }
    
    if(bubblesDeleted == NULL){
        bubblesDeleted = new CCArray();
    }
    
    if(bubblesChecked == NULL){
        bubblesChecked = new CCArray();
    }
    
    bubblesDeleted->removeAllObjects();
    bubblesChecked->removeAllObjects();
    checkDeleteBubbles(bubble);
    
    if (bubblesDeleted->count() >= 3) {
        deleteBubbles(bubblesDeleted);
        CCArray * deletes = getFloatBubbles();
        deleteBubbles(deletes);
    }
    
    initShooter();
}

void BobbleHero::deleteBubbles(CCArray * bubbles){
    Bubble * tempBubble;
    for(int i = 0; i < bubbles->count(); i++){
        CCPoint * point = (CCPoint *) bubbles->objectAtIndex(i);
        tempBubble = (Bubble *) ((CCArray *) bubblesData->objectAtIndex((int) point->y))->objectAtIndex((int) point->x);
        tempBubble->bubbleID = 0;
    }
}

void BobbleHero::checkDeleteBubbles(Bubble * bubble){
//    CCLOG("================");
    CCArray * neighbors = getNeighbors(bubble);
    neighbors->retain();
    Bubble * tempBubble;
    bool hasChecked = false;
    CCPoint *point;
    CCPoint *checkedPoint;
    for(int i = 0; i < neighbors->count(); i++){
        
        point = (CCPoint *) neighbors->objectAtIndex(i);
        CCLOG("P:%i", (int) point->y);
        CCLOG("PP:%i", (int) point->x);
        tempBubble = (Bubble *) ((CCArray *) bubblesData->objectAtIndex((int) point->y))->objectAtIndex((int) point->x);
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

    CCArray * firstRow = (CCArray *) bubblesData->objectAtIndex(0);
    Bubble * bubble;
    for (int i=0; i<BUBBLE_COL; i++) {
        bubble = (Bubble *) firstRow->objectAtIndex(i);
        
        if(bubble->bubbleID != 0){
            bubblesConnected->addObject(new CCPoint(bubble->col, bubble->row));
        }
    }
    
    for (int i=0; i<BUBBLE_COL; i++) {
        bubble = (Bubble *) firstRow->objectAtIndex(i);
        if(bubble->bubbleID != 0){
            getConnectedBubble(bubble);
        }
    }
    
    CCLOG("CL:%i", bubblesConnected->count());
    
    if(bubblesConnected != NULL){
        CCPoint *p;
        CCLog("(==Connected==)");
        for (int i=0; i<bubblesConnected->count(); i++) {
            p =  (CCPoint *) bubblesConnected->objectAtIndex(i);
            CCLog("(%d,%d)", (int) p->y, (int) p->x);
        }
        CCLog("(====)");
    }
    
    CCArray * out = CCArray::create();
    out->retain();
    CCArray *tempArr;
    Bubble *tempbubble;
    CCPoint * tempPoint;
    bool hasFLoat;
    for (int i=0; i<BUBBLE_ROW; i++) {
        tempArr = (CCArray*) bubblesData->objectAtIndex(i);
        for (int j=0; j<BUBBLE_COL; j++) {
            tempbubble = (Bubble*) tempArr->objectAtIndex(j);
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
        tempBubble = (Bubble *) ((CCArray *) bubblesData->objectAtIndex((int) point->y))->objectAtIndex((int) point->x);
        if (tempBubble->bubbleID == 0)  continue;
        
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
            //CCLOG("X:%i", (int) point->x);
            //CCLOG("Y:%i", (int) point->y);
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
        if(bubble->row + 1 < BUBBLE_ROW && bubble->col-1 >= 0)
            outArray->addObject(new CCPoint(bubble->col-1, bubble->row+1));
        //bottomRight
        if(bubble->row + 1 < BUBBLE_ROW)
            outArray->addObject(new CCPoint(bubble->col, bubble->row+1));
    }else{
        //same like top
        if(bubble->row-1 >= 0)
            outArray->addObject(new CCPoint(bubble->col, bubble->row-1));
        if(bubble->col+1 < BUBBLE_COL && bubble->row - 1 >= 0)
            outArray->addObject(new CCPoint(bubble->col+1, bubble->row-1));
        if(bubble->row + 1 < BUBBLE_ROW)
            outArray->addObject(new CCPoint(bubble->col, bubble->row+1));
        if(bubble->col+1 < BUBBLE_COL && bubble->row + 1 <= BUBBLE_ROW)
            outArray->addObject(new CCPoint(bubble->col+1, bubble->row+1));
    }
    
    return outArray;
}

bool BobbleHero::checkGameOver(){
    CCArray *tempArr;
    Bubble *tempbubble;
    tempArr = (CCArray*) bubblesData->objectAtIndex(BUBBLE_ROW-3);
    for (int j=0; j<BUBBLE_COL; j++) {
        tempbubble = (Bubble*) tempArr->objectAtIndex(j);
        if (tempbubble->bubbleID != 0) {
            return true;
        }
    }
    return false;
}

