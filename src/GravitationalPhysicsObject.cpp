//
//  GravitationalPhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "GravitationalPhysicsObject.h"

#define kPOMinObjectDistance          2.0
#define kPOGravitationalConstant      10.0
#define kPOFrictionConstant           0.05


void GravitationalPhysicsObject::update(vector<PhysicsObject*> &otherObjects, float dTime)
{
    static float maxDistance = 1000000000000;
    
    for (int i=0; i<otherObjects.size(); i++){
        
        PhysicsObject *otherObject = otherObjects[i];
        if (otherObject == this) continue;
        
        // Step 1: Calculate force
        
        // jitter if we need to to avoid locking
        if ((position - otherObject->position).length() < kPOMinObjectDistance){
            position += ofVec2f(ofRandom(1,2), ofRandom(1,2));
        }
        
        ofVec2f pDiff       = position - otherObject->position;
        ofVec2f pDiffNorm   = pDiff.normalized();
        float   pDiffAbs    = MAX(pDiff.length(), kPOMinObjectDistance);
        
        // too far away to care?
        if (pDiffAbs <= maxDistance){
            
            float absForce = kPOGravitationalConstant * mass * otherObject->mass / powf(pDiffAbs, 2.0);
            
            // force vector
            ofVec2f force = absForce*pDiffNorm;
            
            // update my state
            ofVec2f acceleration = -force/mass;
            velocity += acceleration*dTime;
            
            // update other state
            if (!otherObject->isAnchored){
                ofVec2f otherAcceleration = force/otherObject->mass;
                otherObject->velocity += otherAcceleration*dTime;
            }
            
//            if (typeid(*otherObject) == typeid(DotPhysicsObject)){
//                if (pDiffAbs <= radius + ((DotPhysicsObject*)otherObject)->getRadius())
//                {
//                    velocity *= -1;
//                    otherObject->velocity *= -1;
//                }
//            }
        }
    }
}