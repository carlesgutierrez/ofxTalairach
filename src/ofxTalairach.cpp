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
	actualBrainArea = Noresult;
	locBrain = ofVec3f(0,0,0);
	
	localhost =		true;
	isearchOption = 2;
	command =		" org.talairach.PointToTD";
	filepath =		"../../../data/talairach.jar";
}

//------------------------------------------------------------------------------
ofxTalairach::~ofxTalairach(){
	
}

//------------------------------------------------------------------------------
void ofxTalairach::setup(string _filepath){
	filepath=_filepath;
	//a check for a jar File Exitance
	if(existsFile(filepath)){ // TODO This only will work for OSX systems
		commandline = "java -cp "+ filepath +" org.talairach.AtlasServer 1600";
		system(commandline.c_str());
		bConnected = true; //TODO better to read buffer output to check connection message

	}else {
		cout << "ofxTalairach Error :: talairach.jar does not exist in bin/data folder" << endl;
	}
}

//------------------------------------------------------------------------------
void ofxTalairach::close(){
	//a check for a jar File Exitance
	if(bConnected){
		commandline = "java -cp "+ filepath +" org.talairach.AtlasServer 1600";
		system(commandline.c_str());
		bConnected = false;
	}
}

//--------------------------------------------------------------
void ofxTalairach::connectT(){
	//a check for a jar File Exitance
	if(existsFile("../../../data/talairach.jar")){ // TODO This only will work for OSX systems
		commandline = "java -cp ../../../data/talairach.jar org.talairach.AtlasServer 1600";
		system(commandline.c_str());
		bConnected = true; //TODO better to read buffer output to check connection message
	}else {
		cout << "ofxTalairach Error :: talairach.jar does not exist in bin/data folder" << endl;
	}
	
}

