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
    boundingRadius = _radius;
    mass = 0.001 * _radius * _radius;
    color = _color;
}

void DotPhysicsObject::draw()
{
    ofPushStyle();
    ofSetColor(color);
    ofCircle(position, boundingRadius);
    ofPopStyle();
}
