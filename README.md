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
(3) vector<string> getLabelsArroundCube(ofVec3f pos, int sizecube);


#License

MIT License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.


