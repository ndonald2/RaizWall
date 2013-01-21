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

#define USE_FORCE_MUTEX 0

// Base physics object is "passive" - it does not apply forces, only reacts to them

class PhysicsObject {
  
public:
    
    PhysicsObject();
    
    // Handle collisions with other objects
    virtual void handleCollisions(vector<PhysicsObject*> & otherObjects, float dTime);
    
    // Update position and bounce off walls
    virtual void move(float dTime, const ofVec2f & windowSize); // TODO: Get rid of window size in favor of "wall" objects

    // Are we intersecting? Basic bounding radius test (can be overridden)
    virtual bool intersecting(PhysicsObject * otherObject);
    
    // Did we pass through this other object on the last frame?
    virtual bool passedThrough(PhysicsObject * otherObject);
    
    // Amount of time since last frame that intersection occurred
    virtual float deltaTimeSinceIntersection(PhysicsObject * otherObject, float dTime);
    
    // Draw
    virtual void draw() {};
    
    // --------- Forces -------------
    // These can be updated on multiple threads. Not sure whether mutex is absolutely critical or not.
    // Thus, you can disable it at the top.
    inline void setForce(const ofVec2f & newForce);
    inline void addForce(const ofVec2f & forceToAdd);
    
#if USE_FORCE_MUTEX
    inline const ofVec2f getForce();
#else
    inline const ofVec2f & getForce();
#endif
    
    //--------- Setters ------------
    
    virtual void setIsAnchored(bool _isAnchored);
    
    virtual void setIsSolid(bool _isSolid);
    
    virtual void setCollisionMultiplier(float multiplier);
    
    virtual void setMass(float newMass);
    
    virtual void setAmbientFriction(float newAmbientFriction);
    
    virtual void setPosition(const ofVec2f & newPosition);
    
    virtual void setVelocity(const ofVec2f & newVelocity);
    
    
    //---------- Getters -------------
    
    inline bool             getIsAnchored() { return isAnchored; };
    inline bool             getIsSolid() { return isSolid; };
    inline float            getCollisionMultiplier() { return collisionMultiplier; }
    inline float            getBoundingRadius() { return boundingRadius; };
    inline float            getMass() { return mass; };
    inline float            getAmbientFriction() { return ambientFriction; };
    inline const ofVec2f  & getPosition() { return position; };
    inline const ofVec2f  & getLastPosition() { return lastPosition; };
    inline const ofVec2f  & getVelocity() { return velocity; };
    inline const ofVec2f  & getLastVelocity() { return lastVelocity; };
    
protected:

    // Collide with other objects
    virtual void collide(PhysicsObject * otherObject, float dTime);
    
    bool    isAnchored;
    bool    isSolid;
    
    float   mass;
    float   ambientFriction;
    float   collisionMultiplier;
    float   boundingRadius;
    
    ofVec2f force;
    ofVec2f acceleration;
    ofVec2f position;
    ofVec2f lastPosition;
    ofVec2f velocity;
    ofVec2f lastVelocity;
    
    Poco::FastMutex forceMutex;
    
};


#pragma mark - Active Object
// Active objects - apply and respond to forces

class ActivePhysicsObject : public PhysicsObject
{

public:
    // Update force/accel/velocity on other objects
    virtual void applyForces(vector<PhysicsObject*> & otherObjects, float dTime);
    
    // Force applied to other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime) { return ofVec2f::zero(); };
};


#endif /* defined(__raizWall__PhysicsObject__) */
