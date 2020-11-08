#include "AIPlayer.h"
#include <vector>
#include "Game.h"
#include <algorithm> 
#include <chrono> 
using namespace std::chrono;




AIPlayer::AIPlayer(int time, int playerSymbol) : Player(time, playerSymbol)
{



}

bool AIPlayer::play(Game& game)	// function that's respoosbile for choosing the AI player paly.
{
	auto start = high_resolution_clock::now();
	hashCounter = 0;
	int depth = std::max(1,2*(game.getround()*game.getTime()));
	//if (game.getSize() == 10)
	//	depth = depth / 2;
	if (game.getSize() == 6)
		depth = depth * 5;
	Move move=alphaBeta( game,  depth, DBL_MIN, DBL_MAX, true);
	if (move.fromX == move.toX && move.fromY == move.toY)
	{
		return false;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	
	move.time = duration.count()/1000000;
	if (game.updatePlayerTime(playerSymbol, move.time) < 0)
		return false;
	//move.time = duration.count() / 1000;
	int plays = 0;
	plays = game.getSize() == 10 ? 92 : 32;
	move.depth = std::min(depth, plays-game.getround()/2);
	move.hashCounter = hashCounter;
	move.PV = evaulate(game);
	game.applyandPrintMove(move,playerSymbol);
	

	return true;



}

Move AIPlayer::alphaBeta(Game game, int depth, double alpha, double beta, bool maximizingPlayer)	// Alpha-beta search function
{
	Move opt;
	std::vector<Move> moves = findMoves(game);
	Game tempGame(game);
	double value;
	Move currentMove;
	Move result;
	if (depth == 0 || moves.size()<=0)			// alpha beta recursive.
	{
		opt.evaluation = evaulate(game);
		return opt;
	}

	if (maximizingPlayer)		// if max node
	{
		opt.evaluation = INT_MIN;
		for (int i = 0; i < moves.size(); i++)
		{
		
			//currentMove = moves[rand() % moves.size()];
			currentMove = moves[i];

			tempGame.applyMove(currentMove,playerSymbol);
			result = alphaBeta(tempGame,depth-1,alpha,beta,false);
			opt.cuts += result.cuts;
			if(opt.evaluation < result.evaluation)
			{
				opt = currentMove;
				opt.evaluation = result.evaluation;
			//	opt.depth = std::min(opt.depth, depth);

			}
			alpha = std::max(alpha, opt.evaluation);
			if (alpha >= beta)
			{//beta cut-off
				opt.cuts++;
				break;
			}
		}
	}
	else
	{
		opt.evaluation = DBL_MAX;
		for (int i = 0; i < moves.size(); i++)
		{
			//currentMove = moves[rand() % moves.size()];
			currentMove = moves[i];
			tempGame.applyMove(currentMove, playerSymbol);
			result = alphaBeta(tempGame, depth - 1, alpha, beta, false);
			if (opt.evaluation > result.evaluation)
			{
				opt = currentMove;
				opt.evaluation = result.evaluation;
			//	opt.depth = std::min(opt.depth, depth);
			}
			opt.cuts += result.cuts;
			alpha = std::min(alpha, opt.evaluation);
			if (alpha <= beta)		//beta cut-off
			{	
				opt.cuts++;
				break;
			}
		}
	}
///	opt.depth = depth;
	return opt;

}

std::vector<Move> AIPlayer::findMoves(Game& game)		//function to find avilabe moves
{
	std::vector<Move> moves;
	std::vector<std::vector<int>> queens;
	Move currentMove;
	int size = game.getSize();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (game.getIndex(i, j) == playerSymbol) {
				queens.push_back({ i,j });
			}
		}
	}

	for (int i = 0; i < queens.size(); i++)
	{
		currentMove.fromX = queens[i][0];
		currentMove.fromY = queens[i][1];
		findMovesByIndex(currentMove, moves,game);
	}
	
	return moves;
	


}

