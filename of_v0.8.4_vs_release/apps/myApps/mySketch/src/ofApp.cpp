#include "ofApp.h"

void ofApp::setup()
{


	
	camWidth 		= 640;	// try to grab at this size. 
	camHeight 		= 480;
	
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight);
	
	videoInverted.allocate(camWidth,camHeight,OF_PIXELS_RGB);
	videoTexture.allocate(videoInverted);
	ofSetVerticalSync(true);

	canvas.allocate(camWidth, camHeight);

    ofSetWindowShape(camWidth, camHeight);

    psBlend.setup(camWidth, camHeight);
    blendMode = 0;
}

void ofApp::update()
{	
	vidGrabber.update();
	
	if (vidGrabber.isFrameNew()){
		/*ofPixelsRef pixels = vidGrabber.getPixelsRef();
		for (int i = 0; i < pixels.size(); i++){
			videoInverted[i] = 255 - pixels[i];
		}
		videoTexture.loadData(videoInverted);
		*/
		psBlend.begin();
		canvas.draw(0,0);
		psBlend.end();
	}
}

void ofApp::draw()
{
	canvas.begin();
    psBlend.draw(vidGrabber.getTextureReference(), blendMode);
	canvas.end();

	canvas.draw(0, 0);

    ofSetWindowTitle("blendMode: " + psBlend.getBlendMode(blendMode));
    ofDrawBitmapString("press UP/DOWN key", 10, 20);

	//ofSetHexColor(0xffffff);
//	vidGrabber.draw(20,20);
//	videoTexture.draw(20+camWidth,20,camWidth,camHeight);
}

void ofApp::keyPressed(int key)
{
    if (key == OF_KEY_UP)
    {
        if (blendMode >= 24)
        {
            blendMode = 0;
        }
        else {
            blendMode++;
        }
    }
    if (key == OF_KEY_DOWN)
    {
        if (blendMode <= 0)
        {
            blendMode = 24;
        }
        else
        {
            blendMode--;
        }
    }
    if (key == ' ')
    {
        ofSaveFrame();
    }
	if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
	}
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}