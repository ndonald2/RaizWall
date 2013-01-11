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
    for (int i=0; i<objects.size(); i++)
    {
        vector<PhysicsObject*>::const_iterator first = objects.begin() + i;
        vector<PhysicsObject*>::const_iterator last = objects.end();
        vector<PhysicsObject*> otherObjects(first,last);
        objects[i]->update(otherObjects, dTime);
        objects[i]->move(dTime);
        objects[i]->bounce();
    }
}

void PhysicsManager::draw()
{
    for (int i=0; i<objects.size(); i++)
    {
        objects[i]->draw();
    }
}
