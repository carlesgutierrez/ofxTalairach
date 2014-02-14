#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	myTalairach.setup("../../../data/talairach.jar");
	locBrain= ofVec3f(0,0,0);
	cubeSize=3;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	myTalairach.drawDebug(100, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
		case '1':
			myTalairach.getStructuralProbMap(locBrain); //TODO local true don return results...
			break;
		case '2':
			myTalairach.getLabels(locBrain);
			break;
		case '3':
			myTalairach.getLabelsArroundCube(locBrain, cubeSize); //15, 10, 8
			break;
		case OF_KEY_UP:
			cubeSize += 2;
			if (cubeSize > 11)cubeSize = 11;
			myTalairach.setSizeCube(cubeSize);
			break;
		case OF_KEY_DOWN:
			cubeSize -= 2;
			if (cubeSize < 3)cubeSize = 3;
			myTalairach.setSizeCube(cubeSize);
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	//Map Mouse position and search X, Y around 0 and 200
	locBrain.x = ofMap(x, 0, ofGetWidth(), 0, 200, false);
	locBrain.y = ofMap(y, 0, ofGetWidth(), 0, 200, false);
	locBrain.z = 8;//ofMap(mouseY, 0, ofGetWidth(), 0, 200, true);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
