#include "ofxGuiGraphics.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiGraphics::ofxGuiGraphics(){
	_bLoaded = false;
}

ofxGuiGraphics::~ofxGuiGraphics(){
}

ofxGuiGraphics::ofxGuiGraphics(const Config & config){
    setup(config.canvasName,config.graphics,config.shape.width,config.shape.height);
 }

ofxGuiGraphics & ofxGuiGraphics::setup(string canvasName, ofBaseDraws * graphics, float w, float h){
	if(graphics->getHeight() != 0 && graphics->getWidth() != 0){
		_bLoaded = true;
		setName(canvasName);
		this->graphics = graphics;
		setSize(w, h);
	}else{
        ofLogError("ofxGuiGraphics:setup()", "graphics cannot be loaded, width = 0 or height = 0");
	}

    return *this;
}

void ofxGuiGraphics::setShape(float x, float y, float w, float h){
    ofxBaseGui::setPosition(x,y);
    setSize(w,h);
}

void ofxGuiGraphics::setShape(ofRectangle r){
    ofxBaseGui::setPosition(r.x,r.y);
    setSize(r.width,r.height);
}

void ofxGuiGraphics::setSize(float w, float h){
	if(_bLoaded){
		if(w == 0){
			if(h == 0){
				w = graphics->getWidth();
				h = graphics->getHeight();
			}else{
				w = h * graphics->getWidth() / graphics->getHeight();
			}
		}
		h = w * graphics->getHeight() / graphics->getWidth();
		b.width = w;
		b.height = h;
        sizeChangedE.notify(this);
		setNeedsRedraw();
	}
}

void ofxGuiGraphics::generateDraw(){
	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

	if(bShowName){
		textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height - textPadding);
	}
}

void ofxGuiGraphics::render(){
	ofColor c = ofGetStyle().color;

	bg.draw();
	if(_bLoaded){
		graphics->draw(b);
	}

	if(bShowName){
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode != OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(textColor);

		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();

		ofSetColor(c);
		if(blendMode != OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
	}
}

ofAbstractParameter & ofxGuiGraphics::getParameter(){
	return label;
}
