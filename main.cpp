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
	visits[x_dest][y_dest].st = taken;

	int dist_final = distance;
	int extra_cost = 3;
	int x_next = x_dest, y_next = y_dest;

	visits[x_dest][y_dest].netn = netnum;
	visits[x_dest][y_dest].st = taken;

	while (dist_final > 1) {

		bool up_check = false, down_check = false, right_check = false, left_check = false, tie_break = false;
		int right_diff = 0, horizontal_direction = 0, horizontal_difference = 0, vertical_difference = 0, vertical_direction = 0, up_diff = 0;
		/*bool inbound_check = false;
		bool visit_check = false;*/

		//cout << endl << visits[x_next][y_next - 1].st << endl;

		if (inbound(x_next - 1, y_next) && visits[x_next - 1][y_next].st == visited) {
			left_check = true;
		}
		if (inbound(x_next, y_next + 1) && visits[x_next][y_next +1].st == visited) {
			down_check = true;
		}
		if (inbound(x_next +1, y_next) && visits[x_next +1][y_next].st == visited) {
			right_check = true;
		}
		if (inbound(x_next, y_next -1) && visits[x_next][y_next -1].st == visited) {
			up_check = true;
			//inbound_check = inbound(x_next, y_next - 1);
			//visit_check = visits[x_next][y_next - 1].st == visited;
		}

		//if (inbound_check) cout << "inbound_check" << endl;
		//if (visit_check) cout << "visit_check" << endl;

		if (left_check && right_check) {
			horizontal_difference = visits[x_next + 1][y_next].distance_from_source - visits[x_next - 1][y_next].distance_from_source;
			if (horizontal_difference > 0) {
				right_diff = x_next - 1;
				horizontal_direction = visits[x_next - 1][y_next].distance_from_source;
			}
			else if (horizontal_difference < 0) {
				right_diff = x_next + 1;
				horizontal_direction = visits[x_next + 1][y_next].distance_from_source;
			}
		}
			else if (!left_check && right_check) {
				horizontal_direction = visits[x_next + 1][y_next].distance_from_source;
				right_diff = x_next + 1;
			}
				else if (left_check && !right_check) {
					horizontal_direction = visits[x_next - 1][y_next].distance_from_source;
					right_diff = x_next - 1;
				}
		if (up_check && down_check) {
			vertical_difference = visits[x_next][y_next +1].distance_from_source - visits[x_next][y_next - 1].distance_from_source;
			if (vertical_difference > 0) {
				up_diff = x_next - 1;
				vertical_direction = visits[x_next][y_next - 1].distance_from_source;
			}
			else if (vertical_difference < 0) {
				up_diff = x_next + 1;
				vertical_direction = visits[x_next][y_next + 1].distance_from_source;
			}
		}
			else if (!up_check && down_check) {
				horizontal_direction = visits[x_next][y_next -1].distance_from_source;
				up_diff = y_next - 1;
			}
				else if (up_check && !down_check) {
				horizontal_direction = visits[x_next][y_next + 1].distance_from_source;
				up_diff = y_next + 1;
				}

		if (layer_num == 1) {
			vertical_direction = vertical_direction + extra_cost;
			tie_break = true;
		}
			else if (layer_num == 2) {
				horizontal_direction = horizontal_direction + extra_cost;
			}

		if (vertical_direction > horizontal_direction) x_dest = right_diff;
		else if (vertical_direction < horizontal_direction) y_dest = up_diff;
		else {
			if (tie_break) x_dest = right_diff;
			else y_dest = up_diff;
		}

		dist_final = visits[x_dest][y_dest].distance_from_source;
		visits[x_dest][y_dest].st = taken;
		for (int d = 0; d < 50; d++) {
			for (int f = 0; f < 50; f++) {
				cout << visits[d][f].st;
			}
			cout << endl;
		}
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
			visits[d][f].st = available;
			visits[d][f].distance_from_source = 0;
		}
	}

	//Loop that goes through the list of nets
	for (int i = 0; i < num; i++) {

		int counter2 = 0; //Counter that keeps track of the nets that have been connected 
		int next_destination = counter2 + 1;
		//Set all untaken cells back to available to prepare for the next net values
		for (int d = 0; d < 1000; d++) {
			for (int f = 0; f < 1000; f++) {
				if (!visits[d][f].st == taken) {
					visits[d][f].st = available;
					visits[d][f].distance_from_source = 0;
				}
			}
		}

		int counter = nets[i].numPoints; //Counter that keeps track of the number of points in the net

		while (counter2 < counter - 1)
		{
			int dist = 1;
			int min_dist;

			//Setting the values of the source and the destination accordingly
			int x_coord = nets[i].getPinx(counter2);
			int y_coord = nets[i].getPiny(counter2);
			int x_destination = nets[i].getPinx(next_destination);
			int y_destination = nets[i].getPiny(next_destination);


			all.push({ x_coord, y_coord, dist }); //Pushing source into queue
			visits[x_coord][y_coord].distance_from_source = all.front().distance;

			int x_next, y_next, huh = 0;

			//Pathfinding loop, keeps running until the source gets popped again
			while (!all.empty())
			{

				Node in_while = all.front(); //Keeps track of the front node of the queue
				visits[in_while.x][in_while.y].st = visited;
				

				for (int ii = 0; ii < 4; ii++)
				{
					x_next = in_while.x + x_move[ii]; //Keeps track of the eventual moves in the x direction 
					y_next = in_while.y + y_move[ii]; //Keeps track of the eventual moves in the y direction
					min_dist = in_while.distance + 1;

					//If the destination has been found then backtrack using pathfinder function
					if (x_next == x_destination && y_next == y_destination)
					{
						visits[x_next][y_next].distance_from_source = min_dist;
						//pathfinder(x_next, y_next, nets[i].getLayer(counter2 + 1), min_dist, nets[i].getNetNum());
							for (int d = 0; d < 30; d++) {
								for (int f = 0; f < 50; f++) {
									cout << visits[d][f].distance_from_source;
								}
								cout << endl;
							}
							//cout << x_next << "\t" << y_next << "\t" << min_dist << endl;

						pathfinder(x_next, y_next, 1, min_dist, nets[i].getNetNum());
						while (!all.empty()) { all.pop(); }
					}



					else if (inbound(x_next,y_next) && visits[x_next][y_next].st == available)
					{
						visits[x_next][y_next].distance_from_source = min_dist;
						all.push({ x_next, y_next, min_dist });
						visits[x_next][y_next].st = visited;
						//cout << x_next << "\t" << y_next << "\t" << nets[i].getLayer(counter2 + 1) << "\t" << min_dist << "\t" << nets[i].getNetNum() << "\t I am not path finder" << endl;
					}

					//else cout<<"invalid"<<endl;
				}
				if (!all.empty())
					all.pop();
				huh++;
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

	/*for (int d = 0; d < 100; d++) {
		for (int f = 0; f < 100; f++) {
			cout << visits[d][f].netn << "\t";
		}
		cout << endl;
	}*/
	cout << "Program ended";
	system("pause");
	return 0;
}