//------------------------------------------------------------------------------
void ofxTalairach::drawDebug(int x, int y){
	
	int yposProb = 100;
	int yposLabel = 190;
	int yposLabelCube = 310;
	
	//Info
	ofDrawBitmapStringHighlight("Press '1','2', or '3' to get a sample with diferent data from The Talairach Client", x, y);
	ofDrawBitmapStringHighlight("Move mouse to change Brain Point location x="+ofToString(locBrain.x,2)+" y="+ofToString(locBrain.y,2)+" z="+ofToString(locBrain.z,2), x, y+MARGINTEXT);
	ofDrawBitmapStringHighlight("Press '+' or '-' for change the cubeSize request", x, y+MARGINTEXT*2);
	
	ofDrawBitmapStringHighlight("Last point requested ", x, y+MARGINTEXT*3);
	ofDrawBitmapString(ofToString(point3d), x, y+MARGINTEXT*4);
	
	//DaerProbability map
	ofDrawBitmapStringHighlight("Probability Map: ", x, y+yposProb+ MARGINTEXT);
	
	//TODO this should be defined in header as static...
	string myBrainAreas[MAXITEMS] = {"Noresult", "Caudate", "Putamen", "Thalamus", "Insula", "Frontal lobe", "Temporal lobe", "Parietal lobe", "Occipital lobe", "Cerebellum"};
	
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
void ofxTalairach::drawRequestedLabels(vector<string> vlabels, int x, int y){
	
	for (int i=0; i<vlabels.size(); i++) {
		ofDrawBitmapString(vlabels[i], x, i*MARGINTEXT + y + MARGINTEXT*2);
	}
	
}

//GETTERS
//--------------------------------------------------------------
vector<string> ofxTalairach::get(ofVec3f pos){

	commandline =  "java -cp ";
	commandline += filepath;
	commandline += command;
	commandline += slocalhost;
	string slabelop;
	if (isearchOption==3){
		slabelop = " "+ofToString(isearchOption,0)+":" + " "+ofToString(cubeSize,0)+",";
	}else{
		slabelop = " "+ofToString(isearchOption,0)+",";
	}
	commandline += slabelop;
	string cubePointStr =	  " " +ofToString(pos.x,0)
							+ ", "+ofToString(pos.y,0)
							+ ", "+ofToString(pos.z,0);
	commandline += cubePointStr;
	
	cout << commandline << endl;
	return	request(commandline); //request
}

//--------------------------------------------------------------
float ofxTalairach::getStructuralProbMap(ofVec3f pos){
	if (localhost) {
		string slocalhost = " host=localhost:1600";
		commandline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD";
		string slabelop = " 1,";
		commandline += slocalhost;
		commandline += slabelop;
		
	}else commandline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD 1,";
	//commandline += " 15, 10, 12";
	
	string cubePointStr = " "+ofToString(pos.x,0)+", "+ofToString(pos.y,0)+", "+ofToString(pos.z,0);
	commandline += cubePointStr;
	
	probMap = requestSPM(commandline);//Structural Probability Maps request
	
	return probMap;
}

//--------------------------------------------------------------
vector<string> ofxTalairach::getLabels(ofVec3f pos){
	//Save last data used to request
	point3d = pos;
	
	if (localhost) {
		string slocalhost = " host=localhost:1600";
		commandline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD";
		string slabelop = " 2,";
		commandline += slocalhost;
		commandline += slabelop;
		
	}else{
		commandline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD 2,";
	}
	string cubePointStr = " "+ofToString(pos.x,0)+", "+ofToString(pos.y,0)+", "+ofToString(pos.z,0);
	commandline += cubePointStr;
	requestTL(vectorLabels, commandline);
	
	return vectorLabels;
}

//--------------------------------------------------------------
vector<string> ofxTalairach::getLabelsArroundCube(ofVec3f pos, int _cubeSize){
	//Save last data used to request
	point3d = pos;
	cubeSize = _cubeSize;
	
	if (localhost) {
		string slocalhost = " host=localhost:1600";
		commandline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD";
		string slabelop = " 3:";
		commandline += slocalhost;
		commandline += slabelop;
		
	}else commandline = "java -cp ../../../data/talairach.jar org.talairach.PointToTD 3:";
	string cubeSizeStr = " "+ofToString(cubeSize,0)+",";
	//commandline += " 7,";// To set a different cube size, use "3:<cubesize>". Sizes of 3, 5, 7, 9 and 11 are accepted.
	commandline += cubeSizeStr;
	
	string cubePointStr = " "+ofToString(pos.x,0)+", "+ofToString(pos.y,0)+", "+ofToString(pos.z,0);
	commandline += cubePointStr;
	requestTL(vectorLabelsAroundCube, commandline);
	
	return vectorLabelsAroundCube;
}

//--------------------------------------------------------------
vector<string> ofxTalairach::request(string commandline){
	
	FILE *fp = popen(commandline.c_str(), "r");
	vector<string> labels;
	string resultsubbuff;
	
	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), fp))
	{
		if ( sizeof(buffer) > 0 ){
			resultsubbuff = ofToString(buffer);
		}
		labels.push_back(resultsubbuff);
	}
	
	pclose(fp);
	return labels;
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
	
	bool bProbMap = false;
	int numberofchars = 0;
	
	//ofVec3f location = ofVec3f(-1,-1,-1);
	float myresult = -1;
	
	if (strncmp (_buffer,"Caudate ",8) == 0){
		cout << "Caudate found= " << _buffer << endl;
		actualBrainArea = Caudate;
		bProbMap = true;
		numberofchars = 8;
	}
	else if (strncmp (_buffer,"Putamen ",8) == 0){
		cout << "Putamen found= " << _buffer << endl;
		actualBrainArea = Putamen;
		bProbMap = true;
		numberofchars = 8;
	}
	else if (strncmp (_buffer,"Thalamus ",9) == 0){
		cout << "Thalamus found= " << _buffer << endl;
		actualBrainArea = Thalamus;
		bProbMap = true;
		numberofchars = 9;
	}
	else if (strncmp (_buffer,"Insula ",7) == 0){
		cout << "Insula found= " << _buffer << endl;
		actualBrainArea = Insula;
		bProbMap = true;
		numberofchars = 7;
	}
	else if (strncmp (_buffer,"Frontal Lobe ",13) == 0){
		cout << "FrontalLobe found= " << _buffer << endl;
		actualBrainArea = FrontalLobe;
		bProbMap = true;
		numberofchars = 13;
	}
	else if (strncmp (_buffer,"Temporal Lobe ",14) == 0){
		cout << "TemporalLobe= " << _buffer << endl;
		actualBrainArea = TemporalLobe;
		bProbMap = true;
		numberofchars = 14;
	}
	else if (strncmp (_buffer,"Parietal Lobe ",14) == 0){
		cout << "ParietalLobe= " << _buffer << endl;
		actualBrainArea = ParietalLobe;
		bProbMap = true;
		numberofchars = 14;
	}
	else if (strncmp (_buffer,"Occipital Lobe ",15) == 0){
		cout << "OccipitalLobe= " << _buffer << endl;
		actualBrainArea = OccipitalLobe;
		bProbMap = true;
		numberofchars = 15;
	}
	else if (strncmp (_buffer,"Cerebellum ",11) == 0){
		cout << "Cerebellum= " << _buffer << endl;
		actualBrainArea = OccipitalLobe;
		bProbMap = true;
		numberofchars = 11;
	}
	else if  (strncmp (_buffer,"No data",7) == 0){
		cout << "No data found" << _buffer << endl;
		actualBrainArea = Noresult;
		bProbMap = false;
	}
	
	if(bProbMap){
		//then copy my 5 interesant characters and return the desired variable 
		char subbuff[6];
		memcpy( subbuff, &_buffer[numberofchars], 5 );
		subbuff[5] = '\0';
		
		string resultsubbuff(subbuff);
		myresult = ofToFloat(resultsubbuff);
	}
	//Caudate, Putamen, Thalamus, Insula, FrontalLobe, TemporalLobe, ParietalLobe, OccipitalLobe, Cerebellum
	
	return myresult;
}

//--------------------------------------------------------------
void ofxTalairach::setLocalhost(bool _host){
	localhost = _host;
	if (localhost==true){
		slocalhost = " host=localhost:1600";
	}else{
		slocalhost = "";
	}
}

//--------------------------------------------------------------
void ofxTalairach::setSearchOption(int _searchOption){
	
	if (_searchOption>0 && _searchOption<4){
		isearchOption = _searchOption;
	}else{
		cout << "search option must be 1 , 2 or 3";
	}
}

void ofxTalairach::setSizeCube(int _sizecube){
	//Sizes of 3, 5, 7, 9 and 11 are accepted.
	if(_sizecube == 3 || _sizecube == 5 || _sizecube == 7 || _sizecube == 9 || _sizecube == 11){
		cubeSize = _sizecube;
	}
	else {
		cout << "ofxTalairach:: Set a cubeSize error. Only 3, 5, 7, 9 and 11 are accepted" << endl;
	}
}











