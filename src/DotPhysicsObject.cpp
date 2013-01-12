//
//  DotPhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "DotPhysicsObject.h"

DotPhysicsObject::DotPhysicsObject(float _radius, ofColor _color)
{
    setRadius(_radius);
    setColor(_color);
}

void DotPhysicsObject::resolveCollisions(vector<PhysicsObject*> & otherObjects, float dTime)
{
    // Assume perfectly elastic collisions
    
    if (!isAnchored){
        
        for (int i=0; i<otherObjects.size(); i++){
            
            PhysicsObject* otherObject = otherObjects[i];
            if (otherObject == this) continue;
            
            // TODO: Find largest overlap, resolve that one (priority of collisions)
//            vector<PhysicsObject*> overlappingObjects;
//            vector<float> overlap;
        
            if (typeid(*otherObject) == typeid(DotPhysicsObject))
            {            
                DotPhysicsObject *otherDot = (DotPhysicsObject*)otherObject;
                
                float overlap = (position.distance(otherDot->position) - radius - otherDot->getRadius());
                ofVec2f uvVelocity = velocity.normalized();
                
                if (overlap < 0)
                {
                    // unit vector between centers (line of collision)
                    ofVec2f collisionVect = (otherDot->position - position).normalized();
                    float halfRefAngle = velocity.angle(collisionVect);
                    
                    // get interpolation (how far along were we in the frame when we collided)
                    float fInterp = intersectionFactor(otherDot, dTime);
                    
                    // back up one interpolated step
                    position -= velocity * dTime * (1.0f -fInterp);
                    
                    // anchored object has "infinite" mass and we can treat it separately
                    if (otherObject->isAnchored){

                        velocity = -(velocity.rotate(2.0*halfRefAngle));
                        
                    }                    
                    else{
                        
                        // back other object one interpolated step
                        otherObject->position -= otherObject->velocity * dTime * (1.0 -fInterp);
                        
                    
                    
                    }

                }
            }
        }
        
        // Collide against walls
        if (position.x - radius < 0){
            velocity.x *= -1;
            position.x = radius;
        }
        else if (position.x + radius > ofGetWidth())
        {
            velocity.x *= -1;
            position.x = ofGetWidth() - radius;
        }
        
        if (position.y - radius < 0){
            velocity.y *= -1;
            position.y = radius;
        }
        else if (position.y + radius > ofGetHeight()){
            velocity.y *= -1;
            position.y = ofGetHeight() - radius;
        }
        
    }
}


void DotPhysicsObject::draw()
{
    ofSetColor(color);
    ofCircle(position, radius);
    
    stringstream ss;
    ss << "velocity : " << velocity.x << " x " << velocity.y << endl;
    ofLog(OF_LOG_NOTICE, ss.str());
}

void DotPhysicsObject::setRadius(float _radius)
{
    radius = _radius;
    mass = (4.0/3.0)*M_PI*powf(_radius,3.0);
}

void DotPhysicsObject::setColor(ofColor _color)
{
    color = _color;
}

float DotPhysicsObject::intersectionFactor(DotPhysicsObject *otherObject, float dTime)
{
    // Intersection time interpolation
    // http://stackoverflow.com/questions/11369616/circle-circle-collision-prediction
    
    float rs = radius + otherObject->radius;
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