void AIPlayer::findMovesByIndex(Move& move, std::vector<Move>& moves,Game& game)	// Given a queen indice, The function will search for moves.
{
	int i = move.fromX;
	int j = move.fromY;
	int size = game.getSize();
	Move temp = move;
	for (int k = i+1; k < size; k++)		// Check if player can move right then shoot.
	{
		if (game.getIndex(k, j) == 0)
		{
			temp = move;
			temp.toX = k;
			temp.toY = j;
			game.applytemp(temp,playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp,playerSymbol);
		}
		else {
			break;
		}
	}

	for (int k = i-1; k >= 0; k--)		// Check if player can move left then shoot.
	{
		if (game.getIndex(k, j) == 0)
		{
			temp = move;
			temp.toX = k;
			temp.toY = j;
			game.applytemp(temp, playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp, playerSymbol);
		}
		else {
			break;
		}
	}

	for (int k = i-1, l = j-1; k >= 0 && l >= 0; k--, l--)		// Check if player can move  left-down then shoot.	
	{
		if (game.getIndex(k, l) == 0 )
		{
			temp = move;
			temp.toX = k;
			temp.toY = l;
			game.applytemp(temp, playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp, playerSymbol);
		}
		else {
			break;
		}
	}

	for (int k = i+1, l = j+1; k < size && l < size; k++, l++)		// Check if player can move to the right-up then shoot.
	{
		if (game.getIndex(k, l) == 0 )
		{
			temp = move;
			temp.toX = k;
			temp.toY = l;
			game.applytemp(temp, playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp, playerSymbol);
		}
		else {
			break;
		}
	}

	for (int k = j+1; k < size; k++)		// Check if player can move to the up then shoot.
	{
		if (game.getIndex(i, k) == 0 )
		{
			temp = move;
			temp.toX = i;
			temp.toY = k;
			game.applytemp(temp, playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp, playerSymbol);
		}
		else {
			break;
		}
	}


	for (int k = j-1; k >= 0; k--)		// Check if player can move to the down then shoot.
	{
		if (game.getIndex(i, k) == 0)
		{
			temp = move;
			temp.toX = i;
			temp.toY = k;
			game.applytemp(temp, playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp, playerSymbol);
		}
		else {
			break;
		}
	}


	for (int k = i-1, l = j+1; k >= 0 && l < size; k--, l++)		// Check if player can move to the left-up then shoot.
	{
		if (game.getIndex(k, l) == 0 )
		{
			temp = move;
			temp.toX =k;
			temp.toY =l;
			game.applytemp(temp, playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp, playerSymbol);
		}
		else {
			break;
		}
	}
	for (int k = i+1, l = j-1; k < size && l >=0; k++, l--)		// Check if player can move to the right-down then shoot.
	{
		if (game.getIndex(k, l) == 0)
		{
			temp = move;
			temp.toX = k;
			temp.toY = l;
			game.applytemp(temp, playerSymbol);
			findShoots(temp, moves, game);
			game.unapplytemp(temp, playerSymbol);
		}
		else {
			break;
		}
	}



}

void AIPlayer::findShoots(Move & move, std::vector<Move>& moves, Game & game)		// given indices find where the queen can shoot it's arrows.
{
	int i = move.toX;
	int j = move.toY;
	int size = game.getSize();
	Move temp = move;
	for (int k = i + 1; k < size; k++)		// Check if player can move right then shoot.
	{
		if (game.getIndex(k, j) == 0)
		{
			temp = move;
			temp.shootX = k;
			temp.shootY = j;
			moves.push_back(temp);
		}
		else {
			break;
		}
	}

	for (int k = i - 1; k >= 0; k--)		// Check if player can move left then shoot.
	{
		if (game.getIndex(k, j) == 0)
		{
			temp = move;
			temp.shootX = k;
			temp.shootY = j;
			moves.push_back(temp);

		}
		else {
			break;
		}
	}

	for (int k = i - 1, l = j - 1; k >= 0 && l >= 0; k--, l--)		// Check if player can move  left-down then shoot.	
	{
		if (game.getIndex(k, l) == 0)
		{
			temp = move;
			temp.shootX = k;
			temp.shootY = l;
			moves.push_back(temp);
		}
		else {
			break;
		}
	}

	for (int k = i + 1, l = j + 1; k < size && l < size; k++, l++)		// Check if player can move to the right-up then shoot.
	{
		if (game.getIndex(k, l) == 0)
		{
			temp = move;
			temp.shootX = k;
			temp.shootY = l;
			moves.push_back(temp);
		}
		else {
			break;
		}
	}

	for (int k = j + 1; k < size; k++)		// Check if player can move to the up then shoot.
	{
		if (game.getIndex(i, k) == 0)
		{
			temp = move;
			temp.shootX = i;
			temp.shootY = k;
			moves.push_back(temp);
		}
		else {
			break;
		}
	}


	for (int k = j - 1; k >= 0; k--)		// Check if player can move to the down then shoot.
	{
		if (game.getIndex(i, k) == 0)
		{
			temp = move;
			temp.shootX = i;
			temp.shootY = k;
			moves.push_back(temp);
		}
		else {
			break;
		}
	}


	for (int k = i - 1, l = j + 1; k >= 0 && l < size; k--, l++)		// Check if player can move to the left-up then shoot.
	{
		if (game.getIndex(k, l) == 0)
		{
			temp = move;
			temp.shootX = k;
			temp.shootY = l;
			moves.push_back(temp);
		}
		else {
			break;
		}
	}
	for (int k = i + 1, l = j - 1; k < size && l >=0; k++, l--)		// Check if player can move to the right-down then shoot.
	{
		if (game.getIndex(k, l) == 0)
		{
			temp = move;
			temp.shootX = k;
			temp.shootY = l;
			moves.push_back(temp);
		}
		else {
			break;
		}
	}


}

