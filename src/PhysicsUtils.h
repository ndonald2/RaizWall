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

typedef enum {
    
    PTOperationModeMove,
    PTOperationModeUpdate,
    PTOperationModeCollide // TODO
    
} PhysicsThreadedOperationMode;

class PhysicsThreadedOperation : public ofThread
{
public:
    PhysicsThreadedOperation();
    
    void performMoves(vector<PhysicsObject*>::iterator begin, vector<PhysicsObject*>::iterator end, float dTime);
    void performUpdates(ActivePhysicsObject * activeObject, vector<PhysicsObject*>::iterator begin, vector<PhysicsObject*>::iterator end, float dTime);
    
    bool getIsProcessing() { return isProcessing; };
    
protected:
    void threadedFunction();
    
private:
    bool                            isProcessing;
    PhysicsThreadedOperationMode    mode;
    float                           _dTime;
    
    ofVec2f windowSize;
    
    ActivePhysicsObject *            _activeObject;
    vector<PhysicsObject*>::iterator _begin;
    vector<PhysicsObject*>::iterator _end;
};

#endif /* defined(__raizWall__PhysicsUtils__) */
