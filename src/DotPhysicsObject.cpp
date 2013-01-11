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

void DotPhysicsObject::bounce()
{
    // modified bounce
    
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