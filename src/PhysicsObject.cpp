//
//  PhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsObject.h"
#include "PhysicsUtils.h"

PhysicsObject::PhysicsObject()
{
    isAnchored = false;
    isSolid = true;
    boundingRadius = 1.0f;
    ambientFriction = 0.001f;
    mass = 10.0;
    velocity = ofVec2f();
    position = ofVec2f();
    lastPosition = ofVec2f();
}

void PhysicsObject::update(vector<PhysicsObject*> & otherObjects, float dTime)
{    
    for (int i=0; i<otherObjects.size(); i++){
        
        PhysicsObject *otherObject = otherObjects[i];
        
        // handle collisions
        // A-posteriori collision detection
        if (intersecting(otherObject) || passedThrough(otherObject) || otherObject->passedThrough(this)){
            
            collide(otherObject, dTime);
            
            // if still intersecting, split the difference
            if (intersecting(otherObject))
            {
                ofVec2f difference = otherObject->position - position;
                if (difference.length() == 0) difference = ofVec2f(1,0);
                float overlap = (boundingRadius + otherObject->boundingRadius - position.distance(otherObject->position));
                                
                difference.normalize();
                
                if (isAnchored){
                    otherObject->position += overlap * difference;
                }
                else if (otherObject->isAnchored){
                    position -= overlap * difference;
                }
                else{
                    position -= overlap * (difference/2.0f);
                    otherObject->position += overlap * (difference/2.0f);
                }
            }
        }
        else {
            
            // Step 1: Calculate forces
            ofVec2f forceToMe = otherObject->forceAppliedTo(this, dTime);
            ofVec2f forceToOther = forceAppliedTo(otherObject, dTime);
            
            // Step 2: update my state
            ofVec2f acceleration = forceToMe/mass;
            velocity += acceleration*dTime;
            
            // Step 3: update other state
            ofVec2f otherAcceleration = forceToOther/otherObject->mass;
            otherObject->velocity += otherAcceleration*dTime;
        }

    }
    
    // Collide against walls
    if (position.x - boundingRadius < 0){
        velocity.x *= -1;
        position.x = boundingRadius;
    }
    else if (position.x + boundingRadius > ofGetWidth())
    {
        velocity.x *= -1;
        position.x = ofGetWidth() - boundingRadius;
    }
    
    if (position.y - boundingRadius < 0){
        velocity.y *= -1;
        position.y = boundingRadius;
    }
    else if (position.y + boundingRadius > ofGetHeight()){
        velocity.y *= -1;
        position.y = ofGetHeight() - boundingRadius;
    }
}


void PhysicsObject::collide(PhysicsObject *otherObject, float dTime)
{
    
    // Assume perfectly elastic collisions
    // http://stackoverflow.com/questions/345838/ball-to-ball-collision-detection-and-handling
    
    ofVec2f uvVelocity = velocity.normalized();
    
    // get interpolation (how far along were we in the frame when we collided)
    float dTimeOfCollision = deltaTimeSinceIntersection(otherObject, dTime);
    
    if (!isAnchored){
        // back up one interpolated step for tangential collision
        position = lastPosition + velocity*dTimeOfCollision;
    }
    
    if (!otherObject->isAnchored){
        // back other object one interpolated step
        otherObject->position = otherObject->lastPosition + otherObject->velocity*dTimeOfCollision;
    }
    
    ofVec2f collisionVect = (position - otherObject->position);
    collisionVect = collisionVect.length() == 0 ? ofVec2f(1,0) : collisionVect.normalized();
    
    // get components perpendicular to tangent
    float pt1i = velocity.dot(collisionVect);
    float pt2i = otherObject->velocity.dot(collisionVect);
    
    if (isAnchored){
        otherObject->velocity += -2*pt2i*collisionVect;
    }
    else if (otherObject->isAnchored){
        velocity = -2*pt1i*collisionVect;
    }
    else{
        float pt1f = ((pt1i * (mass - otherObject->mass)) + (2 * otherObject->mass * pt2i))/(mass + otherObject->mass);
        float pt2f = -pt1f*mass/otherObject->mass;
        
        velocity += (pt1f - pt1i)*collisionVect;
        otherObject->velocity += (pt2f - pt2i)*collisionVect;
    }
    
}

void PhysicsObject::move(float dTime)
{
    // Update position
    if (!isAnchored){
        velocity *= (1.0f - ambientFriction);
        lastPosition = position;
        position += velocity*dTime;
    }    
}

bool PhysicsObject::intersecting(PhysicsObject *otherObject)
{
    if (otherObject->boundingRadius == 0 || boundingRadius == 0) return false;
    return (boundingRadius + otherObject->boundingRadius - position.distance(otherObject->position)) >= 0;
}

bool PhysicsObject::passedThrough(PhysicsObject *otherObject)
{
    // see if our line of travel intersected the other object's bounding sphere
    bool pt = false;
    
    // we can't pass through anything if we're anchored or not solid
    if (!isAnchored & isSolid && otherObject->isSolid){
        
        // project distance from last point to other object onto path unit vector
        ofVec2f lastPosToObject = otherObject->getPosition() - lastPosition;
        ofVec2f pathVec = position - lastPosition;
        float projectionMag = lastPosToObject.dot(pathVec.normalized());
        if (projectionMag > 0 && projectionMag < pathVec.length() &&  pathVec.length() > 0){
            
            ofVec2f projVec = projectionMag * pathVec;
            float distToObj = lastPosToObject.distance(projVec);
            pt = (distToObj <= boundingRadius + otherObject->boundingRadius);
        }
    }
    
    return pt;
}

float PhysicsObject::deltaTimeSinceIntersection(PhysicsObject *otherObject, float dTime)
{
    // Intersection time interpolation
    // http://stackoverflow.com/questions/11369616/circle-circle-collision-prediction
    
    float rs = boundingRadius + otherObject->boundingRadius;
    rs *= rs;
    
    ofVec2f dV = velocity - otherObject->velocity;
    ofVec2f dP = lastPosition - otherObject->lastPosition;
    
    float A = (dV.x*dV.x + dV.y*dV.y);
    float B = 2.0f * (dV.x*dP.x + dV.y*dP.y);
    float C = dP.x*dP.x + dP.y*dP.y - rs;
    
    float sqrtOp = (B*B) - (4*A*C);
    if (sqrtOp > 0){
        sqrtOp = sqrtf(sqrtOp);
        float s1 = (-B + sqrtOp)/(2*A);
        float s2 = (-B - sqrtOp)/(2*A);
                
        float ans = MIN(s1,s2);
        return (ans >= 0.0 && ans <= dTime) ? ans : 0.0f;
    }
    else{
        return 0;
    }
}

#pragma mark - Setters and Getters

void PhysicsObject::setIsAnchored(bool _isAnchored)
{
    isAnchored = _isAnchored;
    setVelocity(ofVec2f());
}

void PhysicsObject::setIsSolid(bool _isSolid)
{
    isSolid = _isSolid;
}

void PhysicsObject::setMass(float newMass)
{
    mass = MAX(0, newMass);
}

void PhysicsObject::setPosition(ofVec2f newPosition)
{
    position = newPosition;
    lastPosition = newPosition;
}

void PhysicsObject::setVelocity(ofVec2f newVelocity)
{
    if (!isAnchored){
        velocity = newVelocity;
    }
}