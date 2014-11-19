#include "ofApp.h"
//--------------------------------------------------------------
ofApp::~ofApp(){
}

void ofApp::setup()
{
	ofFile file; 
	file.open("settings.xml");
	ofBuffer fileBuffer = file.readToBuffer();
	ofXml settings;
	settings.loadFromBuffer( fileBuffer.getText() );
	settings.setToParent();
	int xx = ofToInt(settings.getValue("settings/window/x"));
	int yy = ofToInt(settings.getValue("settings/window/y"));
	int ww = ofToInt(settings.getValue("settings/window/width"));
	int hh = ofToInt(settings.getValue("settings/window/height"));
	int fps = ofToInt(settings.getValue("settings/window/fps"));

	camWidth = ofToInt(settings.getValue("settings/capture/width"));
	camHeight = ofToInt(settings.getValue("settings/capture/height"));
	maxDuration = ofToInt(settings.getValue("settings/capture/maxDuration"));
	
	ofSetVerticalSync(true);
    ofEnableAlphaBlending(); 
	ofSetWindowPosition(xx, yy);
	ofSetWindowShape(ww, hh);
	ofSetFrameRate(fps);
	
	gui1 = new ofxUISuperCanvas("TORNANDO VISIVEL O INVISIVEL", 10, 10, 200, 300);
    gui1->addSpacer();
    gui1->addFPS();
	gui1->addSlider("DURATION", 1.0, maxDuration, &duration);
	gui1->addSlider("MIX RATE", 0.0, 255.0, &mixRate);
	gui1->addSlider("CLEAN RATE", 0.0, 255.0, &cleanRate);
	gui1->addLabelButton("CAM SETTINGS", false);
	blendNames.push_back("Normal");
	blendNames.push_back("Multiply");
	blendNames.push_back("Average");
	blendNames.push_back("Add");
	blendNames.push_back("Substract");
	blendNames.push_back("Difference");
	blendNames.push_back("Negation");
	blendNames.push_back("Exclusion");
	blendNames.push_back("Screen");
	blendNames.push_back("Overlay");
	blendNames.push_back("SoftLight");
	blendNames.push_back("HardLight");
	blendNames.push_back("ColorDodge");
	blendNames.push_back("LinearLight");
	blendNames.push_back("VividLight");
	blendNames.push_back("PinLight");
	blendNames.push_back("HardMix");
	blendNames.push_back("Reflect");
	blendNames.push_back("Glow");
	blendNames.push_back("Phoenix");
	blendNames.push_back("Hue");
	blendNames.push_back("Saturation");
	blendNames.push_back("Color");
	blendNames.push_back("Luminosity");
	gui1->addDropDownList("BLEND", blendNames)->activateToggle("Screen");;
	gui1->addSpacer();
	gui1->addLabelButton("SAVE", false);
	ofAddListener(gui1->newGUIEvent,this,&ofApp::guiEvent);

	gui1->loadSettings("guiSettings.xml");
    

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(fps);
	vidGrabber.initGrabber(camWidth,camHeight);

	vidSaver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);

	bufferSize = maxDuration*fps;
	for(int i=0; i<bufferSize; i++){
		buffer.push_back(ofPixels());
	}


	canvas.allocate(camWidth, camHeight, GL_RGB);

    psBlend.setup(camWidth, camHeight);
    blendMode = 0;

	mode = WAITING;
}

void ofApp::update()
{	
	vidGrabber.update();
	
	if (vidGrabber.isFrameNew()){
		ofPushStyle();
			psBlend.begin();
				ofSetColor(0, cleanRate);
				ofRect(0,0,camWidth,camHeight);
				ofSetColor(255, mixRate);
				canvas.draw(0,0);
			psBlend.end();
		ofPopStyle();
	}
}

void ofApp::draw()
{
	ofPushStyle();
		canvas.begin();
			psBlend.draw(vidGrabber.getTextureReference(), blendMode);
		canvas.end();
	ofPopStyle();

	ofPushMatrix();
		ofScale(-1, 1);
		canvas.draw(-ofGetWidth(), 0, ofGetWidth(), ofGetHeight());
	ofPopMatrix();

	waitToStartRecording++;
	if(mode==RECORDING && waitToStartRecording > 120){
		canvas.readToPixels(buffer[bufferIndex]);
		bufferIndex++;
		recordedFrames = bufferIndex;

		if(bufferIndex>=bufferSize || bufferIndex>=duration*ofGetFrameRate()){
			save();
		}
	}

	ofSetWindowTitle("blendMode: " + psBlend.getBlendMode(blendMode) + " FPS: " + ofToString(ofGetFrameRate()));
}

void ofApp::keyPressed(int key)
{
	if (key == 'f')
    {
		ofToggleFullscreen();
	}
	else if (key == 'p')
    {
		gui1->toggleVisible();
	}
	else if (key == OF_KEY_UP)
    {
        if (blendMode >= 24)
        {
            blendMode = 0;
        }
        else {
            blendMode++;
        }
    }
    else if (key == OF_KEY_DOWN)
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
    else if (key == ' ')
    {
		switch(mode){
			case WAITING:
				mode = RECORDING;
				waitToStartRecording = 0;
				bufferIndex = 0;
				recordedFrames = 0;
				break;
			case RECORDING:
			case SAVING:
				save();
				break;
		}
    }
	else if (key == 's' || key == 'S'){
		gui1->saveSettings("guiSettings.xml");
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

void ofApp::save()
{
	mode = SAVING;

	string fileName = "TVI-"+ofGetTimestampString();
	ofSaveScreen("photos/"+fileName+".png");
				
	vidSaver.setup(camWidth, camHeight, "videos/"+fileName+".mov");
	for(int i=0; i<recordedFrames; i++){
		vidSaver.addFrame(buffer[i].getPixels(), 0.03333333333f);
	}
	vidSaver.finishMovie();
	
	mode = WAITING;
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	int kind = e.getKind();
	if(name=="CAM SETTINGS"){
		vidGrabber.videoSettings();
	}else if(name=="SAVE"){
		gui1->saveSettings("guiSettings.xml");
	}else if(name=="BLEND"){
		ofxUIDropDownList *radio = (ofxUIDropDownList *) e.widget;
		vector<int> selectedIndices = radio->getSelectedIndeces();
		if(selectedIndices.size()>0){
			blendMode = selectedIndices[0];
		}
	}
}