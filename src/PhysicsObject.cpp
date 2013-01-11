//
//  PhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsObject.h"

#define kPOMinObjectDistance          2.0
#define kPOGravitationalConstant      100.0
#define kPOFrictionConstant           0.001

PhysicsObject::PhysicsObject()
{
    mass = 10.0;
    isAnchored = false;
    velocity = ofVec2f();
    position = ofVec2f();
}

void PhysicsObject::move(float dTime)
{
    // Update friction and position
    if (!isAnchored){
        velocity *= (1.0f - kPOFrictionConstant);
        position += velocity*dTime;
    }
}

void PhysicsObject::bounce()
{
    // Bounce
    if (position.x < 0){
        velocity.x *= -1;
        position.x = 0;
    }
    else if (position.x > ofGetWidth())
    {
        velocity.x *= -1;
        position.x = ofGetWidth();
    }
    
    if (position.y < 0){
        velocity.y *= -1;
        position.y = 0;
    }
    else if (position.y > ofGetHeight()){
        velocity.y *= -1;
        position.y = ofGetHeight();
    }
}
