// #####################################################
// # The following code connects to a server and by
// # analyzing live video feed from a camera, determines
// # the number of edges detected in certain defined
// # 'zones of interest'. The program REQUIRES:
// # Opencv C++11. Was tested for Ubuntu 16.04
// #
// #  The program provides a simple interface for initial
// #  configurations and supports:
// #  - brightness and canny filter in-app tweaking
// #  - configuration saving/loading
// #  - dynamic interaction with the definez zones (add, delete, update)
// #####################################################

#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include <cstring>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <mysql/mysql.h>

#include "display.h"

#define SERVER "YOUR IP HERE"
#define USER "USER"
#define PASSWORD "PASS"
#define DATABASE "DB_NAME"

using namespace cv;
using namespace std;

Mat src, src_gray,img, new_image;
Mat dst, detected_edges;
//----------------DEFAULTS-------------- (external from global.h)
Scalar primaryColor = Scalar(255,0,0);
Scalar red = Scalar(0,0,255);
Scalar green = Scalar(0,255,0);
Scalar white = Scalar(255,255,255);
int lowThreshold=45; // number of edges slider
int lowThreshold2=4; // Sensitivity slider
const int max_lowThreshold=100; // maximum number of edges on the slider
const int max_edgeThreshold=18; // maximum sensitivity on the slider
double alpha=1.3; int bta=60; // alpha and beta defaults for the brightness sliders
int alpha_aux=13;
//----
unsigned int numrows;
unsigned int last_index=0;

int ratio = 3;
int kernel_size = 3;
const char* window_name="Edge Map"; // title

MYSQL *connect; //connection instance for the DB
MYSQL_RES *res_set; // result
MYSQL_ROW row; // last retrieved row from the DB

Point cor[5]; // selected points ti define area of interest
int k=0; // index for the last selected point in the array

list<InterestZone*>zones;
bool connect_me(){ // connect to the server
	connect=mysql_init(NULL);
	if (!connect)
	{
		cout<<"MySQL Initialization failed";
		return false;
	}
	connect=mysql_real_connect(connect, SERVER, USER, PASSWORD , DATABASE ,0,NULL,0);
	if (connect)
	{
		cout<<"connection Succeeded\n";
	}
	else
	{
		cout<<"connection failed\n";
	}
	return true;
}

// ------ check collision
void onMeanThresholdSlide(int theSliderValue)
{
	lowThreshold2 = theSliderValue;
}
void checkForCollision(){
		int j=0;
		list<InterestZone*>::iterator it;
		for(it=zones.begin();it!=zones.end();it++){
			j++;
			int val = (*it)->compute_mean();
			if(val > lowThreshold2){
				(*it)->displayEdges(red);
				if((*it)->filled=="1"){
					(*it)->filled ="0";
					(*it)->updateSV();
				}
			}else{
				(*it)->displayEdges(green);
				if((*it)->filled=="0"){
					(*it)->filled ="1";
					(*it)->updateSV();
				}
			}
		}
}
//-------------------Mouse call
void mouse_call(int event,int x,int y,int,void*)
{
	if(event==EVENT_LBUTTONDOWN && k<4)
	{
			drawCircles(x,y);
			imshow("Edge Map",img); k++;
	}
}
static void CannyThreshold(int, void*) // applies canny filter for edge detection
{
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
		displayCircles(); checkForCollision();
    imshow( window_name, dst );
    setMouseCallback(window_name,mouse_call);
}
static void EdgeThreshold(int theSliderValue, void*)
{
  lowThreshold2 = theSliderValue;
}

