//
//  PhysicsUtils.cpp
//  raizWall
//
//  Created by Virek on 1/13/13.
//
//

#include "PhysicsUtils.h"

#define PHY_THREAD_SLEEP_US 1000

#pragma mark - Move operation

PhysicsThreadedOperation::PhysicsThreadedOperation(){
    isProcessing = false;
    mode = PTOperationModeMove;
}

void PhysicsThreadedOperation::performMoves(vector<PhysicsObject *>::iterator begin, vector<PhysicsObject *>::iterator end, float dTime){
    if (!isProcessing){
        _dTime = dTime;
        _begin = begin;
        _end = end;
        mode = PTOperationModeMove;
        windowSize = ofGetWindowSize();
        isProcessing = true;
    }
}


void PhysicsThreadedOperation::performUpdates(ActivePhysicsObject *activeObject, vector<PhysicsObject *>::iterator begin, vector<PhysicsObject *>::iterator end, float dTime)
{
    if (!isProcessing){
        _dTime = dTime;
        _activeObject = activeObject;
        _begin = begin;
        _end = end;
        mode = PTOperationModeUpdate;
        windowSize = ofGetWindowSize();
        isProcessing = true;
    }
}

void PhysicsThreadedOperation::threadedFunction()
{
    while (isThreadRunning()){
    
        if (isProcessing){
            
            switch (mode) {
                case PTOperationModeMove: {
                    vector<PhysicsObject*>::iterator it = _begin;
                    while (it != _end){
                        (*it++)->move(_dTime, windowSize);
                    }
                }
                    
                    break;
                    
                case PTOperationModeUpdate:{
                    vector<PhysicsObject*> otherObjects(_begin, _end);
                    _activeObject->applyForces(otherObjects, _dTime);
                    isProcessing = false;
                }
                    break;
                    
                default:
                    break;
            }

            isProcessing = false;
        }
        usleep(PHY_THREAD_SLEEP_US);
    }
}