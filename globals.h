#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include <cstring>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <mysql/mysql.h>

using namespace cv;
using namespace std;

#ifndef GLOBALS_H_
#define GLOBALS_H_

extern Mat src, src_gray,img, new_image;
extern Mat dst, detected_edges;


extern int lowThreshold, lowThreshold2;
extern double alpha; 
extern int bta,alpha_aux,ratio,kernel_size;

extern MYSQL *connect; // for the database
extern MYSQL_RES *res_set;
extern MYSQL_ROW row;
extern unsigned int numrows;
extern unsigned int last_index;


extern Point cor[5]; // selected points ti define area of interest
extern int k; // index for the last selected point in the array // 0


extern Scalar primaryColor, red, green, white; // default color pallete initialized in video_canny.cpp


#endif