//-----------------------Main
int main()
{
	namedWindow("Menu");
	if(!connect_me()) return 0; // break if anything bad happens
  if(load_menu()==0) return 0; // load the menu
	destroyWindow("Menu");
 //Open the default video camera
 VideoCapture cap;
 cap.open(0);
 //cap.open("http://37.156.71.254/cgi-bin/faststream.jpg?stream=half&fps=15&rand=COUNTER#.XTFhpwHi2m4.link");
 // if not success, exit program
 cleanCircles();
 if (cap.isOpened() == false)
 {
  cout << "Cannot open the video camera" << endl;
  cin.get(); //wait for any key press
  return -1;
 }

 double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
 double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

 cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;
 string window_name = "Edge Map";
 namedWindow(window_name); //create a window called "My Camera Feed"

 while (true)
 {
  Mat frame;
  bool bSuccess = cap.read(frame); // read a new frame from video
  if (bSuccess == false)
  {
   cout << "Video camera is disconnected" << endl;
   cin.get(); //Wait for any key press
   break;
  }
  //show the frame in the created window
  src = frame;
	adjust_light();
  img = dst;

  dst.create( src.size(), src.type() );
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  namedWindow( window_name, WINDOW_AUTOSIZE );

	// create the sliders attached to the stream
	createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  createTrackbar( "Min Sensitivity:", window_name, &lowThreshold2, max_edgeThreshold, EdgeThreshold );
	createTrackbar( "Alpha:", window_name, &alpha_aux, 35, AlphaThreshold );
	createTrackbar( "Beta:", window_name, &bta, 100, BetaThreshold);

  CannyThreshold(0, 0);
	int wk = waitKey(10);
  if (wk == 27)
  {
	   cout << "Esc key is pressed by user. Stoppig the video" << endl;
		 const string cmnd = "DELETE FROM Spot;";
		 mysql_query(connect, cmnd.c_str());
	   break;
	} else if(wk==8){ //backspace pressed
			if(k>0){ // delete last point if there are any
				cor[k-1].x=-1;
	 		 	k--;
		 }else if(!zones.empty()){ // if not, delete last zone
			 const string cmnd = "DELETE FROM Spot WHERE ID LIKE'" + zones.back()->ID + "';";
			 mysql_query(connect, cmnd.c_str());
			 zones.pop_back();
		 }
	} else if(wk == 0x0D || wk == 0x50){
		if(k==4){
			// create zone, add zone to list
			InterestZone *n1 = new InterestZone(cor);
			zones.push_back(n1);
			cleanCircles(); // clear display from selected points (and reset k)
			n1->ID = createId("Insert an ID:");
			n1->addToSV();
			cout<<"Add begin\n";
		}
	} else if(wk == 32) { // Spacebar  is pressed
		int pars=0;
		list<InterestZone*>::iterator it;
		for(it=zones.begin();it!=zones.end();it++){
			cout<<(*it)->medium<<"\n";
			for(int j=0;j<4;j++){
				cout<<(*it)->points[0][j]<<" ";
			}
			cout<<"\n";
		}
		cout<<"\n";
	} else if(wk == 's'){
		string name = createId("Set a name for your save:"); string aux;
		string cmnd = "Select uid from Config where name like '" + name + "';";
		mysql_query (connect,cmnd.c_str());
		res_set = mysql_store_result(connect);
		row= mysql_fetch_row(res_set);
		if(res_set!=nullptr && row!=nullptr){
			aux = createId("Override the save named: "+name+" (y/n)");
			if(aux=="y" || aux =="Y" || aux == "yes" || aux == "YES" || aux=="Yes"){
				cmnd = "DELETE FROM Config WHERE name LIKE '" + name +"';";
				mysql_query(connect, cmnd.c_str());
				cmnd = "INSERT INTO Config VALUES (uid, '"+name+"','" + save_saveFile()+"',"+to_string(lowThreshold)+","+to_string(lowThreshold2)+","+to_string(alpha_aux)+","+to_string(bta)+");";
				mysql_query(connect, cmnd.c_str());
			}
		}
		else{
			cmnd = "INSERT INTO Config VALUES (uid, '"+name+"','" + save_saveFile()+"',"+to_string(lowThreshold)+","+to_string(lowThreshold2)+","+to_string(alpha_aux)+","+to_string(bta)+");";
			mysql_query(connect, cmnd.c_str());
		}
	}
 }
 mysql_close (connect);
 return 0;

}

