/*
 Roadmap
 
 done - Port Queue to oF
 done - Create objects for data containment
 done - Port VirtuaFlex to oF
 - make pretty rotation function for flex object
 done Implement chase selection method, probably use presets
 - Implement preset save and recall
 - Implement burst methods
 - Implement individual control
 
 - Implement midi protocol
 - Implement DMX output
 - Implement OSC BPM listen?
 - Implement OSC output to FFGL plugin?
 */

#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(25);
    ofEnableSmoothing();
    
    //init flex objects
    for(int i = 0; i < LUXAS; i++)
        flex[i] = Luxaflex((ofGetWidth()/LUXAS)*i+(ofGetWidth()/LUXAS)*0.125);
    
    //delay value per flex
    delay = 0.2;
    newDelay = delay;
}

//--------------------------------------------------------------
void testApp::update(){
    newDelay = float(mouseX)/float(ofGetWidth());
    
    //temp mouse control
    if(mouseY < ofGetHeight()/3)
        addToQueue(-1);
    else if (mouseY > ofGetHeight()/3 * 2)
        addToQueue(1);
    else
        addToQueue(0);
    
    //check if the queue is empty or not so we can safely update the delay

    //maybe I should just handle this with a master switch on/off?
    if(newDelay != delay && isQueueEmpty())
    {
            delay=newDelay;
    }
    
    //check if the queue is empty or not so we can safely update the chase
    bool isChaseDifferent = false;
    for(int i = 0; i < LUXAS; i++)
        if(chase[i]!=newChase[i])
            isChaseDifferent = true;
    
    if(isChaseDifferent && isQueueEmpty())
    {
        for(int i = 0; i < LUXAS; i++)
            chase[i]=newChase[i];
        isChaseDifferent=false;
    }
    
    //get the value from the queue
    //taking delay value and chase order into account
    //feed it to flex objects
    for(int i = 0; i < LUXAS; i++)
    {
        int v = (MAXMEMORY/LUXAS-1) * (chase[i]-1) * delay;
        flex[i].update(valueAtTime[1][v]);
    }
 
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(32);
    ofSetColor(255);
    
    //draw temp help guides
    ofLine(0,ofGetHeight()/3,ofGetWidth(),ofGetHeight()/3);
    ofLine(0,ofGetHeight()/3 * 2,ofGetWidth(),ofGetHeight()/3 * 2);
    
    //draw flex object
    for(int i = 0; i < LUXAS; i++)
        flex[i].draw();

    if(isQueueEmpty())
    {
        ofSetColor(0,255,0);
        ofRect(ofGetWidth()-20,ofGetHeight()-20,10,10);
    }
    else
    {
        ofSetColor(255,0,0);
        ofRect(ofGetWidth()-20,ofGetHeight()-20,10,10);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key)
    {
        case 49:
            setChase(1, 2, 3, 4, 5);
            break;
        case 50:
            setChase(2, 1, 2, 0, 0);
            break;
        case 51:
            setChase(5, 4, 3, 2, 1);
            break;
        case 52:
            setChase(1, 1, 1, 1, 1);
            break;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){


}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::addToQueue(float v)
{
    //push back all the queue values 
    for(int i = MAXMEMORY-1; i > 0; i--)
    {
        valueAtTime[1][i] = valueAtTime[1][i-1];
        valueAtTime[0][i] = valueAtTime[0][i-1];
    }
    
    //and update the first one
    valueAtTime[1][0]=v;
    valueAtTime[0][0]=ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void testApp::setChase(int v1, int v2, int v3, int v4, int v5)
{
    int tempChase[5] = {v1,v2,v3,v4,v5};
    for(int i = 0; i < LUXAS; i++)
        newChase[i]=tempChase[i];
}

//--------------------------------------------------------------
bool testApp::isQueueEmpty()
{
    bool isEmpty = true;
    
    int highestChaseValue = 1;
    for(int i = 0; i < LUXAS; i++)
        if(chase[i]>highestChaseValue)
            highestChaseValue=chase[i];
    
    
    //TAKE INTO ACCOUNT HIGHEST CHASE VALUE (not necesserily 5);
    int lastMemoryInUse = int((MAXMEMORY/(LUXAS+1)) * highestChaseValue * delay);
    
    //if just one of the queue values we've needed for this delay and chase value is not 0
    //it means there is still animation going on, and so the queue is not empty
    for(int i = 0; i <= lastMemoryInUse; i++)
        if(valueAtTime[1][i]!=0)
            isEmpty = false;
    
    //if that's not the case
    if(isEmpty)
    {
        //zap the remaining memory to 0
        for(int i = lastMemoryInUse+1; i < MAXMEMORY; i++)
            valueAtTime[1][i] = 0;
        return true;
    }
    else{
        return false;
    }
}