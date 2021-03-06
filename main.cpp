﻿#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "net.h"
using namespace std;

//Macros
#define NETSIZE 50
#define GRIDSIZE 1000
enum status { available, visited, taken };
struct grid { int netn; int distance_from_source; status st; };

struct Node {
	int x;
	int y;
	int distance;
};



//Global Variables
net nets[NETSIZE];
grid visits[50][GRIDSIZE][GRIDSIZE]; //2D Array that keeps track of which areas of the grid have been visited
queue <Node> all;
queue <Node> all2;
int x_move[] = { -1,0,1,0 };
int y_move[] = { 0,1,0,-1 };

//Utility function to check if we're within boundaries
bool inbound(int x_next, int y_next) {
	return (!(x_next < 0 || x_next >= 1000 || y_next < 0 || y_next >= 1000));
}

//Function that connects the pins
void pathfinder(int x_dest, int y_dest, int layer_num, int distance, int netnum) {
	visits[layer_num][y_dest][x_dest].st = taken;
	bool horizontal_layer = false, vertical_layer = false;
	int dist_final = distance;
	int extra_cost = 3;
	int x_next = x_dest, y_next = y_dest;
	int temp_var_x = 0, temp_var_y = 0;
	int temp_layer = layer_num;
	//if (layer_num % 2 != 0) horizontal_layer = true;
	//else vertical_layer = true;

	visits[layer_num][y_dest][x_dest].netn = netnum;


	while (dist_final > 1) {
		visits[layer_num][x_next][y_next].st = taken;
		bool up_check = false, down_check = false, right_check = false, left_check = false, tie_break = false;
		int right_diff = 0, horizontal_direction = 0, horizontal_difference = 0, vertical_difference = 0, vertical_direction = 0, up_diff = 0, vertical_cost = 0, horizontal_cost = 0;
		int up_val = 0, down_val = 0, left_val = 0, right_val = 0;
		int x_temp = x_next, y_temp = y_next;
		temp_var_x = x_next;
		temp_var_y = y_next;
		//visits[layer_num][x_next][y_next].netn = netnum;
		visits[temp_layer][x_next][y_next].netn = netnum;
		if (temp_layer % 2 != 0) horizontal_layer = true;
		else vertical_layer = true;
		

		//Check the source's adjacent cells, are they visited? if yes, what are their values?
		if (inbound(x_next, y_next - 1) && visits[layer_num][y_next - 1][x_next].st == visited) {
			up_check = true;
			up_val = visits[layer_num][y_next - 1][x_next].distance_from_source;
		}
		if (inbound(x_next, y_next + 1) && visits[layer_num][y_next + 1][x_next].st == visited) {
			down_check = true;
			down_val = visits[layer_num][y_next + 1][x_next].distance_from_source;
		}
		if (inbound(x_next - 1, y_next) && visits[layer_num][y_next][x_next - 1].st == visited) {
			left_check = true;
			left_val = visits[layer_num][y_next][x_next - 1].distance_from_source;
		}
		if (inbound(x_next + 1, y_next) && visits[layer_num][y_next][x_next + 1].st == visited) {
			right_check = true;
			right_val = visits[layer_num][y_next][x_next + 1].distance_from_source;
		}

		//To eliminate two directions, we have to check the net ditance horizontally and vertically
		horizontal_difference = right_val - left_val;
		vertical_difference = up_val - down_val;

		//Choosing whether to go right or left. 
		if (horizontal_difference > 0) {
			if (left_check) x_temp = x_temp - 1;
			else x_temp = x_temp + 1;
		}
		else if (horizontal_difference < 0) {
			if (right_check) x_temp = x_temp + 1;
			else x_temp = x_temp - 1;
		}
		else x_temp = x_temp;

		//Choosing whether to go up or down. 
		if (vertical_difference > 0) {
			if (down_check) y_temp = y_temp + 1;
			else y_temp = y_temp - 1;
		}
		else if (vertical_difference < 0) {
			if (up_check) y_temp = y_temp - 1;
			else y_temp = y_temp + 1;
		}
		else y_temp = y_temp;



		vertical_cost = visits[layer_num][y_temp][x_next].distance_from_source;
		horizontal_cost = visits[layer_num][y_next][x_temp].distance_from_source;

		//now comparing whether to move horizontally or vertically
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

		if (temp_var_x != x_next) {
			if (vertical_layer) temp_layer = layer_num;
			else {
				temp_layer = layer_num + 1;
				//visits[layer_num][x_next][y_next].netn = netnum;
			}
		}
		else {
			if (vertical_layer) {
				temp_layer = layer_num - 1;
				//visits[layer_num][x_next][y_next].netn = netnum;
			}
			else temp_layer = layer_num;
		}

		//visits[layer_num][x_next][y_next].netn = netnum;
		visits[temp_layer][x_next][y_next].netn = netnum;
		dist_final = visits[layer_num][y_next][x_next].distance_from_source;
		visits[temp_layer][x_next][y_next].st = taken;


	}
	visits[temp_layer][x_next][y_next].netn = netnum;
}



