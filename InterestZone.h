#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include <cstring>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <mysql/mysql.h>
#include "globals.h"

using namespace cv;
using namespace std;

string makePointString(Point a);
string makePolyString(Point a, Point b, Point c, Point d);
Point getWeightedPoint(Point a, Point b, Point c, Point d);

class InterestZone{
public:
	Point pts[1][4];
	int npoints;
	int medium; int index;
	Point* points[1];
	string ID; 
	string filled;
	
	InterestZone();
	InterestZone(Point*);
	void displayEdges(Scalar);
	int compute_mean();
	void addToSV();
	void updateSV();
};

extern list<InterestZone*>zones;

