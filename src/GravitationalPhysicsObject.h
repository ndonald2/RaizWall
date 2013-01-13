//
//  GravitationalPhysicsObject.h
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#ifndef __raizWall__GravitationalPhysicsObject__
#define __raizWall__GravitationalPhysicsObject__

#include "PhysicsObject.h"

class GravitationalPhysicsObject : public PhysicsObject {

public:
    
    GravitationalPhysicsObject();
    
    // no drawing by default
    virtual void draw() {};
    
    // Force applied by other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime);
    
    inline void setIsRepulsor(bool repulsor) { isRepulsor = repulsor; };
    
    // so forces don't become ridiculously large
    float minDistanceThresh;
    
protected:
    
    bool isRepulsor;
};

#endif /* defined(__raizWall__GravitationalPhysicsObject__) */
