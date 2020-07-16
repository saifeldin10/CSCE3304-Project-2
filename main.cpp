#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "net.h"

//Macros
#define NETSIZE 50
enum status { empty, visited, taken };
struct grid { int netn; int distance_from_source; status st; };

struct Node{
	int x;
	int y;
	int distance;
};

using namespace std;

//Global Variables
net nets[NETSIZE];
grid visits[1000][1000]; //2D Array that keeps track of which areas of the grid have been visited
queue <Node> all;
int x_move[] = { -1,0,1,0 };
int y_move[] = { 0,1,0,-1 };

//Function that connects the pins
void pathfinder(int x_dest, int y_dest, int layer_num, int distance, int netnum){

	int dist_final = distance;
	bool next_up = false, next_down = false, next_left = false, next_right = false;
	int up_check, down_check, right_check, left_check;
	int x_next, y_next;

	visits[x_dest][y_dest].netn = netnum;
	visits[x_dest][y_dest].st = taken;

	while(dist_final != 1){

	for (int ii=0; ii<4; ii++){
		x_next = x_dest + x_move[ii];
		y_next = y_dest + y_move[ii];

		if(ii==0 && (!(x_next < 0 || x_next >= 1000 || y_next < 0 || y_next >= 1000)) && visits[x_next][y_next].st == visited) {
			next_left = true;
			left_check = y_next;
		}
			else if (ii==1 && (!(x_next < 0 || x_next >= 1000 || y_next < 0 || y_next >= 1000)) && visits[x_next][y_next].st == visited){
				next_up = true;
				up_check = y_next;
			}
				else if(ii==2 && (!(x_next < 0 || x_next >= 1000 || y_next < 0 || y_next >= 1000)) && visits[x_next][y_next].st == visited){ 
					next_right = true;
					right_check = x_next;
				}
					else if((!(x_next < 0 || x_next >= 1000 || y_next < 0 || y_next >= 1000)) && visits[x_next][y_next].st == visited) {
						next_left = true;
						left_check = x_next;
					}
	}

	if (layer_num == 1){
		if (next_left && next_right){
			if (visits[left_check][y_dest].distance_from_source<visits[right_check][y_dest].distance_from_source){
				visits[left_check][y_dest].st = taken;
				x_dest = left_check;
			}
				else if(visits[left_check][y_dest].distance_from_source>visits[right_check][y_dest].distance_from_source){
					visits[right_check][y_dest].st = taken;
					x_dest = right_check;				
				}
					else if(visits[right_check][y_dest].st == taken){
						visits[left_check][y_dest].st = taken;
						x_dest = left_check;						
					}
						else {
							visits[right_check][y_dest].st = taken;
							x_dest = right_check;	
						}
		}
		else if (next_left && !next_right){
			visits[left_check][y_dest].st = taken;
			x_dest = left_check;			
		}
		else if (!next_left && next_right){
			visits[right_check][y_dest].st = taken;
			x_dest = right_check;	
		}
		else if (!next_left && !next_right && next_up && next_down){
			if (visits[x_dest][up_check].distance_from_source<visits[x_dest][down_check].distance_from_source){
				visits[x_dest][up_check].st = taken;
				y_dest = up_check;
			}
				else if (visits[x_dest][up_check].distance_from_source<visits[x_dest][down_check].distance_from_source){
				visits[x_dest][down_check].st = taken;
				y_dest = down_check;				
				}
					else if(visits[x_dest][up_check].st == taken){
						visits[x_dest][down_check].st = taken;
						y_dest = down_check;						
					}
						else {
							visits[x_dest][down_check].st = taken;
							y_dest = down_check;	
						}
		}
		else if (!next_left && !next_right && !next_down && next_up){
			visits[x_dest][up_check].st = taken;
			y_dest = up_check;			
		}
		else if (!next_left && !next_right && next_down && !next_up){
			visits[x_dest][down_check].st = taken;
			y_dest = down_check;			
		}
	}

	dist_final = visits[x_dest][y_dest].distance_from_source;
	visits[x_dest][y_dest].netn = netnum;

	}

}

