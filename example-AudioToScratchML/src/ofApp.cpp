#include "ofApp.h"

#include <sndfile.hh>

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	string filename = "Serato/Serato_CD.aif";// "TraktorMK2/Traktor_MK2_Scribble.wav";
	string absoluteFilename = ofToDataPath(filename, true);
	SndfileHandle myf = SndfileHandle(absoluteFilename.c_str());
	bufferFrames = myf.frames();
	int n = bufferFrames * myf.channels();
	floatBuffer.resize(n);
	curBuffer.resize(n);
	myf.read(&floatBuffer[0], n);
	
	bufferPosition = 0;
	ttmPosition = 0;
	relativePosition = 0;
	relativeTtm.resize(ofGetWidth());
	absoluteTtm.resize(ofGetWidth());
	pitchTtm.resize(ofGetWidth());
	
	string timecode = "serato_cd"; // "serato_cd" "serato_a" "traktor_a"
	float speed = 1.0; // 1.0 is 33 1/3, 1.35 is 45 rpm
	int sampleRate = 44100; // myf.samplerate()
	timecoder_init(&timecoder, timecode.c_str(), speed, sampleRate);
	//timecoder_monitor_init(&timecoder, MIXXX_VINYL_SCOPE_SIZE);
	
	bufferSize = 256;
	exporting = true;
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
}

void ofApp::exit() {
	timecoder_clear(&timecoder); // class
	timecoder_free_lookup(); // static
}

float ofApp::getPitch() {
	float pitch = timecoder_get_pitch(&timecoder);
	return pitch;
}

float ofApp::getRelative() {
	relativePosition += bufferSize * getPitch();
	return relativePosition;
}

float ofApp::getAbsolute() {
	float when;
	float pos = timecoder_get_position(&timecoder, &when);
	if(pos != -1) {
		absolutePosition = pos;
	} else {
		float pitch = getPitch();
		absolutePosition += (bufferSize / 45.7) * pitch; // not sure where the 45.7 comes from...
	}
	return absolutePosition;
}

void ofApp::update() {
}

void drawTtm(vector<float>& ttm, int position) {
	ofNoFill();
	ofBeginShape();
	float min, max;
	for(int i = 0; i < ttm.size(); i++) {
		if(i == 0 || ttm[i] < min) {
			min = ttm[i];
		}
		if(i == 0 || ttm[i] > max) {
			max = ttm[i];
		}
	}
	for(int i = 0; i < ttm.size(); i++) {
		int j = (i + position) % ttm.size();
		if(ttm[j] != -1) {
			ofVertex(i, ofMap(ttm[j], min, max, ofGetHeight(), 0));
		}
	}
	ofEndShape();
}

void ofApp::draw() {
	ofBackground(0);
	ofSetColor(255);
	drawTtm(relativeTtm, ttmPosition);
	drawTtm(absoluteTtm, ttmPosition);
	ofSetColor(ofColor::red);
	drawTtm(pitchTtm, ttmPosition);
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels) {
	float sum = 0;
	for (int i = 0; i < bufferSize; i++){
		output[i * nChannels] = floatBuffer[bufferPosition * nChannels];
		output[i * nChannels + 1] = floatBuffer[bufferPosition * nChannels + 1];
		
		curBuffer[i * nChannels] = output[i * nChannels] * (1<<15);
		curBuffer[i * nChannels + 1] = output[i * nChannels + 1] * (1<<15);
	
		if(i % 2 == 0) { // drop 96khz to 48khz
			bufferPosition++;
			if(bufferPosition == bufferFrames) {
				bufferPosition = 0;
				relativePosition = 0;
				exportXml();
			}
		}
	}
	
	timecoder_submit(&timecoder, &curBuffer[0], bufferSize);
	relativeTtm[ttmPosition] = getRelative();
	absoluteTtm[ttmPosition] = getAbsolute();
	if(exporting) {
		wholeBuffer.push_back(absoluteTtm[ttmPosition] );
	}
	pitchTtm[ttmPosition] = getPitch();
	ttmPosition++;
	if(ttmPosition == relativeTtm.size()) {
		ttmPosition = 0;
	}
}

void ofApp::exportXml() {
	ofFile out("out.xml", ofFile::WriteOnly);
	out.setf(ios::fixed, ios::floatfield);
	out.precision(2);
	for(int i = 0; i < wholeBuffer.size(); i++) {
		out << "<p>" << wholeBuffer[i] << "</p>";
	}
	exporting = false;
}