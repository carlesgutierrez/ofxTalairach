ofxTalairach
========

![Screenshot](https://github.com/carlesgutierrez/ofxTalairach/raw/master/screen.png)

#Description
Simple Client for Talairach Daemon by command line opcions
http://www.talairach.org/manual.html#CommandLine

The demo requires: 

openFrameworks https://github.com/openframeworks/openFrameworks 

#Use
PointToTD operates on a single coordinate and has three search options:
For the moment the methods available are (1) and (3)

(1) Structural Probability Maps
(2) Talairach labe
(3) Talairach labels within a cube range
(4) Talairach gray matter labels. 

The program is run as follows:

(1) float getStructuralProbMap(ofVec3f pos);
(2) vector<string> getLabels(ofVec3f pos);
(3) vector<string> getLabelsArroundCube(ofVec3f pos, int sizecube);

There is a example you can check for easy understanding of that functionalities.

#License

Talairach data is free for educational and research purposes.

For commercial usage and redistribution, please contact with Dr. Lancaster from http://www.talairach.org/