//Function that implements lee's algorithm
void lee(int x_coord, int y_coord, int first_layer, int x_destination, int y_destination, int second_layer, int dist, int i) {

	bool horizontal = false, vertical = false, horizontal_layer = false, vertical_layer = false;
	while (!all2.empty()) { all2.pop(); }
	all.push({ x_coord, y_coord, dist }); //Pushing source into queue
	all2.push({ x_coord, y_coord, dist }); //Pushing source into the second queue
	visits[first_layer][y_coord][x_coord].distance_from_source = all.front().distance;
	int new_first_layer = first_layer;
	int new_second_layer = second_layer;
	if (first_layer < second_layer) new_first_layer++;
	else new_first_layer--;
	if (first_layer % 2 != 0) horizontal_layer = true;
	else vertical_layer = true;

	int temp_layer;
	//Targetfinding loop, keeps running until the source gets popped again
	while (!all.empty())
	{

		int x_next = 0, y_next = 0, min_dist = 0;
		bool via = false;
		Node in_while = all.front(); //Keeps track of the front node of the queue
		visits[first_layer][in_while.y][in_while.x].st = visited;



		for (int ii = 0; ii < 4; ii++)
		{

			x_next = in_while.x + x_move[ii]; //Keeps track of the eventual moves in the x direction 
			y_next = in_while.y + y_move[ii]; //Keeps track of the eventual moves in the y direction
			min_dist = in_while.distance + 1;


			//The next statement is the essence of the multi-layering simulation, until we are on the same layer we are going to install vias on the source coordinates
			//on the initial layer and all layers in between
			if (first_layer != second_layer) {
				visits[first_layer][x_coord][y_coord].st = taken;
				visits[first_layer][x_coord][y_coord].netn = nets[i].getNetNum();
			}
			else new_first_layer = first_layer;

			visits[first_layer][x_coord][y_coord].netn = nets[i].getNetNum();
			visits[new_first_layer][y_coord][x_coord].distance_from_source = 1;

			if (ii % 2 != 0) {
				if (vertical_layer) temp_layer = first_layer - 1;
				else temp_layer = first_layer;
			}
			else {
				if (vertical_layer) temp_layer = first_layer;
				else temp_layer = first_layer + 1;
			}


			//If the destination has been found then backtrack using pathfinder function
			if (x_next == x_destination && y_next == y_destination)
			{
				visits[new_first_layer][y_next][x_next].distance_from_source = min_dist;
				pathfinder(x_next, y_next, new_first_layer, min_dist, nets[i].getNetNum());
				while (!all.empty()) { all.pop(); }
			}

			//If destination is not found, we push a node with the new coordinates to the queue, we edit the distance and label it as visited
			else if (inbound(x_next, y_next) && visits[temp_layer][y_next][x_next].st == available)
			{
				visits[temp_layer][y_next][x_next].distance_from_source = min_dist;
				visits[new_first_layer][y_next][x_next].distance_from_source = min_dist;
				all.push({ x_next, y_next, min_dist });
				visits[new_first_layer][y_next][x_next].st = visited;
				visits[temp_layer][y_next][x_next].st = visited;
			}
		}
		if (!all.empty())
			all.pop();
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
	ofstream outFile;

	//************************************************************ FILE OPEN AND PARSE ************************************************************//

	//Opening "input" and "output" file
	inFile.open("C:\\Users\\Youssef Ragai\\Desktop\\New folder\\CMakeProject1\\input.txt");
	outFile.open("output.txt");
	if (!inFile) {
		cout << "RUMBLE! File failed to open!" << endl;
		exit(1);
	}
	if (!outFile) {
		cout << "RUMBLE! File failed to open!" << endl;
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
		input[i].erase(0, 5);
		for (int j = 0; j < input[i].size(); j++)
			if (input[i].at(j) == '(' || input[i].at(j) == ')')
				input[i].erase(input[i].begin() + j);
		extract(input[i], i);
	}

	//***************************************************************** ALGORITHM *****************************************************************//


	//Setting all values of the grid to 0
	for (int d = 0; d < GRIDSIZE; d++) {
		for (int f = 0; f < GRIDSIZE; f++) {
			for (int e = 0; e < 50; e++) {
				visits[e][f][d].st = available;
				visits[e][f][d].distance_from_source = 0;
			}
		}
	}

	//Loop that goes through the list of nets
	for (int i = 0; i < num; i++) {

		int counter2 = 0; //Counter that keeps track of the nets that have been connected 
		int next_destination = 0;
		int counter = nets[i].numPoints - 1; //Counter that keeps track of the number of points in the net

		while (counter2 < counter)
		{
			bool via = false;
			int dist = 1;
			int min_dist = 0;
			next_destination = next_destination + 1;


			for (int d = 0; d < GRIDSIZE; d++) {
				for (int f = 0; f < GRIDSIZE; f++) {
					for (int e = 0; e < 50; e++) {
						if (visits[e][f][d].st != taken) {
							visits[e][f][d].st = available;
							visits[e][f][d].distance_from_source = 0;
						}
					}
				}
			}

			//Setting the values of the source and the destination accordingly
			int x_coord = nets[i].getPinx(0);
			int y_coord = nets[i].getPiny(0);
			int first_layer = nets[i].getLayer(0);
			int x_destination = nets[i].getPinx(next_destination);
			int y_destination = nets[i].getPiny(next_destination);
			int second_layer = nets[i].getLayer(next_destination);

			//calling lee's algorithm function, passing i is important so we can extract local data
			lee(x_coord, y_coord, first_layer, x_destination, y_destination, second_layer, dist, i);

			counter2++;
		}
	}

	//Output the taken cells to display the connection paths for each net
	int n = 0; //Keep track of which net the output cycle is on
	while (n < num)
	{
		cout << "net" << n + 1 << " ";
		outFile << "net" << n + 1 << " ";
		for (int i = 0; i < 50; i++)	//Assuming there are only three layers
		{
			for (int j = 0; j < GRIDSIZE; j++)
			{
				for (int k = 0; k < GRIDSIZE; k++)
				{
					//Print if the cell traversed is taken and belongs to the net being outputted
					if (visits[i][j][k].netn == n + 1)
					{
						cout << "(" << i +1 << ", " << k  << ", " << j  << ") ";
						outFile << "(" << i +1 << ", " << k  << ", " << j  << ") ";
					}

				}
			}
		}
		cout << endl;
		outFile << endl;
		n++;
	}

	//Closing "input" file
	inFile.close();
	outFile.close();

	cout << endl;
	//*************************************************************** DEBUG Section ***************************************************************//

	
	cout << "Program terminated..." << endl;
	system("pause");
	return 0;
}
