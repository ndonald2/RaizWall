//
//  GravitationalPhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "GravitationalPhysicsObject.h"

#define kPOGravitationalConstant      100.0

GravitationalPhysicsObject::GravitationalPhysicsObject()
{
    isRepulsor = false;
    boundingRadius = 0;
    minDistanceThresh = 20;
}

ofVec2f GravitationalPhysicsObject::forceAppliedTo(PhysicsObject *otherObject, float dTime)
{
    static float maxDistance = 1000000000000;

    ofVec2f force = ofVec2f();
    
    if (!otherObject->isAnchored){
    
        ofVec2f pDiff       = position - otherObject->position;
        ofVec2f pDiffNorm   = pDiff.normalized();
        float   pDiffAbs    = MAX(pDiff.length(), minDistanceThresh);
        
        // too far away to care?
        if (pDiffAbs <= maxDistance){
            
            float absForce = kPOGravitationalConstant * mass * otherObject->mass / powf(pDiffAbs, 2.0);
            
            // force vector
            force = absForce*pDiffNorm * (isRepulsor ? -1.0 : 1.0);
        }
    }
    
    return force;
}
