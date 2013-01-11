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
        
    void update(vector<PhysicsObject*> &otherObjects, float dTime);
    
};

#endif /* defined(__raizWall__GravitationalPhysicsObject__) */
