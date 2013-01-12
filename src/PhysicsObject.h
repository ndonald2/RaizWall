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
    
    // Update force/accel/velocity
    void update(vector<PhysicsObject*> & otherObjects, float dTime);
    
    // Update position
    virtual void move(float dTime);
    
    // Collide with other objects (and walls)
    virtual void resolveCollisions(vector<PhysicsObject*> & otherObjects, float dTime) {};
    
    // Force applied to other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime) { return ofVec2f(); };

    
    // Draw
    virtual void draw() = 0;
    
    bool    isAnchored;
    float   mass;
    float   friction;
    ofVec2f position;
    ofVec2f velocity;

};

#endif /* defined(__raizWall__PhysicsObject__) */
