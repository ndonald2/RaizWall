//
//  PhysicsUtils.cpp
//  raizWall
//
//  Created by Virek on 1/13/13.
//
//

#include "PhysicsUtils.h"

#pragma mark - Move operation

void PhysicsMoveOperation::performMoves(vector<PhysicsObject *>::iterator begin, vector<PhysicsObject *>::iterator end, float dTime){
    if (!isThreadRunning()){
        _dTime = dTime;
        _begin = begin;
        _end = end;
        try{
            startThread();
        }
        catch (Poco::SystemException & e){
            
        }
    }
}
void PhysicsMoveOperation::threadedFunction()
{
    vector<PhysicsObject*>::iterator it = _begin;
    while (it != _end){
        (*it++)->move(_dTime);
    }
}

#pragma mark - Update Operation

void PhysicsUpdateOperation::performUpdates(ActivePhysicsObject *activeObject, vector<PhysicsObject *>::iterator begin, vector<PhysicsObject *>::iterator end, float dTime)
{
    if (!isThreadRunning()){
        _dTime = dTime;
        _activeObject = activeObject;
        _begin = begin;
        _end = end;
        try{
            startThread();
        }
        catch (Poco::SystemException & e){
            
        }
    }
}

void PhysicsUpdateOperation::threadedFunction()
{
    vector<PhysicsObject*> otherObjects(_begin, _end);
    _activeObject->applyForces(otherObjects, _dTime);
}