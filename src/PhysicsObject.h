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
    
    // Update force/accel/velocity and handle collisions
    virtual void update(vector<PhysicsObject*> & otherObjects, float dTime);
    
    // Collide with other objects
    virtual void collide(PhysicsObject * otherObject, float dTime);
    
    // Update position
    virtual void move(float dTime);
    
    // Force applied to other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime) { return ofVec2f(); };

    // Are we intersecting? Basic bounding radius test (can be overridden)
    virtual bool intersecting(PhysicsObject * otherObject);
    
    // Did we pass through this other object on the last frame?
    virtual bool passedThrough(PhysicsObject * otherObject);
    
    // Amount of time since last frame that intersection occurred
    virtual float deltaTimeSinceIntersection(PhysicsObject * otherObject, float dTime);
    
    // Draw
    virtual void draw() {};
    
    
    //--------- Setters ------------
    
    virtual void setIsAnchored(bool _isAnchored);
    
    virtual void setIsSolid(bool _isSolid);
    
    virtual void setMass(float newMass);
    
    virtual void setPosition(const ofVec2f & newPosition);
    
    virtual void setVelocity(const ofVec2f & newVelocity);
    
    
    //---------- Getters -------------
    
    inline bool             getIsAnchored() { return isAnchored; };
    inline bool             getIsSolid() { return isSolid; };
    inline float            getBoundingRadius() { return boundingRadius; };
    inline float            getMass() { return mass; };
    inline const ofVec2f  & getPosition() { return position; };
    inline const ofVec2f  & getLastPosition() { return lastPosition; };
    inline const ofVec2f  & getVelocity() { return velocity; };
    inline const ofVec2f  & getLastVelocity() { return lastVelocity; };
    
    
protected:
    
    bool    isAnchored;
    bool    isSolid;
    
    float   mass;
    float   ambientFriction;
    
    float   boundingRadius;
    
    ofVec2f force;
    ofVec2f position;
    ofVec2f lastPosition;
    ofVec2f velocity;
    ofVec2f lastVelocity;
    
};

#endif /* defined(__raizWall__PhysicsObject__) */
