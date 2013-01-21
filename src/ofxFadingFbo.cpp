//
//  ofxFadingFbo.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/21/13.
//
//

#include "ofxFadingFbo.h"
#include "ofxNDUtils.h"

void ofxFadingFbo::allocate(int width, int height, float fade_ms)
{
    _alphaFadeCoef = msToOnePoleTC(fade_ms);
    _colorFadeCoef = 1.0f;
    
    ofFbo::Settings fboSettings;
    fboSettings.width = width;
    fboSettings.height = height;
    fboSettings.useDepth = false;
    fboSettings.useStencil = false;
    fboSettings.depthStencilAsTexture = false;
    fboSettings.numColorbuffers = 2;
    fboSettings.internalformat = GL_RGBA;
    
    ofFbo::allocate(fboSettings);
    
    _fadeShader.load("shaders/vanilla.vert", "shaders/trails.frag");
}

void ofxFadingFbo::begin()
{
    ofDisableBlendMode();
    ofSetColor(255,255,255);
    ofFill();
    
    ofFbo::begin();
    setActiveDrawBuffer(0);
    ofClear(0,0,0,0);
    
    ofTexture & fadingTex = getTextureReference(1);
    
    _fadeShader.begin();
    _fadeShader.setUniformTexture("texSampler", fadingTex, 1);
    _fadeShader.setUniform1f("alphaDecay", _alphaFadeCoef);
    _fadeShader.setUniform1f("colorDecay", _colorFadeCoef);
    _fadeShader.setUniform1f("alphaMin", 0.08);
    int w = getWidth();
    int h = getHeight();
    ofxNDBillboardRect(0, 0, w, h, w, h);
    _fadeShader.end();
}

void ofxFadingFbo::end()
{
    ofDisableBlendMode();
    setActiveDrawBuffer(1);
    ofClear(0,0,0,0);
    getTextureReference(0).draw(0,0);
    ofFbo::end();
}

void ofxFadingFbo::draw(float x, float y)
{
    draw(x, y, getWidth(), getHeight());
}

void ofxFadingFbo::draw(float x, float y, float width, float height)
{
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofTexture & trailTex = getTextureReference(0);
    trailTex.draw(x,y,width,height);
}

void ofxFadingFbo::setAlphaFadeMs(float ms)
{
    _alphaFadeCoef = msToOnePoleTC(ms);
}

void ofxFadingFbo::setColorFadeMs(float ms)
{
    _colorFadeCoef = msToOnePoleTC(ms);
}