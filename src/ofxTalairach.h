// =============================================================================
//
// Copyright (c) 2014 Carles Gutierrez <http://carlesgutierrez.wordpress.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================
// Talairach http://www.talairach.org/manual.html
// =============================================================================


#pragma once

#include "ofMain.h"
#define MARGINTEXT 22
#define MAXITEMS 11


class ofxTalairach
{
public:
	
	enum brainArea {Noresult, Caudate, Putamen, Thalamus, Insula, FrontalLobe, TemporalLobe, ParietalLobe, OccipitalLobe, Cerebellum};
	
    ofxTalairach();
	~ofxTalairach();
	
	void setup(string _filepath);
	void update();
	void close();
	void connectT();
	
	//Key Mouse orders for a simple example
	void mouseMoved(int x, int y );
	void keyReleased(int key);
	
	//Drawers
	void drawDebug(int x, int y);
	void drawRequestedLabels(vector<string> vlabels, int x, int y);
	
	//Getters
	float getStructuralProbMap(ofVec3f pos); //(1) Structural Probability Maps
	vector<string> getLabels(ofVec3f pos); //(2) Talairach label
	vector<string> getLabelsArroundCube(ofVec3f pos, int _cubeSize); //(3) Talairach labels within a cube range
	vector<string> getLabelsGrayMatter(ofVec3f pos); //(4) Talairach gray matter labels.
	
	vector<string>	get(ofVec3f pos);
	vector<string> request(string commandline);
	
	//Setters
	void setLocalhost(bool host);
	void setSearchOption(int searchOption);
	void setSizeCube(int _sizecube);
	
private:	
	//Connecting
	bool	bConnected;
	bool	localhost;
	string	slocalhost;
	int		isearchOption;
	string filepath;
	string command;
	
	//Utils 
	inline bool existsFile (const std::string& name) { return ( access( name.c_str(), F_OK ) != -1 ); }
	
	//Request Funtions
	void requestTL(vector<string> &vlabel, string commandline);
	string readBuffer4Label(char _buffer[]);
	float requestSPM(string commandline);
	float readBufferSPM(char _buffer[]);
	
	//Data used  to request
	ofVec3f locBrain;
	ofVec3f point3d;
	int cubeSize;
	string commandline;
	
	//Data returned
	brainArea actualBrainArea;
	float probMap;
	vector<string> vectorLabelsAroundCube;
	vector<string> vectorLabels;

	
};

