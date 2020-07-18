#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "net.h"
#include "net.h"

//Macros
#define NETSIZE 50
enum status { available, visited, taken };
struct grid { int netn; int distance_from_source; status st; };

struct Node {
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

//Utility function to check if we're within boundaries
bool inbound(int x_next, int y_next) {
	return (!(x_next < 0 || x_next >= 1000 || y_next < 0 || y_next >= 1000));
}

//Function that connects the pins
void pathfinder(int x_dest, int y_dest, int layer_num, int distance, int netnum) {
	visits[y_dest][x_dest].st = taken;

	int dist_final = distance;
	int extra_cost = 3;
	int x_next = x_dest, y_next = y_dest;

	visits[x_dest][y_dest].netn = netnum;


	while (dist_final > 1) {
		visits[x_next][y_next].st = taken;
		bool up_check = false, down_check = false, right_check = false, left_check = false, tie_break = false;
		int right_diff = 0, horizontal_direction = 0, horizontal_difference = 0, vertical_difference = 0, vertical_direction = 0, up_diff = 0, vertical_cost = 0, horizontal_cost = 0;
		int up_val = 0, down_val = 0, left_val = 0, right_val = 0;
		int x_temp = x_next, y_temp = y_next;
		visits[x_next][y_next].netn = netnum;


		if (inbound(x_next, y_next - 1) && visits[y_next - 1][x_next].st == visited) {
			up_check = true;
			up_val = visits[y_next - 1][x_next].distance_from_source;
		}
		if (inbound(x_next, y_next + 1) && visits[y_next +1][x_next].st == visited) {
			down_check = true;
			down_val = visits[y_next + 1][x_next].distance_from_source;
		}
		if (inbound(x_next - 1, y_next) && visits[y_next][x_next - 1].st == visited) {
			left_check = true;
			left_val = visits[y_next][x_next - 1].distance_from_source;
		}
		if (inbound(x_next +1, y_next) && visits[y_next][x_next +1].st == visited) {
			right_check = true;
			right_val = visits[y_next ][x_next +1].distance_from_source;
		}

		horizontal_difference = right_val - left_val;
		vertical_difference = up_val - down_val;

		if (horizontal_difference > 0) {
			if (left_check) x_temp = x_temp - 1;
			else x_temp = x_temp + 1;
		}
		else if (horizontal_difference < 0) {
			if (right_check) x_temp = x_temp + 1;
			else x_temp = x_temp - 1;
		}
		else x_temp = x_temp;


		if (vertical_difference > 0) {
			if (down_check) y_temp = y_temp + 1;
			else y_temp = y_temp - 1;
		}
		else if (vertical_difference < 0) {
			if (up_check) y_temp = y_temp - 1;
			else y_temp = y_temp + 1;
		}
		else y_temp = y_temp;


		if (layer_num == 1) {
			vertical_cost = visits[y_temp][x_next].distance_from_source + extra_cost;
			horizontal_cost = visits[y_next][x_temp].distance_from_source;
		}
			else if (layer_num == 2) {
			vertical_cost = visits[y_temp][x_next].distance_from_source;
			horizontal_cost = visits[y_next][x_temp].distance_from_source + extra_cost;
			}

		if (horizontal_cost > vertical_cost)
		{
			if (vertical_difference != 0)y_next = y_temp;
			else  x_next = x_temp;
		}
		else 
		{
			if (horizontal_difference != 0)x_next = x_temp;
			else y_next = y_temp;
		}

		dist_final = visits[y_next][x_next].distance_from_source;
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
	inFile.open("C:\\Users\\Youssef Ragai\\Desktop\\New folder\\CMakeProject1\\input.txt");
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

	//Setting all values of the grid to 0
	for (int d = 0; d < 1000; d++) {
		for (int f = 0; f < 1000; f++) {
			visits[f][d].st = available;
			visits[f][d].distance_from_source = 0;
		}
	}

	//Loop that goes through the list of nets
	for (int i = 0; i < num; i++) {

		int counter2 = 0; //Counter that keeps track of the nets that have been connected 
		int next_destination = 0;
		int counter = nets[i].numPoints -1; //Counter that keeps track of the number of points in the net

		while (counter2 < counter)
		{
			int dist = 1;
			int min_dist = 0;
			next_destination = next_destination + 1;

			for (int d = 0; d < 1000; d++) {
				for (int f = 0; f < 1000; f++) {
					if (visits[f][d].st != taken) {
						visits[f][d].st = available;
						visits[f][d].distance_from_source = 0;
					}
				}
			}

			//Setting the values of the source and the destination accordingly
			int x_coord = nets[i].getPinx(0);
			int y_coord = nets[i].getPiny(0);
			int x_destination = nets[i].getPinx(next_destination);
			int y_destination = nets[i].getPiny(next_destination);


			all.push({ x_coord, y_coord, dist }); //Pushing source into queue
			visits[y_coord][x_coord].distance_from_source = all.front().distance;

			int x_next = 0, y_next = 0;

			//Pathfinding loop, keeps running until the source gets popped again
			while (!all.empty())
			{

				Node in_while = all.front(); //Keeps track of the front node of the queue
				visits[in_while.y][in_while.x].st = visited;
				

				for (int ii = 0; ii < 4; ii++)
				{
					x_next = in_while.x + x_move[ii]; //Keeps track of the eventual moves in the x direction 
					y_next = in_while.y + y_move[ii]; //Keeps track of the eventual moves in the y direction
					min_dist = in_while.distance + 1;

					//If the destination has been found then backtrack using pathfinder function
					if (x_next == x_destination && y_next == y_destination)
					{
						visits[y_next][x_next].distance_from_source = min_dist;
						pathfinder(x_next, y_next, 1, min_dist, nets[i].getNetNum());
						while (!all.empty()) { all.pop(); }
					}

					/*if (inbound(x_next, y_next) && counter2 != 0) cout << "true_inbound" << endl;
					if (visits[y_next][x_next].st == available && counter2 != 0) cout << "it is available" << endl;*/


					else if (inbound(x_next,y_next) && visits[y_next][x_next].st == available)
					{
						visits[y_next][x_next].distance_from_source = min_dist;
						all.push({ x_next, y_next, min_dist });
						visits[y_next][x_next].st = visited;
					}

				}
				if (!all.empty())
					all.pop();
			}
			counter2++;
		}
	}


	//*************************************************************** DEBUG Section ***************************************************************//

	//Closing "input" file
	inFile.close();

	cout << endl;

	for (int d = 0; d < 100; d++) {
		for (int f = 0; f < 100; f++) {
			cout << visits[f][d].netn;
		}
		cout << endl;
	}
	cout << "Program ended";
	system("pause");
	return 0;
}