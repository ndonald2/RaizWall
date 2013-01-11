//
//  PhysicsObject.h
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#ifndef __raizWall__PhysicsObject__
#define __raizWall__PhysicsObject__

#include "ofMain.h"

class PhysicsObject {
  
public:
    
    PhysicsObject();
    
    virtual void update(vector<PhysicsObject*> &otherObjects, float dTime) = 0;
    virtual void move(float dTime);
    virtual void bounce();
    virtual void draw() = 0;
        
    bool    isAnchored;
    float   mass;
    ofVec2f position;
    ofVec2f velocity;
};

#endif /* defined(__raizWall__PhysicsObject__) */
