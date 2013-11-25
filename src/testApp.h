#pragma once

#include "ofMain.h"
#include "Luxaflex.h"

#define MAXMEMORY 250 //25 frames per second, means 10 frames total
#define LUXAS 5 //value between 0 and 5

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //queue vars and functions
    void addToQueue(float v);
    float valueAtTime[2][MAXMEMORY];
    float delay;
    float newDelay;
    bool isQueueEmpty();
    
    //chase vars and functions
    void setChase(int v1, int v2, int v3, int v4, int v5);
    int chase[LUXAS] = {1,2,3,4,5};
    int newChase[LUXAS] = {1,2,3,4,5};
    Luxaflex flex[LUXAS];
};
