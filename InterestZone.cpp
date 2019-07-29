#include "InterestZone.h"

Point getWeightedPoint(Point a, Point b, Point c, Point d){
	Point result;
	result.x = (a.x+b.x+c.x+d.x)/4;
	result.y = (a.y+b.y+c.y+d.y)/4;
	return result;
}

string makePointString(Point a){
	string result = "point(";
	result+=to_string(a.x)+","+to_string(a.y)+")";
	return result;
}

string makePolyString(Point a, Point b, Point c, Point d){
	return makePointString(a)+","+makePointString(b)+","+makePointString(c)+","+makePointString(d);
}

InterestZone::InterestZone(){
	filled = "0";
	npoints = 4;
	points[0]=pts[0];
}

InterestZone::InterestZone(Point* rec){
		points[0]=pts[0];
		filled = "0";
		for(int i=0;i<4;i++){
			pts[0][i]=rec[i]; // assigning the points for the mask
		}
		npoints=4;
		cout<<"!POINTS "<< pts[0][0] <<" "<<pts[0][1]<<" " <<pts[0][2]<<" "<<pts[0][3]<<"\n";
	}

void InterestZone::displayEdges(Scalar color){
		//draw lines
		for(int i=1;i<4;i++){
			if(pts[0][i].x!=-1){
				line(img,pts[0][i],pts[0][i-1],color);
				putText(img, ID, getWeightedPoint(pts[0][0],pts[0][1],pts[0][2],pts[0][3]), FONT_HERSHEY_COMPLEX_SMALL, 0.6, color, 1, CV_AA);
			}
		}
		if(pts[0][3].x!=-1) line(img,pts[0][3],pts[0][0],color);
	}

	int InterestZone::compute_mean(){
		Mat grey = img.clone();
		Mat1b mask(grey.rows, grey.cols, uchar(0));
		Mat1b mask2 = mask.clone();
		fillPoly(mask2, (const Point**)points, &this->npoints, 1, Scalar(255,0,0));
		Scalar average = mean(grey, mask2);
		//cout<<"The average for [TAG: "<<j<<"] is: "<<average<<"\n";
		medium = (average[0] + average[1] + average[2])/3;
		return medium;
		}

	void InterestZone::addToSV(){
		cout<<"Add end\n";
		const string cmnd = "INSERT INTO Spot VALUES ('" + ID + "',"+ filled+",0,"+
                makePolyString(pts[0][0],pts[0][1],pts[0][2],pts[0][3])+");";
		mysql_query(connect, cmnd.c_str());
		cout<<cmnd<<"\n";
	}
	void InterestZone::updateSV(){
		const string cmnd = "UPDATE Spot SET isFree="+filled+" WHERE ID LIKE '"+ID+"';";
		mysql_query(connect, cmnd.c_str());
	}


