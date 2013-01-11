//
//  PhysicsManager.h
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#ifndef __raizWall__PhysicsManager__
#define __raizWall__PhysicsManager__

#include "PhysicsObject.h"
#include "ofMain.h"

class PhysicsManager {

public:

    void addObject(PhysicsObject *object);
    void removeObject(PhysicsObject *object);
    
    void update(float dTime);
    void draw();
    
private:
    
    vector<PhysicsObject*>   objects;
    
};


#endif /* defined(__raizWall__PhysicsManager__) */
