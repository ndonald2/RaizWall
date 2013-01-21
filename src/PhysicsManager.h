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
#include "PhysicsUtils.h"
#include "ofMain.h"

#define USE_THREADED_UPDATES  1
#define NUM_UPDATE_THREADS    4

class PhysicsManager {

public:

    PhysicsManager();
    ~PhysicsManager();
    
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
    
    vector<PhysicsObject*>          allObjects;
    vector<ActivePhysicsObject*>    activeObjects;
    vector<PhysicsObject*>          passiveObjects;
    Poco::Mutex                     mutex;
    
#if USE_THREADED_UPDATES
    PhysicsThreadedOperation        threadedOperations[NUM_UPDATE_THREADS];
#endif
};


#endif /* defined(__raizWall__PhysicsManager__) */
