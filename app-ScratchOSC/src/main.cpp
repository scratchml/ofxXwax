#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 768, 256, OF_WINDOW);
	ofRunApp(new ofApp());
}
