#include <iostream>
#ifndef Game_H
#define Game_H
#include "Move.h"

class Game {
	int **board=NULL;
	int p1Time;
	int p2Time;
	int size;
	int time;
	int currentTurn;
	int round = 1;
public:

	Game(int size,int time);

	//Game(int size, int time,int** board);

	int getSize() const;
	int getround();
	int getIndex(int i, int j) const;

	int checkEnd();
	~Game();
	int getTime() const;
	Game(const Game& game);

	void applyMove(Move & move, int symbol);

	void applyandPrintMove(Move & move, int symbol);


	int updatePlayerTime(int index, int time);

	void applytemp(Move & move, int symbol);

	void unapplytemp(Move & move, int symbol);

	bool checkCanMove(int i, int j);

	bool checkCanShoot(int i, int j);

	void printMove(Move & move);

	int getTurn() const;


};

#endif