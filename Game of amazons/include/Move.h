#ifndef Move_H
#define Move_H

struct Move
{
	int fromX = 0, fromY = 0, toX = 0, toY = 0, shootX = 0, shootY = 0;
	Move(int fromX, int fromY, int toX, int toY, int shootX, int shootY)
	{
		this->fromX = fromX;
		this->fromY = fromY;
		this->toX = toX;
		this->toY = toY;
		this->shootX = shootX;
		this->shootY = shootY;


	}
	Move() {}
	int time = 0;
	double evaluation = 0;
	double PV = 0;
	int cuts = 0;
	int depth = 0;
	int hashCounter = 0;
};


#endif
