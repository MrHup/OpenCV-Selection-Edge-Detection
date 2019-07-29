#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include <cstring>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <mysql/mysql.h>
#include "globals.h"
#include "InterestZone.h"

string make_PointString(Point a);
string save_saveFile();
void load_saveFile(string txt);
int round_up(double a, double b);
void get_page();
int wait_for_keyMenu();
int load_menu();

