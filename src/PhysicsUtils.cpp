//
//  PhysicsUtils.cpp
//  raizWall
//
//  Created by Virek on 1/13/13.
//
//

#include "PhysicsUtils.h"

#pragma mark - Move operation

PhysicsMoveOperation::PhysicsMoveOperation(){
    isProcessing = false;
}

void PhysicsMoveOperation::performMoves(vector<PhysicsObject *>::iterator begin, vector<PhysicsObject *>::iterator end, float dTime){
    if (!isProcessing){
        _dTime = dTime;
        _begin = begin;
        _end = end;
        isProcessing = true;
    }
}

void PhysicsMoveOperation::threadedFunction()
{
    while (isThreadRunning()){
    
        if (isProcessing){
            vector<PhysicsObject*>::iterator it = _begin;
            while (it != _end){
                (*it++)->move(_dTime);
            }
            isProcessing = false;
        }
        usleep(100);
    }
}

#pragma mark - Update Operation

PhysicsUpdateOperation::PhysicsUpdateOperation(){
    isProcessing = false;
}

void PhysicsUpdateOperation::performUpdates(ActivePhysicsObject *activeObject, vector<PhysicsObject *>::iterator begin, vector<PhysicsObject *>::iterator end, float dTime)
{
    if (!isProcessing){
        _dTime = dTime;
        _activeObject = activeObject;
        _begin = begin;
        _end = end;
        isProcessing = true;
    }
}

void PhysicsUpdateOperation::threadedFunction()
{
    while (isThreadRunning()){
        if (isProcessing){
            vector<PhysicsObject*> otherObjects(_begin, _end);
            _activeObject->applyForces(otherObjects, _dTime);
            isProcessing = false;
        }
        usleep(100);
    }

}