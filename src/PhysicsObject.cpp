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
    boundingRadius = 1.0f;
    ambientFriction = 0.001f;
    mass = 10.0;
    velocity = ofVec2f();
    position = ofVec2f();
}

void PhysicsObject::update(vector<PhysicsObject*> & otherObjects, float dTime)
{    
    for (int i=0; i<otherObjects.size(); i++){
        
        PhysicsObject *otherObject = otherObjects[i];
        
        // handle collision
        if (intersecting(otherObject)){
            collide(otherObject, dTime);
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
    float fInterp = intersectionFactor(otherObject, dTime);
    
    if (!isAnchored){
        // back up one interpolated step for tangential collision
        position -= velocity * dTime * (1.0f -fInterp);
    }
    
    if (!otherObject->isAnchored){
        // back other object one interpolated step
        otherObject->position -= otherObject->velocity * dTime * (1.0 -fInterp);
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
        position += velocity*dTime;
    }    
}

bool PhysicsObject::intersecting(PhysicsObject *otherObject)
{
    float overlap = (position.distance(otherObject->position) - boundingRadius - otherObject->boundingRadius);
    return overlap <= 0;
}


float PhysicsObject::intersectionFactor(PhysicsObject *otherObject, float dTime)
{
    // Intersection time interpolation
    // http://stackoverflow.com/questions/11369616/circle-circle-collision-prediction
    
    float rs = boundingRadius + otherObject->boundingRadius;
    rs *= rs;
    
    ofVec2f lastPos = position - velocity*dTime;
    ofVec2f otherLastPos = otherObject->isAnchored ? otherObject->position : otherObject->position - otherObject->velocity*dTime;
    
    ofVec2f dV = velocity - otherObject->velocity;
    ofVec2f dP = lastPos - otherLastPos;
    
    float A = (dV.x*dV.x + dV.y*dV.y);
    float B = 2.0f * (dV.x*dP.x + dV.y*dP.y);
    float C = dP.x*dP.x + dP.y*dP.y - rs;
    
    float sqrtOp = (B*B) - (4*A*C);
    if (sqrtOp > 0){
        sqrtOp = sqrtf(sqrtOp);
        float s1 = (-B + sqrtOp)/(2*A);
        float s2 = (-B - sqrtOp)/(2*A);
        
        float ans = s1 <= dTime ? s1 : (s2 <= dTime ? s2 : 0);
        return ans/dTime;
    }
    else{
        return 0;
    }
}