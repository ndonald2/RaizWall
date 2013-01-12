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
    // Force applied by other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime);
    
    inline void setIsRepulsor(bool repulsor) { isRepulsor = true; };
    
protected:
    
    bool isRepulsor;
};

#endif /* defined(__raizWall__GravitationalPhysicsObject__) */
