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
	vectorLabelsAroundCube.clear();
	probMap = 0;
	actualBrainArea = -1;
	
}

//------------------------------------------------------------------------------
ofxTalairach::~ofxTalairach(){
	
}

//------------------------------------------------------------------------------
void ofxTalairach::setup(){
	//connectT(); //TODO problems with connection, figure already binding and request are working without anyway..  
	
}

//------------------------------------------------------------------------------
void ofxTalairach::drawDebug(int x, int y){
	
	int yposProb = 80;
	int yposLabel = 180;
	int yposLabelCube = 300;
	
	//Info
	ofDrawBitmapStringHighlight("Press '1','2', or '3' to get a sample with diferent data from The Talairach Client", x, y);
	ofDrawBitmapStringHighlight("Press '+' or '-' for change the cubeSize request", x, y+MARGINTEXT);
	
	ofDrawBitmapStringHighlight("point requested " +ofToString(point3d), x, y+MARGINTEXT*2);
	
	//DaerProbability map
	ofDrawBitmapStringHighlight("Probability Map: ", x, y+yposProb+ MARGINTEXT);
	
	string myBrainAreas[MAXITEMS] = {"Caudate", "Putamen", "Thalamus", "Insula", "Frontal lobe", "Temporal lobe", "Parietal lobe", "Occipital lobe", "Cerebellum"};
	
	if(actualBrainArea>0){
		ofDrawBitmapString(ofToString(myBrainAreas[actualBrainArea],2),x, y+yposProb+ MARGINTEXT*2 );
		ofDrawBitmapString(ofToString(probMap,2),x, y+yposProb+ MARGINTEXT*3 );
	}

	
	//Labels
	ofDrawBitmapStringHighlight("Labels: ", x, y+yposLabel);
	ofSetColor(10, 10, 10);
	drawRequestedLabels(vectorLabels, x,y+yposLabel);
	
	ofDrawBitmapStringHighlight("Labels with a cubeSize=: " + ofToString(cubeSize) , x, y+yposLabelCube);
	ofSetColor(10, 10, 10);
	drawRequestedLabels(vectorLabelsAroundCube, x,y+yposLabelCube);

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
void ofxTalairach::drawRequestedLabels(vector<string> vlabels, int x, int y){
	
	for (int i=0; i<vlabels.size(); i++) {
		ofDrawBitmapString(vlabels[i], x, i*MARGINTEXT + y + MARGINTEXT*2);
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
	
	string cubePointStr = " "+ofToString(pos.x,0)+", "+ofToString(pos.y,0)+", "+ofToString(pos.z,0);
	//commnadline += " 15, 10, 8";
	commnadline += cubePointStr;
	requestTL(vectorLabelsAroundCube, commnadline);
	
	return vectorLabelsAroundCube;
}

//--------------------------------------------------------------
vector<string> ofxTalairach::getLabels(ofVec3f pos){
	//Save last data used to request 
	point3d = pos;
	
	commnadline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD 2,";
	string cubePointStr = " "+ofToString(pos.x,0)+", "+ofToString(pos.y,0)+", "+ofToString(pos.z,0);
	commnadline += cubePointStr;
	requestTL(vectorLabels, commnadline);
	
	return vectorLabels;
}

//--------------------------------------------------------------
void ofxTalairach::requestTL(vector<string> &vlabel, string commandline){
	
	vlabel.clear();
	
	FILE *fp = popen(commandline.c_str(), "r");
	
	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), fp))
	{
		string resultBuffeRead = readBuffer4Label(buffer);
		vlabel.push_back(resultBuffeRead);
	}
	
	pclose(fp);
}

//--------------------------------------------------------------
string ofxTalairach::readBuffer4Label(char _buffer[]){
	
	string resultsubbuff = "";
	
	if ( sizeof(_buffer) > 0 ){
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
		res = readBufferSPM(buffer);
	}
	
	pclose(fp);
	
	return res;
}

//--------------------------------------------------------------
float  ofxTalairach::readBufferSPM(char _buffer[]){
	
	//ofVec3f location = ofVec3f(-1,-1,-1);
	float myresult = -1;
	
	if (strncmp (_buffer,"Caudate ",8) == 0){
		cout << "Caudate found= " << _buffer << endl;
		
		actualBrainArea = Caudate;
		
		//then copy my 5 interesant characters and return the desired variable 
		char subbuff[6];
		memcpy( subbuff, &_buffer[8], 5 );
		subbuff[5] = '\0';
		
		string resultsubbuff(subbuff);
		myresult = ofToFloat(resultsubbuff);
		
	}
	if (strncmp (_buffer,"Putamen ",8) == 0){
		cout << "Putamen found= " << _buffer << endl;
		
		actualBrainArea = Putamen;
		
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
	
	switch (key) {
		case '1':
			getStructuralProbMap(ofVec3f(15, 10, 12));
			break;
		case '2':
			getLabels(ofVec3f(15, 10, 8));
			break;
		case '3':
			getLabelsArroundCube(ofVec3f(15, 10, 8), cubeSize);
			break;
		case OF_KEY_UP:
			cubeSize += 2;
			if (cubeSize > 11)cubeSize = 11;
			break;
		case OF_KEY_DOWN:
			cubeSize -= 2;
			if (cubeSize < 3)cubeSize = 3;
			break;
		default:
			break;
	}

}
