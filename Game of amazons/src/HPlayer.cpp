#include "HPlayer.h"
#include <string.h>
#include "Game.h"
#include <chrono> 
#include <iostream>
#include <string>

#include <algorithm> 
using namespace std::chrono;

HPlayer::HPlayer():Player(INT_MAX,2)
{
}

bool HPlayer::play(Game& game)		// take human play.
{
	auto start = high_resolution_clock::now();

	if (game.checkEnd())
		return false;
	std::string s ;
	//std::cin >> s;
	std::getline(std::cin, s);
	Move move = stringToMove(s);
	if (move.fromX == move.toX && move.fromY == move.toY)
		return false;

	if (game.updatePlayerTime(playerSymbol, move.time) < 0)
		return false;

	game.applyandPrintMove(move, playerSymbol);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	move.time = duration.count() / 1000000;
	return true;
}



Move HPlayer::stringToMove(std::string s)
{
 	Move move;
	if (s.length() < 1)
		return move;
	move.fromX = int(s[0] - 65);
	move.fromY = int(s[1]-49);

	move.toX = int(s[3] - 65);
	move.toY = int(s[4] - 49);

	move.shootX = int(s[6] - 65);
	move.shootY= int(s[7] - 49);

	return move;

}