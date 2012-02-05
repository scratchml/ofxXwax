#include "ofApp.h"

static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);
static const int maxData = 512;

void ofApp::loadSettings() {
	ofxXmlSettings xml;
	xml.loadFile("settings.xml");
	
	xml.pushTag("record");
	recordFormat = xml.getValue("format", "serato_2a");
	recordSide = xml.getValue("side", "right");
	xml.popTag();
	
	xml.pushTag("osc");
	oscHost = xml.getValue("host", "");
	oscPort = xml.getValue("port", 0);
	oscSubdivide = xml.getValue("subdivide", 8);
	oscPitch = xml.getValue("pitch", 0);
	oscRelative = xml.getValue("relative", 0);
	oscDegrees = xml.getValue("degrees", 0);
	xml.popTag();
	
	xml.pushTag("audio");
	audioSamplerate = xml.getValue("samplerate", 96000);
	audioBuffersize = xml.getValue("buffersize", 128);
	xml.popTag();
	
	xml.pushTag("serial");
	serialPort = xml.getValue("port", "");
	serialThreshold = xml.getValue("threshold", 0);
	xml.popTag();
}

void ofApp::setup() {
	ofSetWindowTitle("ScratchOSC");
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofSetCircleResolution(50);
	ofSetLineWidth(2);
	
	loadSettings();
	oscRate = (audioSamplerate / audioBuffersize) / oscSubdivide;
	
	xwax.setup(audioSamplerate, audioBuffersize, recordFormat);
	ofSoundStreamSetup(0, 2, audioSamplerate, audioBuffersize, 1);
	
	osc.setup(oscHost, oscPort);
	
	serialReady = serial.setup(serialPort, 115200);
	
	middleAudioBuffer.resize(audioBuffersize * 2);
	frontAudioBuffer.resize(audioBuffersize * 2);
	audioFrame = 0;
}  

void ofApp::update() {
	audioMutex.lock();
	frontAudioBuffer = middleAudioBuffer;
	audioMutex.unlock();
}

void ofApp::draw() {
	ofBackground(0);
	
	ofPushMatrix();
	ofTranslate(10, 10);
	drawAudioInput(0, 0, 128);
	drawVinyl(128, 0, 128);
	
	ofTranslate(256, 0);
	ofSetColor(yellowPrint);
	drawCurve(absolutePosition, 128);
	ofSetColor(magentaPrint);
	drawCurve(relativePosition, 128);
	ofSetColor(255);
	drawCurve(faderPosition, 128, 0, 255);
	ofPopMatrix();
	
	ofSetColor(255);
	
	ofPushMatrix();
	ofTranslate(10, ofGetHeight() - 120);
	ofDrawBitmapString("pitch: " + ofToString(xwax.getPitch(), 2) + "x", 0, 20);
	ofDrawBitmapString("velocity: " + ofToString(xwax.getVelocity(), 2) + " ms", 0, 40);
	ofDrawBitmapString("relative: " + ofToString(xwax.getRelative(), 0) + " ms", 0, 60);
	ofDrawBitmapString("absolute: " + ofToString(xwax.getAbsolute(), 0) + " ms", 0, 80);
	ofDrawBitmapString("absolute valid: " + ofToString(xwax.isAbsoluteValid() ? "yes" : "no"), 0, 100);
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(256, ofGetHeight() - 120);
	ofDrawBitmapString("format: " + xwax.getFormat() + " (" + recordSide +")", 0, 20);
	ofDrawBitmapString("osc: " + oscHost + ":" + ofToString(oscPort), 0, 40);
	ofDrawBitmapString("subdivide: " + ofToString(oscSubdivide) + " (" + ofToString(oscRate, 1) + "Hz)", 0, 60);
	ofDrawBitmapString("audio: " + ofToString(audioSamplerate) + "Hz, " + ofToString(audioBuffersize) + " samples", 0, 80);
	ofDrawBitmapString("serial: " + ofToString(serialPort), 0, 100);
	
	ofPopMatrix();
}

