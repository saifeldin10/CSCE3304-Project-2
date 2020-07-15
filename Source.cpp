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

	X.push(nets[i].)



	//*************************************************************** DEBUG Section ***************************************************************//
	/*cout << nets[2].getNetNum() << endl;
	cout << nets[2].getLayer(2) << endl;
	cout << nets[2].getPinx(2) << endl;
	cout << nets[2].getPiny(2);*/
	
	//Closing "input" file
	inFile.close();

	cout << endl;
	system("pause");
}