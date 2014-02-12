/*
 *  ofxTalairach.h
 *  talairachApp
 *
 *  Created by Carles on 11/02/14.
 *  Copyright 2014 CarlesGutierrez. All rights reserved.
 *
 */

#include "ofxTalairach.h"


//------------------------------------------------------------------------------
ofxTalairach::ofxTalairach(){
	cubeSize = 3; // Sizes of 3, 5, 7, 9 and 11 are accepted.
	point3d = ofVec3f(0,0,0);
	bConnected = false;
	vectorLabels.clear();
}

//------------------------------------------------------------------------------
ofxTalairach::~ofxTalairach(){
	
}

//------------------------------------------------------------------------------
void ofxTalairach::setup(){
	//connectT(); //TODO problems with connection, 
}

//------------------------------------------------------------------------------
void ofxTalairach::drawDebug(int x, int y){
	
	ofDrawBitmapStringHighlight("Press '1','2', or '3' to get a sample with diferent data from The Talairach Client", x, y);
	ofDrawBitmapStringHighlight("Press '+' or '-' for change the cubeSize request", x, y+MARGINTEXT);
	
	ofSetColor(10, 10, 10);
	drawRequestProbMap(x, y+MARGINTEXT*3);
	
	ofSetColor(10, 10, 10);
	drawRequestedLabels(x,y+MARGINTEXT+100);

}


//--------------------------------------------------------------
void ofxTalairach::connectT(){
	//a check for a jar File Exitance
	if(existsFile("../../../data/talairach.jar")){ // TODO This only will work for OSX systems
		commnadline = "java -cp ../../../data/talairach.jar org.talairach.AtlasServer 1600";
		system(commnadline.c_str());
		bConnected = true; //TODO better to read buffer output to check connection message
	}else {
		cout << "ofxTalairach Error :: talairach.jar does not exist in bin/data folder" << endl;
	}

}

//--------------------------------------------------------------
void ofxTalairach::drawRequestProbMap(int x, int y){

	ofDrawBitmapStringHighlight("Last requested Probability Map: " +ofToString(point3d), x, y);
	ofDrawBitmapString(ofToString(probMap,2), x, y+MARGINTEXT);
}

//--------------------------------------------------------------
void ofxTalairach::drawRequestedLabels(int x, int y){
	
	ofDrawBitmapStringHighlight("Last requested Point: " +ofToString(point3d)+ " cubeSize= "+ ofToString(cubeSize) , x, y);
	
	for (int i=0; i<vectorLabels.size(); i++) {
		ofDrawBitmapString(vectorLabels[i], x, i*MARGINTEXT + y + MARGINTEXT*2);
	}
}

/*
void ofxTalairach::setSizeCube(int _sizecube){
	//Sizes of 3, 5, 7, 9 and 11 are accepted.
	if(_sizecube == 3 || _sizecube == 5 || _sizecube == 7 || _sizecube == 9 || _sizecube == 11){
		cubeSize = _sizecube;
	}
	else {
		cout << "ofxTalairach:: Set a cubeSize error. Only 3, 5, 7, 9 and 11 are accepted" << endl;
	}

}
 */

//GETTERS
//--------------------------------------------------------------
float ofxTalairach::getStructuralProbMap(ofVec3f pos){
	
	point3d = ofVec3f(15, 10, 12);
	commnadline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD 1,";
	commnadline += " 15, 10, 12";
	
	probMap = requestSPM(commnadline);//Structural Probability Maps request
	
	return probMap;
}
//--------------------------------------------------------------
vector<string> ofxTalairach::getLabelsArroundCube(ofVec3f pos, int _cubeSize){
	//Save last data used to request 
	point3d = pos;
	cubeSize = _cubeSize;
	
	commnadline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD 3:";
	string cubeSizeStr = " "+ofToString(cubeSize,0)+",";
	//commnadline += " 7,";// To set a different cube size, use "3:<cubesize>". Sizes of 3, 5, 7, 9 and 11 are accepted.
	commnadline += cubeSizeStr;
	
	string cubePointStr = " "+ofToString(point3d.x,0)+", "+ofToString(point3d.y,0)+", "+ofToString(point3d.z,0);
	//commnadline += " 15, 10, 8";
	commnadline += cubePointStr;
	requestTL(commnadline);
	
	return vectorLabels;
}




//--------------------------------------------------------------
void ofxTalairach::requestTL(string commandline){
	
	vectorLabels.clear();
	
	FILE *fp = popen(commandline.c_str(), "r");
	
	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), fp))
	{
		std::cout << "Output from program: " << buffer << '\n';
		string resultBuffeRead = readBuffer4Label(buffer);
		vectorLabels.push_back(resultBuffeRead);
		
		cout << "Size vectorLabels = " << vectorLabels.size() << endl;
	}
	
	pclose(fp);
}

//--------------------------------------------------------------
string ofxTalairach::readBuffer4Label(char _buffer[]){
	
	string resultsubbuff = "";
	
	if ( sizeof(_buffer) > 0 ){
		cout << "Buffer reading Label= " << _buffer << endl;
		resultsubbuff = ofToString(_buffer);		
	}
	
	return resultsubbuff;
}


//--------------------------------------------------------------
float ofxTalairach::requestSPM(string commandline){
	
	float res = -1;
	
	FILE *fp = popen(commandline.c_str(), "r");
	
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), fp))
	{
		std::cout << "Output from program: " << buffer << '\n';
		res = readBuffer4Caudate(buffer);
	}
	
	pclose(fp);
	
	return res;
}

//--------------------------------------------------------------
float  ofxTalairach::readBuffer4Caudate(char _buffer[]){
	
	//ofVec3f location = ofVec3f(-1,-1,-1);
	float myresult = -1;
	
	if (strncmp (_buffer,"Caudate ",8) == 0){
		cout << "Caudate found= " << _buffer << endl;
		
		//then copy my 5 interesant characters and return the desired variable 
		char subbuff[6];
		memcpy( subbuff, &_buffer[8], 5 );
		subbuff[5] = '\0';
		
		string resultsubbuff(subbuff);
		myresult = ofToFloat(resultsubbuff);
		
	}
	
	return myresult;
}


//--------------------------------------------------------------
void ofxTalairach::keyReleased(int key){
	
	if(key == '1'){
		getStructuralProbMap(ofVec3f(15, 10, 12));
	}
	else if(key == '3'){
		getLabelsArroundCube(ofVec3f(15, 10, 8), cubeSize);
		
	}
	else if(key == OF_KEY_UP){
		cubeSize += 2;
		
		if (cubeSize > 11) {
			cubeSize = 11;
		}
		
		cout << "cubeSize= " << cubeSize << endl;
	}
	else if(key == OF_KEY_DOWN){
		cubeSize -= 2;
		
		if (cubeSize < 3) {
			cubeSize = 3;
		}
		
		cout << "cubeSize= " << cubeSize << endl;
		
	}
}
