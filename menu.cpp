#include "menu.h"

string make_PointString(Point a){
	string result = "point(";
	result+=to_string(a.x)+","+to_string(a.y)+")";
	return result;
}

string save_saveFile(){
	list<InterestZone*>::iterator it;
	string result="";
	for(it=zones.begin();it!=zones.end();it++){
		result+=(*it)->ID+" ";
		for(int j=0;j<4;j++){
			result+=make_PointString((*it)->points[0][j])+" ";
		}
	}
	cout<<"Result is: "<<result;
	return result;
}

void load_saveFile(string txt){
	string s = txt, delimiter = " ", token;
	size_t pos = 0; int i=0;
	InterestZone *n1;
	while ((pos = s.find(delimiter)) != string::npos) {
		 token = s.substr(0, pos);
		 if(i==0){
			 n1 = new InterestZone();
			 n1->ID = token;
		 }else{
			 Point pnt;
			 token.erase(0,6);
			 token.pop_back();
			 string coord1 = token.substr(0, s.find(",")); // first coordinate
			 pnt.x = stoi(coord1);
			 size_t pos = token.find(",");
  	 	 string coord2 = token.substr (pos+1); // second coordinate
			 pnt.y = stoi(coord2);
			 n1->pts[0][i-1] = pnt;
		 }
		 if(i==4) { i=-1;zones.push_back(n1); }
		 s.erase(0, pos + delimiter.length());
		 i++;
	}
}

int round_up(double a, double b){
	double c = a/b;
	if(c>(int)c) c++;
	return (int)c;
}

void get_page(){ // get page of configs
	for(unsigned int j=1; j<=8 && row!=NULL;j++){
		cout<<"("<<j<<") "<<row[0]<<"\n";
		row= mysql_fetch_row(res_set);
		last_index++;
	}
}

int wait_for_keyMenu(){
	get_page();
	cout<<"Press the corresponding number to load a specific config\n";
	cout<<"-------------Page "<<round_up(last_index,8)<<"/"<<round_up(numrows,8)<<" --------\n";
	cout<<"[N]- to go to the next page\n";
	cout<<"[B]- to go back\n";

	int a = waitKey(0);
	if(a=='B' || a=='b'){ // B is pressed, go back
		return 0;
	}else if(a==110 && round_up(last_index,8)<round_up(numrows,8)){ // N is pressed, next page
		system("clear");
		cout<<"N was pressed bitch\n";
		return wait_for_keyMenu();
	}else if(a>='0' && a<='8'){ // number pressed
		system("clear");
		cout<<"You pressed a valid number:\n";
		mysql_query (connect,"select name from Config;");
		int nr = a-'0';
		res_set = mysql_store_result(connect);
		for(unsigned int j=1; j<=nr+8*(round_up(last_index,8)-1);j++){ //load corresponding config
			row= mysql_fetch_row(res_set);
		}
		cout<<"Loading save file named: "<<row[0]<<"\n";

		string res_string = row[0];
		string cmnd = "select list_of_spots,treshold,sensitivity,alpha,beta from Config where name like '"+res_string+"';";
		mysql_query (connect,cmnd.c_str());
		res_set = mysql_store_result(connect);
		row= mysql_fetch_row(res_set);

		load_saveFile(row[0]);
		lowThreshold=stoi(row[1]);
		lowThreshold2=stoi(row[2]);
		alpha_aux = stoi(row[3]);
		bta = stoi(row[4]);
		return 1;
	}
	return 1;
}

int load_menu(){
	system("clear");
	cout<<"----------------CONFIG--------------------\n";
	cout<<"Press one of the following:\n";
	cout<<"(1) - to create a new config file\n";
	cout<<"(2) - to load an already existing config file\n";
	cout<<"(3) - to view all commands for deleting configs etc.\n";
	cout<<"Esc - to exit the program\n";
	cout<<"------------------------------------------\n";

	int kei = waitKey(10);
	while(kei!=0x31 && kei !=0x32 && kei != 0x33 && kei != 0x1B){
		kei = waitKey(10);
	}
	switch (kei) {
		case 0x31:{
			system("clear");
			cout<<"Loading camera, please wait...\n";
			break;
		}
		case 0x32:{
			system("clear");
			cout<<"Current configs in the database:\n";

			mysql_query (connect,"select name from Config;");

			res_set = mysql_store_result(connect);
			numrows = mysql_num_rows(res_set);

			row= mysql_fetch_row(res_set);
			if(wait_for_keyMenu()==0) return 0;
			//waitKey();

			break;
		}
		case 0x33:{
			cout<<"----------------COMMANDS------------------\n";
			cout<<"The following work in the configure camera mode:\n";
			cout<<"[ENTER] - confirms the selection\n";
			cout<<"[LM_CLICK] - select point on the frame\n";
			cout<<"[S] - saves current config\n";
			cout<<"[ESC] - quits the application\n";
			cout<<"------------------------------------------\n";
			cout<<"Press any key to go back...\n";
			waitKey(0); return load_menu();
		}
		case 0x1B:{
				return 0;
		}
		default:{
			cout<<kei<<"\n";
			cout<<"Something unexpected happened\n";
			cout<<"Press any key to quit...\n";
			waitKey(0);
			return 0;
		}
	}
	return 1;
}

