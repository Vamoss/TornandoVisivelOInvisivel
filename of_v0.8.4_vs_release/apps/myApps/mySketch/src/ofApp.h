#pragma once

#include "ofMain.h"
#include "ofxPSBlend.h"
#include "ofxQTVideoSaver.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp
{
public:
    ~ofApp();
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
	void save();

	ofxUISuperCanvas *gui1;
	void guiEvent(ofxUIEventArgs &e);
    
    int					maxDuration;
	float				duration;

	ofxPSBlend			psBlend;
    ofImage				base;
    ofImage				target;
    int					blendMode;
	vector<string>		blendNames;
	
	float				mixRate;
	float				cleanRate;
		
	ofVideoGrabber 		vidGrabber;
	ofPixels		 	videoInverted;
	ofTexture			videoTexture;
	int 				camWidth;
	int 				camHeight;
	
    ofxQtVideoSaver     vidSaver;
	float				waitToStartRecording;

	int					bufferSize;
	vector<ofPixels>	buffer;
	int					bufferIndex;
	int					recordedFrames;

	ofFbo canvas;

	enum modes{WAITING, RECORDING, SAVING, SCREENSAVER};
	modes mode;
};
