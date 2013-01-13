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
    virtual void resolveCollisions(vector<PhysicsObject*> & otherObjects, float dTime);
    
    // Force applied to other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime) { return ofVec2f(); };

    // Are we intersecting? Basic bounding radius test (can be overridden)
    virtual bool intersecting(PhysicsObject * otherObject);
    
    // Intersection factor between two circular bounding shapes
    virtual float intersectionFactor(PhysicsObject * otherObject, float dTime);
    
    // Draw
    virtual void draw() = 0;
    
    bool    isAnchored;
    
    float   boundingRadius;
    
    float   mass;
    float   ambientFriction;
    ofVec2f position;
    ofVec2f velocity;

    
    
};

#endif /* defined(__raizWall__PhysicsObject__) */
