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
}

void Luxaflex::update(int direction)
{
    //base y position of each strip on the y position of the strip following it
    for(int i = STRIPS -2; i > 0; i--)
        y[i]= fminf(i*HEIGHT, y[i+1]-1);
    
    //move last strip 
    switch (direction)
    {
        case -1:
            y[lastStrip]-=1.0 * STRIPSPEED;
            break;
        case 1:
            y[lastStrip]+=1.0 * STRIPSPEED;
            break;
        default:
            break;            
    }
    
    //limit y of last strip to min and max value
    y[lastStrip]=constrain(y[lastStrip], STRIPS, lastStrip * HEIGHT);
}

void Luxaflex::draw()
{
    for(int i =0; i < lastStrip; i++)
    {
        ofRect(x,y[i],w,1);
        //TODO this needs pretty rotate function
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