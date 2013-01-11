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
    void update(vector<PhysicsObject*> &otherObjects, float dTime);
    
    // Update position
    virtual void move(float dTime);
    
    // Bounce off walls -- keeps it on the screen
    virtual void bounce();
    
    // Force applied by other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime) { return ofVec2f(); };
    
    // Draw
    virtual void draw() = 0;
    
    bool    isAnchored;
    float   mass;
    ofVec2f position;
    ofVec2f velocity;
    
protected:
    
    // Collide with other object
    virtual void collide(PhysicsObject * otherObject) = 0;
};

#endif /* defined(__raizWall__PhysicsObject__) */
