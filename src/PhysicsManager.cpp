//
//  PhysicsManager.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsManager.h"

void PhysicsManager::addActiveObject(ActivePhysicsObject *object)
{
    mutex.lock();
    activeObjects.push_back(object);
    mutex.unlock();
}

void PhysicsManager::removeActiveObject(ActivePhysicsObject *object)
{
    mutex.lock();
    vector<ActivePhysicsObject*>::iterator it = activeObjects.begin();
    while (it++ != activeObjects.end()){
        if (*it == object){
            activeObjects.erase(it);
            break;
        }
    }
    mutex.unlock();
}

void PhysicsManager::addPassiveObject(PhysicsObject *object)
{
    mutex.lock();
    passiveObjects.push_back(object);
    mutex.unlock();
}

void PhysicsManager::removePassiveObject(PhysicsObject *object)
{
    mutex.lock();
    vector<PhysicsObject*>::iterator it = passiveObjects.begin();
    while (it++ != passiveObjects.end()){
        if (*it == object){
            passiveObjects.erase(it);
            break;
        }
    }
    mutex.unlock();
}

void PhysicsManager::update(float dTime)
{
    mutex.lock();
     
    // Step 1: Move
    for (int i=0; i<activeObjects.size(); i++)
    {
        activeObjects[i]->move(dTime);
    }
    for (int i=0; i<passiveObjects.size(); i++)
    {
        passiveObjects[i]->move(dTime);
    }
    
    // Step 2: Update physics state and resolve collisions
    // only need N-i objects for next operation, only active objects apply forces    
    for (int i=0; i<activeObjects.size(); i++)
    {
        activeObjects[i]->applyForces(passiveObjects, dTime);
    }
    
    // Step 3 - draw (separate call)
    mutex.unlock();
}

void PhysicsManager::draw()
{
    mutex.lock();
    for (int i=0; i<activeObjects.size(); i++)
    {
        activeObjects[i]->draw();
    }
    for (int i=0; i<passiveObjects.size(); i++)
    {
        passiveObjects[i]->draw();
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