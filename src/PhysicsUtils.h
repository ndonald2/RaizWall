//
//  PhysicsUtils.h
//  raizWall
//
//  Created by Virek on 1/13/13.
//
//

#ifndef __raizWall__PhysicsUtils__
#define __raizWall__PhysicsUtils__

#include "PhysicsObject.h"

class PhysicsMoveOperation : public ofThread
{
public:
    PhysicsMoveOperation();
    void performMoves(vector<PhysicsObject*>::iterator begin, vector<PhysicsObject*>::iterator end, float dTime);
    bool getIsProcessing() { return isProcessing; };
    
protected:
    void threadedFunction();
    
private:
    bool  isProcessing;
    float _dTime;
    vector<PhysicsObject*>::iterator _begin;
    vector<PhysicsObject*>::iterator _end;
};

class PhysicsUpdateOperation : public ofThread
{
public:
    
    PhysicsUpdateOperation();
    void performUpdates(ActivePhysicsObject * activeObject, vector<PhysicsObject*>::iterator begin, vector<PhysicsObject*>::iterator end, float dTime);
    bool getIsProcessing() { return isProcessing; };

protected:
    void threadedFunction();
    
private:
    
    bool  isProcessing;

    float                      _dTime;
    ActivePhysicsObject *      _activeObject;
    vector<PhysicsObject*>::iterator _begin;
    vector<PhysicsObject*>::iterator _end;
};

#endif /* defined(__raizWall__PhysicsUtils__) */
