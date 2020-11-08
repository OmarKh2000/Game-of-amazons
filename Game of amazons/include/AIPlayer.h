#include <iostream>
#include "Player.h"
#include "Game.h"
#include <vector>
#include <unordered_map>
#include <map>

#ifndef AIPlayer_H
#define AIPlayer_H



class AIPlayer : public Player{
	//std::unordered_map<std::vector<int>, double> map;
	//std::unordered_map<std::vector<int>, int, container_hash<std::vector<int>>> map;
		std::map<std::vector<int>, int> map;

	int hashCounter = 0;
public:
	AIPlayer(int time, int playerSymbol);
	bool play(Game& game);

	Move alphaBeta(Game game, int depth, double alpha, double beta, bool maximizingPlayer);

	std::vector<Move> findMoves(Game & game);

	void findMovesByIndex(Move & move, std::vector<Move>& moves, Game & game);

	void findShoots(Move & move, std::vector<Move>& moves, Game & game);

	double evaulate(Game & game);

	int evaulateIndex(int i, int j, Game & game);







};

#endif
