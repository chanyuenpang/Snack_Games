//
//  Bubble.cpp
//  Bobble
//
//  Created by VIP on 10/14/12.
//
//

#include "Bubble.h"
#include "BobbleHero.h"
#include "Const.h"

Bubble::Bubble(int id, CCTexture2D * texture){
    texture->getName();
    //CCTexture2D *pTexture2D  = CCTextureCache::sharedTextureCache()->addImage("bubbles.png");
    this->initWithTexture(texture, CCRectMake(2 * RADIUS * (id-1) , 0, RADIUS * 2, RADIUS * 2));
    this->getTexture()->getName();
    this->bubbleID = id;
}

void Bubble::collosionMotion(Bubble * current){
    active = true;
    //CCLOG("collosionMotion");
    
    oldX = this->x;
    oldY = this->y;
    
    float offsetX = this->x - current->x;
    float offsetY = this->y - current->y;
    float dis = sqrt(offsetX * offsetX + offsetY * offsetY);
    float offset = dis / RADIUS;
    
    if (offset > COLLISON_EFFECT_RANG * 2 || this->row - current->row > 1) {
        active = false;
        return;
    }

    if (current != this) {
        float speed = sqrt(current->vx * current->vx + current->vy * current->vy);
        float r = atan2f(offsetX, offsetY);
        
        this->vx = sinf(r) * 10 * (COLLISON_EFFECT_RANG * 2 - offset) / COLLISON_EFFECT_RANG * 2;
        this->vy = cosf(r) * 20 * (COLLISON_EFFECT_RANG * 2 - offset) / COLLISON_EFFECT_RANG * 2;
    }else{
        this->vx = this->vx/1.5/2;
        this->vy = this->vy/1.5/2;
        
        if (this->vy < 60) {
            this->vy = 60;
        }
    }
    
    this->oldVx = this->vx;
    this->oldVy = this->vy;
    
    this->ax = -this->vx / 0.5;
    this->ay = -this->vy / 0.5;
    
    updateTime = 0;
    isRevert = false;
}

void Bubble::update(float dt){
    if (!active) {
        return;
    }
    
    updateTime += dt;
    if (updateTime > 0.1 && !isRevert) {
        this->vx = - vx/2.5;
        this->vy = - vy/2.5;
        ax = - ax/2.5;
        ay = - ay/2.5;
        isRevert = true;
    }
    
    this->x += this->vx * dt;
    this->y += this->vy * dt;
    
    if(isRevert){
        this->vx += this->ax * dt;
        this->vy += this->ay * dt;
    }
    
    float absVx = this->vx > 0 ? this->vx : -this->vx;
    float absVy = this->vy > 0 ? this->vy : -this->vy;
    
    float absOldVx = this->oldVx > 0 ? this->oldVx : -this->oldVx;
    float absOldVy = this->oldVy > 0 ? this->oldVy : -this->oldVy;
    
//    float offsetX = this->x - this->oldX;
//    float offsetY = this->y - this->oldY;
//    float dis = sqrtf(offsetX * offsetX + offsetY * offsetX);
    
//    if (absVx > absOldVx) {
//        this->x = this->oldX;
//        this->vx = 0;
//        this->ax = 0;
//    }
//    
//    if(absVy > absOldVy){
//        this->y = this->oldY;
//        this->vy = 0;
//        this->ay = 0;
//    }
    if(updateTime >= 0.5/*!vx && !vy && !ax && !ay*/){
        this->active = false;
    }
    this->setPosition(CCPointMake(x, y));
}


void Bubble::motionComplete(Bubble *bubble){
    bubble->active = false;
    
}

//void Bubble::positionFix(CCPoint * point, float speed){
//    active = true;
//    
//    nextX = point->x;
//    nextY = point->y;
//    
//    float rad = atan2f(point->x-this->x, point->y - this->y);
//    this->vx = speed * sin(rad);
//    this->vy = speed * cos(rad);
//    
//    
//    CCActionInterval*  actionTo = CCMoveTo::create(0.2, CCPointMake(point->x, point->y));
//    CCCallFunc* callBack = CCCallFunc::create(this, callfunc_selector(Bubble::motionComplete));
//    this->runAction(CCSequence::create(actionTo, callBack, NULL));
//}

void Bubble::draw(){
    this->setPosition(CCPointMake(x, y));
    CCSprite::draw();
//    if (this->bubbleID == 1) ccDrawColor4F(0, 1, 0, 1);
//    if (this->bubbleID == 2) ccDrawColor4F(1, 0, 0, 1);
//    if (this->bubbleID == 3) ccDrawColor4F(0, 0, 1, 1);
//    if (this->bubbleID == 4) ccDrawColor4F(1, 1, 0, 1);
//    if (this->bubbleID == 5) ccDrawColor4F(1, 0, 1, 1);
//    if (this->bubbleID == 6) ccDrawColor4F(0, 1, 1, 1);
//    if (this->bubbleID == 7) ccDrawColor4F(1, 1, 1, 1);
//    
//    if (this->bubbleID != 0) {
//        ccDrawCircle(CCPointMake(this->x, this->y), this->radius,0,10,false);
//    }
}