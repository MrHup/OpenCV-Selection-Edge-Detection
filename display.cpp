#include "display.h"

//--- for text
string createId(string txt){
	int kei = waitKey(10);
	string current_ID;
	Mat aux_mat = img.clone();
	putText(aux_mat, txt, cvPoint(60,120), FONT_HERSHEY_COMPLEX_SMALL, 1.2, red, 1, CV_AA);
	imshow("Edge Map",aux_mat);
	while((kei != 0x0D && kei != 0x50) || current_ID.empty()==true){
			aux_mat = img.clone();
			if(((char)kei >= 'a' && (char)kei <='z') || ((char)kei >= '0' && (char)kei <='9' )){
				//cur_ID[strlen(cur_ID)]=(char)kei;
				current_ID.push_back((char)kei);
				putText(aux_mat, txt, cvPoint(60,120), FONT_HERSHEY_COMPLEX_SMALL, 1.2, red, 1, CV_AA);
				putText(aux_mat, current_ID, cvPoint(60,230), FONT_HERSHEY_COMPLEX_SMALL, 2.8, white, 2, CV_AA);
				imshow("Edge Map",aux_mat);
			}
			if(kei == 0x08 && current_ID.length()>0) // presses backspace
			{
					current_ID[current_ID.length()-1] = 0; current_ID.erase(current_ID.end()-1);
					putText(aux_mat, txt, cvPoint(60,120), FONT_HERSHEY_COMPLEX_SMALL, 1.2, red, 1, CV_AA);
					putText(aux_mat, current_ID, cvPoint(60,230), FONT_HERSHEY_COMPLEX_SMALL, 2.8, white, 2, CV_AA);
					imshow("Edge Map",aux_mat);
			}
			kei = waitKey(10);
	}
	cout<<"ID is "<<current_ID<<"\n";
	return current_ID;
}
//----------------------------------
void cleanCircles(){
	k=0; //reset k
	for(int i=0;i<4;i++){ //clean vertices
		cor[i].x = -1; cor[i].y = -1;
	}
}
void displayCircles(){
	for(int i=0;i<4;i++){
		if(cor[i].x!=-1){
				circle(img, cor[i], 3, primaryColor, 2, 8, 0);
		}
	}
}
void drawCircles(int ex,int ey){ // draw based on given coordinates
	if(k!=4){
		cor[k].x = ex; cor[k].y=ey;
	}
	displayCircles();

	cout<<"Interest Point["<<k<<"] created at position: "<<cor[k].x<<", "<<cor[k].y<<"\n";

}
//--------- image adjustments
void adjust_light(){
	new_image = Mat::zeros( src.size(), src.type() );
	for( int y = 0; y < src.rows; y++ ) {
			for( int x = 0; x <src.cols; x++ ) {
					for( int c = 0; c < src.channels(); c++ ) {
							new_image.at<Vec3b>(y,x)[c] =
								saturate_cast<uchar>( alpha*src.at<Vec3b>(y,x)[c] + bta -50 );
					}
			}
	}
	src = new_image;
}
void AlphaThreshold(int theSliderValue, void*)
{
  alpha_aux = theSliderValue;
	alpha = (double)theSliderValue/10;
}
void BetaThreshold(int theSliderValue, void*)
{
  bta = theSliderValue;
}
