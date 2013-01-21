//
//  ofxFadingFbo.h
//  raizWall
//
//  Created by Nick Donaldson on 1/21/13.
//
//

#ifndef __raizWall__ofxFadingFbo__
#define __raizWall__ofxFadingFbo__

#include "ofMain.h"

class ofxFadingFbo : public ofFbo
{
public:
    
    void allocate(int width, int height, float fade_ms = 100);
    void begin();
    void end();
    void draw(float x, float y);
    void draw(float x, float y, float width, float height);
    
    void setAlphaFadeMs(float ms);
    void setColorFadeMs(float ms);
    
private:
    
    ofShader    _fadeShader;
    
    float       _alphaFadeCoef;
    float       _colorFadeCoef;
};

#endif /* defined(__raizWall__ofxFadingFbo__) */
