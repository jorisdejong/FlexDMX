/*
 Roadmap
 
 done - Port Queue to oF
 done - Create objects for data containment
 done - Port VirtuaFlex to oF
 done-  make pretty rotation function for flex object
 done - Implement chase selection method, probably use presets
 done - GUI
 
 not needed - Implement preset save and recall
 done - Implement burst methods to reset to real world
 
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
    
    //names for chase radio buttons
    vector<string> names;
	names.push_back("1, 2, 3, 4, 5");
	names.push_back("3, 2, 1, 2, 3");
	names.push_back("1, 5, 1, 5, 1");
    names.push_back("1, 1, 1, 1, 1");
        
    gui = new ofxUICanvas(0,ofGetHeight()-250,200,250);
    gui->setDrawBack(false);
    gui->addLabel("FLEX DMX", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addSlider("Control",0.0,1.0,0.5,40,200);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("Delay",0.0,1.0,0.2,40,200);

    gui->addToggle("Send DMX", true);
    //gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    ofxUIRadio *radio = gui->addRadio("Chase", names, OFX_UI_ORIENTATION_VERTICAL, 17, 17);
    radio->activateToggle("1, 2, 3, 4, 5");
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    //gui->loadSettings("GUI/guiSettings.xml");
    

    
    //init flex objects
    for(int i = 0; i < LUXAS; i++)
        flex[i] = Luxaflex((ofGetWidth()/LUXAS)*i+(ofGetWidth()/LUXAS)*0.125);
    
    //delay value per flex
    delay = 0.2;
    newDelay = delay;
    control = 0.5;
    
    sendDMX = true;
    
    //Enntec setup
    //get the serial device list
    vector <ofSerialDeviceInfo> devices = deviceList.getDeviceList();
    for(int i = 0; i < devices.size(); i++)
    {
        //cout<<devices[i].getDeviceName()<<endl;
        deviceName = devices[i].getDeviceName();
        if(deviceName.substr(0,3) == "tty" && deviceName.substr(14,2) == "EN")
        {
            //connect to the Enttec
            dmx.connect(deviceName, 96);
            connectedToEnttec = true;
            return;
            
        }
        else
            connectedToEnttec = false;
        
    }
    dmx.setChannels(10);
}

//--------------------------------------------------------------
void testApp::update(){

    
    if(control > 0.7)
        addToQueue(-1);
    else if (control < 0.3)
        addToQueue(1);
    else
        addToQueue(0);
    
    //check if the queue is empty or not so we can safely update the delay
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
        flex[i].sendDMX = sendDMX;
        
        int v = (MAXMEMORY/LUXAS-1) * (chase[i]-1) * delay;
        flex[i].update(valueAtTime[1][v]);
        

    }
    
    for(int i = 0; i < LUXAS; i++)
    {
        if(flex[i].move==true)
            sendDmxValue(255, i*2+1);
        else
            sendDmxValue(1, i*2+1);
    
        if(flex[i].upDown == -1)
            sendDmxValue(255,i*2+2);
        if(flex[i].upDown == 1)
            sendDmxValue(1,i*2+2);
    }
    
    dmx.update();
 
}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(32);
    ofSetColor(255);
    
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

void testApp::exit()
{
    // gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
	int kind = e.widget->getKind();

    
    if(name == "Delay")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        newDelay = slider->getScaledValue();
    }
    if(name == "Control")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        control = slider->getScaledValue();

    }
    if(name == "Send DMX")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        sendDMX = toggle->getValue();
        
    }
    
    if(kind == OFX_UI_WIDGET_TOGGLE && name != "Send DMX")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        vector <string> chases = ofSplitString(name, ",");
        setChase(ofToInt(chases[0]), ofToInt(chases[1]), ofToInt(chases[2]), ofToInt(chases[3]), ofToInt(chases[4]));
    }
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout<<key<<endl;
    switch (key)
    {

            
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

void testApp::sendDmxValue(int value, int channel)
{
    dmx.setLevel(channel, value);
}