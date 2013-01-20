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

    // Active objects apply forces to other objects, but can also have forces
    // applied to them.
    void addActiveObject(ActivePhysicsObject *object);
    void removeActiveObject(ActivePhysicsObject *object);
    
    // Passive objects only react to other forces
    void addPassiveObject(PhysicsObject *object);
    void removePassiveObject(PhysicsObject *object);
    
    void update(float dTime);
    void draw();
    
    // Other processes attempting to add/remove/modify objects from another thread
    // should invoke the lock to ensure synchronization
    void lock();
    void unlock();
    
private:
    
    vector<ActivePhysicsObject*>    activeObjects;
    vector<PhysicsObject*>          passiveObjects;
    Poco::Mutex                     mutex;
};


#endif /* defined(__raizWall__PhysicsManager__) */
