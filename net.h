#pragma once
#include <iostream>

using namespace std;

struct coord { int layer; int pinx; int piny; };

class net
{
private:
	int netNum;
	coord netCoor[20]; //Assuming there won't be more than 20 points in the net

public:
	int numPoints = 0;
	
	bool setNum(int num)
	{
		netNum = num;
		return true;
	}

	bool setLayer(int layer, int pos)
	{
		netCoor[pos].layer = layer;
		return true;
	}

	bool setPx(int px, int pos)
	{
		netCoor[pos].pinx = px;
		return true;
	}

	bool setPy(int py, int pos)
	{
		netCoor[pos].piny = py;
		return true;
	}

	int getLayer(int pos)
	{
		return netCoor[pos].layer;
	}

	int getPinx(int pos)
	{
		return netCoor[pos].pinx;
	}

	int getPiny(int pos)
	{
		return netCoor[pos].piny;
	}

	int getNetNum()
	{
		return netNum;
	}
};

