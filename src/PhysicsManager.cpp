//
//  PhysicsManager.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsManager.h"

void PhysicsManager::addObject(PhysicsObject *object)
{
    mutex.lock();
    objects.push_back(object);
    mutex.unlock();
}

void PhysicsManager::removeObject(PhysicsObject *object)
{
    mutex.lock();
    vector<PhysicsObject*>::iterator it = objects.begin();
    while (it++ != objects.end()){
        if (*it == object){
            objects.erase(it);
            break;
        }
    }
    mutex.unlock();
}

void PhysicsManager::update(float dTime)
{
    mutex.lock();
    
    // Step 1: Move
    for (int i=0; i<objects.size(); i++)
    {
        objects[i]->move(dTime);
    }
    
    // Step 2: Update physics state and resolve collisions
    // only need N-i objects for next operation
    vector<PhysicsObject*>::const_iterator first = objects.begin() + 1;
    vector<PhysicsObject*>::const_iterator last = objects.end();
    for (int i=0; i<objects.size(); i++)
    {
        vector<PhysicsObject*> otherObjects(first,last);
        objects[i]->update(otherObjects, dTime);
        first++;
    }
    
    // Step 3 - draw (separate call)
    
    mutex.unlock();
}

void PhysicsManager::draw()
{
    mutex.lock();
    for (int i=0; i<objects.size(); i++)
    {
        objects[i]->draw();
    }
    mutex.unlock();
}

// Public Lock/Unlock

void PhysicsManager::lock()
{
    mutex.lock();
}

void PhysicsManager::unlock()
{
    mutex.unlock();
}