double AIPlayer::evaulate(Game& game)		//evulation function
{
	
	double count1 = 0;
	double count2 = 0;
	int size = game.getSize();
	std::vector<int> board(size*size,0);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			board.push_back(game.getIndex(i, j));
		}

	std::map<std::vector<int>, int>::const_iterator got = map.find(board);
	if (got != map.end())
	{
		hashCounter++;
		return got->second;

	}
	int secondSymbol = playerSymbol == 1 ? 2 : 1;
	int c;
	for(int i=0;i<size;i++)
		for (int j = 0; j < size; j++)
		{
			if (game.getIndex(i, j) == playerSymbol)
			{
				c = evaulateIndex(i, j, game);
				count1 += evaulateIndex(i, j, game);
			}
			if (game.getIndex(i, j) == secondSymbol) {
				c = evaulateIndex(i, j, game);
				count2 += evaulateIndex(i, j, game);
			}
		}
	if (count1 == 0 && count2 == 0)
		return 0;
	map.insert({ board,count1/(count1 + count2) });
	
	return (count1/(count1 +count2));


}




int AIPlayer::evaulateIndex(int i,int j,Game& game)		
{
	int count = 0;
	int size = game.getSize();
	for (int k = i + 1; k < size; k++)		// Check if player can move right then shoot.
	{
		if (game.getIndex(k, j) == 0)
		{
			count++;
		}
		else {
			break;
		}
	}

	for (int k = i - 1; k >= 0; k--)		// Check if player can move left then shoot.
	{
		if (game.getIndex(k, j) == 0)
		{
			count++;


		}
		else {
			break;
		}
	}

	for (int k = i - 1, l = j - 1; k >= 0 && l >= 0; k--, l--)		// Check if player can move  left-down then shoot.	
	{
		if (game.getIndex(k, l) == 0)
		{
			count++;

		}
		else {
			break;
		}
	}

	for (int k = i + 1, l = j + 1; k < size && l < size; k++, l++)		// Check if player can move to the right-up then shoot.
	{
		if (game.getIndex(k, l) == 0)
		{
			count++;

		}
		else {
			break;
		}
	}

	for (int k = j + 1; k < size; k++)		// Check if player can move to the up then shoot.
	{
		if (game.getIndex(i, k) == 0)
		{
			count++;

		}
		else {
			break;
		}
	}


	for (int k = j - 1; k >= 0; k--)		// Check if player can move to the down then shoot.
	{
		if (game.getIndex(i, k) == 0)
		{
			count++;

		}
		else {
			break;
		}
	}


	for (int k = i - 1, l = j + 1; k >= 0 && l < size; k--, l++)		// Check if player can move to the left-up then shoot.
	{
		if (game.getIndex(k, l) == 0)
		{
			count++;

		}
		else {
			break;
		}
	}
	for (int k = i + 1, l = j - 1; k < size && l >=0; k++, l--)		// Check if player can move to the right-down then shoot.
	{
		if (game.getIndex(k, l) == 0)
		{
			count++;

		}
		else {
			break;
		}
	}
	return count;


}