void ofApp::drawAudioInput(float x, float y, float scale) {
	scale /= 4;
	int resolution = MAX(audioBuffersize / scale, 1);
	
	ofPushStyle();
	
	ofPushMatrix();
	ofTranslate(x + scale, y + scale);
	ofScale(scale, scale);
	
	ofNoFill();
	ofSetColor(255);
	ofRect(-1, -1, 2, 2);
	ofBeginShape();
	for(int i = 0; i < audioBuffersize; i += resolution) {
		ofVertex(frontAudioBuffer[i * 2], frontAudioBuffer[i * 2 + 1]);
	}
	ofEndShape();
	
	ofPushMatrix();
	ofSetColor(magentaPrint);
	ofTranslate(1, 0);
	ofScale(2. / audioBuffersize, 1);
	ofBeginShape();
	for(int i = 0; i < audioBuffersize; i += resolution) {
		ofVertex(i, frontAudioBuffer[i * 2]);
	}
	ofEndShape();
	ofPopMatrix();
	
	ofPushMatrix();
	ofSetColor(cyanPrint);
	ofTranslate(0, 1);
	ofScale(1, 2. / audioBuffersize);
	ofBeginShape();
	for(int i = 0; i < audioBuffersize; i += resolution) {
		ofVertex(frontAudioBuffer[i * 2 + 1], i);
	}
	ofEndShape();
	ofPopMatrix();
	
	ofPopMatrix();
	ofPopStyle();
}

void ofApp::drawVinyl(float x, float y, float scale) {
	scale /= 2;
	ofPushStyle();
	ofPushMatrix();
	
	ofTranslate(x, y);
	ofScale(scale, scale);
	ofTranslate(1, 1);
	ofNoFill();
	ofSetColor(255);
	ofCircle(0, 0, 1);
	
	ofPushMatrix();
	ofSetColor(yellowPrint);
	ofRotate(ofxXwax::millisToDegrees(xwax.getAbsolute()));
	ofLine(0, 0, 0, 1);
	ofPopMatrix();
	
	ofPushMatrix();
	ofSetColor(magentaPrint);
	ofRotate(ofxXwax::millisToDegrees(xwax.getRelative()));
	ofLine(0, 0, 0, 1);
	ofPopMatrix();
	
	ofPopMatrix();
	ofPopStyle();
}

void ofApp::drawCurve(deque<float> curve, float scale, float min, float max) {
	if(min == 0 && max == 0) {
		for(int i = 0; i < curve.size(); i++) {
			if(i == 0 || curve[i] < min) {
				min = curve[i];
			}
			if(i == 0 || curve[i] > max) {
				max = curve[i];
			}
		}
	}
	if(fabs(min - max) > FLT_EPSILON) {
		ofNoFill();
		ofBeginShape();
		for(int i = 0; i < curve.size(); i++) {
			float y = ofMap(curve[i], min, max, scale, 0);
			ofVertex(i, y);
			if(i == curve.size() - 1) {
				ofDrawBitmapString(ofToString(curve.back()), i - 80, y);
			}
		}
		ofEndShape();
	}
}

void ofApp::audioIn(float* input, int audioBuffersize, int nChannels) {
	xwax.update(input);
	
	audioMutex.lock();
	middleAudioBuffer.assign(input, input + audioBuffersize * nChannels);
	audioMutex.unlock();
	
	absolutePosition.push_back(xwax.getAbsolute());
	relativePosition.push_back(xwax.getRelative());
	if(absolutePosition.size() > maxData) {
		absolutePosition.pop_front();
	}
	if(relativePosition.size() > maxData) {
		relativePosition.pop_front();
	}
	
	if(audioFrame % oscSubdivide == 0) {
		float position = oscRelative ? xwax.getRelative() : xwax.getAbsolute();
		ofxOscMessage msg;
		msg.setAddress("/scratch/record/" + recordSide);
		msg.addFloatArg(position);
		osc.sendMessage(msg);
		if(oscPitch) {
			ofxOscMessage msg;
			msg.setAddress("/scratch/record/" + recordSide + "/pitch");
			msg.addFloatArg(xwax.getPitch());
			osc.sendMessage(msg);
		}
		if(oscDegrees) {
			ofxOscMessage msg;
			msg.setAddress("/scratch/record/" + recordSide + "/degrees");
			msg.addFloatArg(fmodf(xwax.millisToDegrees(position), 360));
			osc.sendMessage(msg);
		}
	}
	audioFrame++;
	
	if(serialReady) {
		unsigned char faderData;
		int nRead;
		while ((nRead = serial.readBytes(&faderData, 1)) > 0) {
			bool prevOverThreshold = overThreshold;
			overThreshold = faderData > serialThreshold;
			if(prevOverThreshold != overThreshold) {
				ofxOscMessage msg;
				msg.setAddress("/scratch/mixer/fader");
				msg.addFloatArg(overThreshold ? 1 : 0);
				osc.sendMessage(msg);
			}
			faderPosition.push_back(faderData);
			if(faderPosition.size() > maxData) {
				faderPosition.pop_front();
			}
		}
	}
}