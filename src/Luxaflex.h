//
//  Luxaflex.h
//  FlexDMX
//
//  Created by Joris de Jong on 11/24/13.
//
//

#ifndef FlexDMX_Luxaflex_h
#define FlexDMX_Luxaflex_h


#define STRIPS 30
#define HEIGHT 20
#define STRIPSPEED 5

class Luxaflex
{
public:
    //constructors
    Luxaflex(){};
    Luxaflex(int x);
    
    //functions
    void draw();
    void update(int direction);
    float constrain(float v, float min, float max);
    
    //vars
    int x;
    int w;
    float y[STRIPS];
    float rot; //rotation over x axis
    int lastStrip = STRIPS - 1; //var to make code more legible
    
};


#endif
