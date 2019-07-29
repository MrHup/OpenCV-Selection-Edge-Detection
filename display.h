#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include <cstring>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <mysql/mysql.h>
#include "menu.h"
#include "globals.h"

string createId(string txt); // creates an input stream listener via opencv
void cleanCircles(); // cleans the circles from the display
void displayCircles(); // displays the circles used to define the 4 points
void drawCircles(int,int); // adds a circle to the stream

void adjust_light();
void AlphaThreshold(int theSliderValue, void*);
void BetaThreshold(int theSliderValue, void*);


