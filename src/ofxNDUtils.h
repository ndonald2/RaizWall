//
//  ofxNDUtils.h
//
//  Created by Nick Donaldson on 11/11/12.
//
//

#pragma once

#include "ofMain.h"

/// HSB Color representation
/// Created to circumvent ofColor's resetting of hue/sat when changing to full black or white
class ofxNDHSBColor {
    
public:
    ofxNDHSBColor(float _h = 0, float _s = 0, float _b = 0, float _a = 255);
    ofColor getOfColor();
    
    float h;
    float s;
    float b;
    float a;
};

// Billboard rectangle - rect for displaying a texture. Draws vertices and ARB tex coords.
extern void ofxNDBillboardRect(int x, int y, int w, int h, int tw, int th);

// Circular gradient
extern void ofxNDCircularGradient(float radius, const ofColor & start, const ofColor & end);


// Ms to time constant for one pole filter
float msToOnePoleTC(float ms);