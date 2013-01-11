//
//  PhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsObject.h"

#define kPOFrictionConstant           0.001

PhysicsObject::PhysicsObject()
{
    mass = 10.0;
    isAnchored = false;
    velocity = ofVec2f();
    position = ofVec2f();
}

void PhysicsObject::update(vector<PhysicsObject*> &otherObjects, float dTime)
{    
    for (int i=0; i<otherObjects.size(); i++){
        
        PhysicsObject *otherObject = otherObjects[i];
        if (otherObject == this) continue;
        
        // Step 1: Calculate forces
        ofVec2f forceToMe = otherObject->forceAppliedTo(this, dTime);
        ofVec2f forceToOther = forceAppliedTo(otherObject, dTime);
        
        // update my state
        ofVec2f acceleration = forceToMe/mass;
        velocity += acceleration*dTime;
        
        // update other state
        ofVec2f otherAcceleration = forceToOther/otherObject->mass;
        otherObject->velocity += otherAcceleration*dTime;
        
        // detect and resolve collisions
        collide(otherObject);
    }
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
