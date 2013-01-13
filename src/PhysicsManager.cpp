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
    objects.push_back(object);
}

void PhysicsManager::removeObject(PhysicsObject *object)
{
    // TODO: find them and destroy them
}

void PhysicsManager::update(float dTime)
{

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
}

void PhysicsManager::draw()
{
    for (int i=0; i<objects.size(); i++)
    {
        objects[i]->draw();
    }
}
