#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "net.h"

//Macros
#define NETSIZE 50
enum status { empty, visited, taken };
struct grid { int netn; status st; };

using namespace std;

//Global Variables
net nets[NETSIZE];
grid visits[1000][1000]; //2D Array that keeps track of which areas of the grid have been visited
queue <int> X, Y;
int x_move[] = { -1,0,1,0 };
int y_move[] = { 0,1,0,-1 };

//Function that connects the pins
void pathfinder(int x_source, int y_source,int x_dest, int y_dest, int layer_num){

	int x_go_right = x_dest - x_source;
	int y_go_up = y_dest - y_source;

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
void main()
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
			}
		}

	for (int i=0; i<NETSIZE; i++){

		int counter = nets[i].numPoints;
		int counter2 = 0;

		X.push(nets[i].getPinx(counter2));
		Y.push(nets[i].getPiny(counter2));

		int x_source, y_source, x_next, y_next;

		/*for (int d=0; d<1000; d++){
			for (int f=0; f<0; f++){
				if (!visits[d][f].st = taken) visits[d][f].st = empty;
			}
		}*/

		while (!X.empty()){

			x_source = X.front();
			y_source = Y.front();

			for (int ii=0; ii<4; ii++){
				x_next = x_source + x_move[ii];
				y_next = y_source + y_move[ii];

			
				if (x_next == nets[i].getPinx(counter2+1) && y_next == nets[i].getPiny(counter2+1)){

					visits[x_next][y_next].st = visited;
					pathfinder(x_source, y_source, x_next, y_next, nets[i].getPiny(counter2+1));

				}
					else if ((!(x_next < 0 || x_next =>1000 || y_next < 0 || y_next =>1000)) && visits[x_next][y_next].st = empty){
					X.push(x_next);
					Y.push(y_next);
					visits[x_next][y_next].st = visited;
					}
			}
		X.pop();
		Y.pop();
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
	system("pause");
}