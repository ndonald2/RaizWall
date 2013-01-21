//
//  PhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
    isAnchored = false;
    isSolid = true;
    collisionMultiplier = 0.95f;
    boundingRadius = 1.0f;
    ambientFriction = 0.15f;
    mass = 10.0;
}

void PhysicsObject::handleCollisions(vector<PhysicsObject *> &otherObjects, float dTime)
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
                ofVec2f difference = otherObject->getPosition() - position;
                if (difference.length() == 0) difference = ofVec2f(1,0);
                float overlap = (boundingRadius + otherObject->getBoundingRadius() - position.distance(otherObject->getPosition()));

                difference.normalize();

                if (isAnchored){
                    otherObject->setPosition(overlap * difference);
                }
                else if (otherObject->getIsAnchored()){
                    position -= overlap * difference;
                }
                else{
                    position -= overlap * (difference/2.0f);
                    otherObject->setPosition(overlap * (difference/2.0f));
                }
            }
        }
    }
}

void PhysicsObject::move(float dTime, const ofVec2f & windowSize)
{
    // Update position
    if (!isAnchored){
        
        
        // Collide against walls
        if (position.x - boundingRadius < 0){
            velocity.x *= -collisionMultiplier;
            position.x = boundingRadius;
        }
        else if (position.x + boundingRadius > windowSize.x)
        {
            velocity.x *= -collisionMultiplier;
            position.x = windowSize.x - boundingRadius;
        }
        
        if (position.y - boundingRadius < 0){
            velocity.y *= -collisionMultiplier;
            position.y = boundingRadius;
        }
        else if (position.y + boundingRadius > windowSize.y){
            velocity.y *= -collisionMultiplier;
            position.y = windowSize.y - boundingRadius;
        }
        
        // calculate accel based on force
        ofVec2f acceleration = mass > 0 ? getForce()/mass : ofVec2f::zero();
        lastVelocity = velocity;
        velocity += acceleration*dTime;
        
        // apply friction (% loss per second)
        velocity *= powf(1.0f - ambientFriction, dTime);

        lastPosition = position;
        position += velocity*dTime;
        
        // reset force
        setForce(ofVec2f::zero());
    }    
}

bool PhysicsObject::intersecting(PhysicsObject *otherObject)
{
    if (otherObject->boundingRadius == 0 || boundingRadius == 0 || !isSolid || !otherObject->isSolid) return false;
    
    return (boundingRadius + otherObject->boundingRadius - position.distance(otherObject->position)) > 0;
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


void PhysicsObject::collide(PhysicsObject *otherObject, float dTime)
{
    
    // Assume perfectly elastic collisions
    // http://stackoverflow.com/questions/345838/ball-to-ball-collision-detection-and-handling
    
    ofVec2f uvVelocity = velocity.normalized();
    
    // get interpolation (how far along were we in the frame when we collided)
    float timeRatioOfCollision = deltaTimeSinceIntersection(otherObject, dTime)/dTime;
    
    if (!isAnchored){
        // back up one interpolated step for tangential collision
        position = lastPosition.getInterpolated(position, timeRatioOfCollision);
        velocity = lastVelocity.getInterpolated(velocity, timeRatioOfCollision);
    }
    
    if (!otherObject->isAnchored){
        // back other object one interpolated step
        otherObject->position = otherObject->lastPosition.getInterpolated(otherObject->position, timeRatioOfCollision);
        otherObject->velocity = otherObject->lastVelocity.getInterpolated(otherObject->velocity, timeRatioOfCollision);
    }
    
    ofVec2f collisionVect = (position - otherObject->position);
    collisionVect = collisionVect.length() == 0 ? ofVec2f(1,0) : collisionVect.normalized();
    
    // lossy collision
    // TODO: Make this a parameter
    velocity *= collisionMultiplier;
    otherObject->velocity *= otherObject->collisionMultiplier;
    
    // get components perpendicular to tangent
    float pt1i = velocity.dot(collisionVect);
    float pt2i = otherObject->velocity.dot(collisionVect);
    
    if (isAnchored && !otherObject->isAnchored){
        otherObject->velocity += -2*pt2i*collisionVect;
    }
    else if (!isAnchored && otherObject->isAnchored){
        velocity += -2*pt1i*collisionVect;
    }
    else{
        float pt1f = ((pt1i * (mass - otherObject->mass)) + (2 * otherObject->mass * pt2i))/(mass + otherObject->mass);
        float pt2f = -pt1f*mass/otherObject->mass;
        
        velocity += (pt1f - pt1i)*collisionVect;
        otherObject->velocity += (pt2f - pt2i)*collisionVect;
    }
    
}

#pragma mark - Forces

void PhysicsObject::setForce(const ofVec2f & newForce)
{
    forceMutex.lock();
    force = newForce;
    forceMutex.unlock();
}

void PhysicsObject::addForce(const ofVec2f & forceToAdd)
{
    forceMutex.lock();
    force += forceToAdd;
    forceMutex.unlock();
}

const ofVec2f PhysicsObject::getForce()
{
    forceMutex.lock();
    ofVec2f returnForce = force;
    forceMutex.unlock();
    return returnForce;
}

#pragma mark - Setters and Getters

void PhysicsObject::setIsAnchored(bool _isAnchored)
{
    setVelocity(ofVec2f());
    isAnchored = _isAnchored;
}

void PhysicsObject::setIsSolid(bool _isSolid)
{
    isSolid = _isSolid;
}

void PhysicsObject::setCollisionMultiplier(float multiplier)
{
    collisionMultiplier = multiplier;
}

void PhysicsObject::setMass(float newMass)
{
    mass = MAX(0, newMass);
}

void PhysicsObject::setAmbientFriction(float newAmbientFriction)
{
    ambientFriction = CLAMP(newAmbientFriction, 0.0, 1.0);
}

void PhysicsObject::setPosition(const ofVec2f & newPosition)
{
    position = newPosition;
    lastPosition = newPosition;
}

void PhysicsObject::setVelocity(const ofVec2f & newVelocity)
{
    if (!isAnchored){
        velocity = newVelocity;
        lastVelocity = velocity;
    }
}

#pragma mark - Active Physics Objects

void ActivePhysicsObject::applyForces(vector<PhysicsObject*> & otherObjects, float dTime)
{
    for (int i=0; i<otherObjects.size(); i++){
        
        PhysicsObject *otherObject = otherObjects[i];
        if (otherObject == this) continue;
        
        // Calculate and update forces
        if (typeid(*otherObject) == typeid(ActivePhysicsObject)){
            ofVec2f forceToMe = ((ActivePhysicsObject*)otherObject)->forceAppliedTo(this, dTime);
            addForce(forceToMe);
        }
        ofVec2f forceToOther = forceAppliedTo(otherObject, dTime);
        otherObject->addForce(forceToOther);
        
    }

}