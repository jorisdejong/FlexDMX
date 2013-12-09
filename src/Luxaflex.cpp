//
//  Luxaflex.cpp
//  FlexDMX
//
//  Created by Joris de Jong on 11/24/13.
//
//

#include "Luxaflex.h"
#include "testApp.h"


Luxaflex::Luxaflex(int x_)
{
    x=x_;
    w = (ofGetWidth() * 0.75) / LUXAS;
    y[lastStrip] = STRIPS;
    rot=10;
}

void Luxaflex::update(int direction)
{
    //base y position of each strip on the y position of the strip following it
    for(int i = STRIPS -2; i > 0; i--)
        y[i]= fminf(i*HEIGHT, y[i+1]-1);
    
    int mult;
    if(sendDMX)
        mult = 1;
    else
        mult = 10.0;
    //move last strip 
    switch (direction)
    {
        case -1:
            y[lastStrip]-=1.0 * STRIPSPEED * mult;
            rot+=1.0 * ROTSPEED * mult;
            break;
        case 1:
            y[lastStrip]+=1.0 * STRIPSPEED * mult;
            rot-=1.0 * ROTSPEED * mult;
            break;
        default:
            break;            
    }
    
    //limit y of last strip to min and max value
    y[lastStrip]=constrain(y[lastStrip], STRIPS, lastStrip * HEIGHT);
    rot=constrain(rot,10,170);
}

void Luxaflex::draw()
{
    for(int i =0; i < lastStrip; i++)
    {
        ofPushMatrix();
        ofTranslate(x,y[i]);
        ofRotate(rot,1.0,0.0,0.0);
        ofRect(0,0,w,HEIGHT*0.9);
        ofPopMatrix();
    }
}

//constrain function
float Luxaflex::constrain(float v, float min, float max)
{
    if(v > min && v < max)
        return v;
    if(v <= min)
        return min;
    if(v >= max)
        return max;
}