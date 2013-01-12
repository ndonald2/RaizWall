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
                float overlap = -(position.distance(otherDot->position) - radius - otherDot->getRadius());

                if (overlap >= 0)
                {
                    // unit vector between centers (line of collision)
                    ofVec2f collisionVect = (otherDot->position - position).normalized();
                    
                    // split the difference between overlap
                    position -= collisionVect * (overlap/2.0f);
                    
                    // anchored object has "infinite" mass and we can treat it separately
                    if (otherObject->isAnchored){

                        
                        float halfRefAngle = velocity.angle(collisionVect);
                        velocity = -(velocity.rotate(2.0*halfRefAngle));
                        
                    }                    
                    else{
                        
                        // split the difference between overlap
                        otherObject->position += collisionVect * (overlap/2.0f);
                    
                
                        // reflect velocity about tangent to collisionVect
                        float halfReflAngle = collisionVect.angle(velocity);
                    
                    
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