//Coordinate Extractor function
void extract(string str, int i)
{
	stringstream ss;
	ss << str;

	string temp;
	int found; //integer value

	int cc = 0; //Keep track of which value the function is getting
	int inc = 0; //Keep track of number of pins in the net

	while (!ss.eof()) {

		if (cc > 2) //Reset tracker which keeps record of which value is being taken
		{
			cc = 0;
			inc++; //Only increment pin position when all values from preceding pin have been taken
		}

		ss >> temp;
		if (stringstream(temp) >> found)
		{
			if (cc == 0) //Extract Layer value
				nets[i].setLayer(found, inc);
			else 
				if (cc == 1) 
					nets[i].setPx(found, inc); //Extract Pin x value
				else
					nets[i].setPy(found, inc); //Extract Pin y value
		}
		cc++;
		temp = "";
	}
	
	nets[i].numPoints = inc + 1; //Store the number of points there are in the net
}

//MAIN
int main()
{
	string input[50]; //Assuming the input file will not take more than 50 nets
	ifstream inFile;

	//************************************************************ FILE OPEN AND PARSE ************************************************************//

	//Opening "input" file
	inFile.open("input.txt");
	if (!inFile) {
		cout << "File failed to open!" << endl;
		exit(1);
	}

	//Reading from "input" file and writing to variable
	int num = 0;
	while (!inFile.eof())
	{
		getline(inFile, input[num], '\n');
		num++;
	}

	//Parsing input string into Class "Net"
	for (int i = 0; i < num; i++)
	{
		nets[i].setNum(i + 1);
		input[i].erase(0, 4);
		for (int j = 0; j < input[i].size(); j++)
			if (input[i].at(j) == '(' || input[i].at(j) == ')')
				input[i].erase(input[i].begin() + j);
		extract(input[i], i);
	}

	//***************************************************************** ALGORITHM *****************************************************************//


	for (int d=0; d<1000; d++){
		for (int f=0; f<1000; f++){
			visits[d][f].st = empty;
			visits[d][f].distance_from_source = 0;
		}
	}

	for (int i=0; i<NETSIZE; i++){

		int counter2 = 0;

		for (int d=0; d<1000; d++){
			for (int f=0; f<0; f++){
				if (!visits[d][f].st == taken) {
					visits[d][f].st = empty;
					visits[d][f].distance_from_source = 0;
				}
			}
		}
		int counter = nets[i].numPoints;

		while (counter2 != counter){

		int dist = 1;
		int min_dist;

		int x_coord = nets[i].getPinx(counter2);
		int y_coord = nets[i].getPiny(counter2);

		all.push({x_coord, y_coord, dist});

		int x_next, y_next;


		while (!all.empty()){

			Node in_while = all.front();


			for (int ii=0; ii<4; ii++){
				x_next = in_while.x + x_move[ii];
				y_next = in_while.y + y_move[ii];

			
				if (x_next == x_coord && y_next == y_coord){
					min_dist = in_while.distance;
					pathfinder(x_next, y_next, nets[i].getLayer(counter2+1), min_dist, nets[i].getNetNum());
					while (!all.empty()){all.pop();}
				}
					

				
					else if ((!(x_next < 0 || x_next >= 1000 || y_next < 0 || y_next >= 1000)) && visits[x_next][y_next].st == empty){
					all.push({x_next, y_next, in_while.distance+1});
					visits[x_next][y_next].st = visited;
					visits[x_next][y_next].distance_from_source = in_while.distance+1;
					}
			}
		all.pop();
		}
		counter2++;
	}
	}


	//*************************************************************** DEBUG Section ***************************************************************//
	/*cout << nets[1].getNetNum() << endl;
	cout << nets[1].getLayer(1) << endl;
	cout << nets[1].getPinx(1) << endl;
	cout << nets[1].getPiny(1) << endl;
	cout << nets[1].numPoints;*/
	
	//Closing "input" file
	inFile.close();

	cout << endl;

		for (int d=0; d<1000; d++){
		for (int f=0; f<1000; f++){
			cout<<visits[d][f].netn<<"\t";
		}
		cout<<endl;
	}
	system("pause");
	return 0;
}