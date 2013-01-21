//
//  ParticlePhysicsObject.h
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#ifndef __raizWall__DotPhysicsObject__
#define __raizWall__DotPhysicsObject__

#include "PhysicsObject.h"

class ParticlePhysicsObject : public PhysicsObject {
    
public:
    
    ParticlePhysicsObject(float _radius){
        boundingRadius = _radius;
        mass = 0.001 * _radius * _radius;
    }
    
};

#endif /* defined(__raizWall__DotPhysicsObject__) */
