//
//  PhysicsManager.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
#if USE_THREADED_UPDATES
    for (int i=0; i<NUM_UPDATE_THREADS; i++)
    {
        moveOperations[i].startThread();
        updateOperations[i].startThread();
    }
#endif
}

PhysicsManager::~PhysicsManager()
{
#if USE_THREADED_UPDATES
    for (int i=0; i<NUM_UPDATE_THREADS; i++)
    {
        moveOperations[i].waitForThread(true);
        updateOperations[i].waitForThread(true);
    }
#endif
}

void PhysicsManager::addActiveObject(ActivePhysicsObject *object)
{
    mutex.lock();
    activeObjects.push_back(object);
    allObjects.push_back(object);
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
    vector<PhysicsObject*>::iterator ait = allObjects.begin();
    while (ait++ != allObjects.end()){
        if (*ait == object){
            allObjects.erase(ait);
            break;
        }
    }
    mutex.unlock();
}

void PhysicsManager::addPassiveObject(PhysicsObject *object)
{
    mutex.lock();
    passiveObjects.push_back(object);
    allObjects.push_back(object);
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
    vector<PhysicsObject*>::iterator ait = allObjects.begin();
    while (ait++ != allObjects.end()){
        if (*ait == object){
            allObjects.erase(ait);
            break;
        }
    }
    mutex.unlock();
}

void PhysicsManager::update(float dTime)
{
    mutex.lock();
        
    if (allObjects.size() > 0){
    
        // Step 1: Move
    #if USE_THREADED_UPDATES
        
        int objOffset = 0;
        int objChunkSize = allObjects.size()/NUM_UPDATE_THREADS;
        
        for (int i=0; i<NUM_UPDATE_THREADS; i++)
        {
            vector<PhysicsObject*>::iterator first = allObjects.begin() + i*objChunkSize;
            vector<PhysicsObject*>::iterator last = (i == NUM_UPDATE_THREADS - 1) ? allObjects.end() : first + objChunkSize;
            moveOperations[i].performMoves(first, last, dTime);
        }
        
        // wait for operations to finish
        bool threadsFinished;
        do {
            
            threadsFinished = true;
            for (int i=0; i<NUM_UPDATE_THREADS; i++){
                if (moveOperations[i].getIsProcessing()){
                    threadsFinished = false;
                    break;
                }
            };
            
        } while (!threadsFinished);
        
    #else
        
        for (int i=0; i<activeObjects.size(); i++)
        {
            activeObjects[i]->move(dTime);
        }
        for (int i=0; i<passiveObjects.size(); i++)
        {
            passiveObjects[i]->move(dTime);
        }
        
    #endif
        
        // Step 2: Update physics state and resolve collisions
        // only need N-i objects for next operation, only active objects apply forces
    #if USE_THREADED_UPDATES
        
        // Use threaded operations, divide passive objects in half for each active object
        // Only makes sense if more than some threshold of passive objects
        
        objOffset = 0;
        
        for (int a=0; a<activeObjects.size(); a++){
        
            for (int i=0; i<NUM_UPDATE_THREADS; i++)
            {
                vector<PhysicsObject*>::iterator first = allObjects.begin() + i*objChunkSize;
                vector<PhysicsObject*>::iterator last = (i == NUM_UPDATE_THREADS - 1) ? allObjects.end() : first + objChunkSize;
                updateOperations[i].performUpdates(activeObjects[a], first, last, dTime);
            }
            
            // wait for operations to finish
            do {
                
                threadsFinished = true;
                for (int i=0; i<NUM_UPDATE_THREADS; i++){
                    if (updateOperations[i].getIsProcessing()){
                        threadsFinished = false;
                        break;
                    }
                };
                
            } while (!threadsFinished);
        }
        
    #else
        for (int i=0; i<activeObjects.size(); i++)
        {
            activeObjects[i]->applyForces(passiveObjects, dTime);
        }
    #endif
    // Step 3 - draw (separate call)
        
    }
    
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