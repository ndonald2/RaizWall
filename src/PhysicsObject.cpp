//
//  PhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
    friction = 0.01f;
    mass = 10.0;
    isAnchored = false;
    velocity = ofVec2f();
    position = ofVec2f();
}

void PhysicsObject::update(vector<PhysicsObject*> & otherObjects, float dTime)
{    
    for (int i=0; i<otherObjects.size(); i++){
        
        PhysicsObject *otherObject = otherObjects[i];
        
        // Step 1: Calculate forces
        ofVec2f forceToMe = otherObject->forceAppliedTo(this, dTime);
        ofVec2f forceToOther = forceAppliedTo(otherObject, dTime);
        
        // Step 2: update my state
        ofVec2f acceleration = forceToMe/mass;
        velocity += acceleration*dTime;
        
        // Step 3: update other state
        ofVec2f otherAcceleration = forceToOther/otherObject->mass;
        otherObject->velocity += otherAcceleration*dTime;

    }
}

void PhysicsObject::move(float dTime)
{
    // Update velocity and position
    if (!isAnchored){
        velocity *= (1.0f - friction);
        position += velocity*dTime;
    }    
}

//void PhysicsObject::bounce()
//{
//    // Bounce
//    if (position.x < 0){
//        velocity.x *= -1;
//        position.x = 0;
//    }
//    else if (position.x > ofGetWidth())
//    {
//        velocity.x *= -1;
//        position.x = ofGetWidth();
//    }
//    
//    if (position.y < 0){
//        velocity.y *= -1;
//        position.y = 0;
//    }
//    else if (position.y > ofGetHeight()){
//        velocity.y *= -1;
//        position.y = ofGetHeight();
//    }
//}
