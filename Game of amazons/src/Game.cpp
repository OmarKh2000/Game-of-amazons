#include <iostream>
#include "Game.h"


Game::Game(int size,int time)
{	
	this->time = time;
	
	board = new int*[size];
	for (int i = 0; i < size; ++i) {
		board[i] = new int[size];
	}
	p1Time = time*1000000*60;
	p2Time = time*1000000*60;
	this->size = size;
	if (size == 10)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				board[i][j] = 0;
			}

		board[0][3] = 1;
		board[0][6] = 1;
		board[3][0] = 1;
		board[3][9] = 1;
		board[9][3] = 2;
		board[9][6] = 2;
		board[6][0] = 2;
		board[6][9] = 2;



	}
	else if(size==6) {
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				board[i][j] = 0;
			}

		board[2][1] = 1;
		board[3][6] = 1;
		board[0][4] = 2;
		board[0][6] = 2;

	}

}



int Game::getTurn() const
{
	return currentTurn;
}

int Game::getSize() const
{
	return size;
}

int Game::getround()
{
	return round;
}


int Game::getIndex(int i, int j) const
{
	return board[i][j];
}

int Game::checkEnd()		//chech if someone won.
{
	bool p1Lost = false;
	bool p2Lost = false;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{

			if (!p1Lost && board[i][j] ==1)
				p1Lost = (p1Lost || checkCanMove(i, j));
			if (!p2Lost && board[i][j] == 2)
				p2Lost = (p2Lost || checkCanMove(i, j));


		}
	if (!p1Lost && !p2Lost)
	{
		std::cout << "Player" << ((getTurn() + 1) %2 ) << "has won" <<std::endl;
		return true;
	}
	if (!p1Lost)
	{
		std::cout << "Player 2 has won" << std::endl;
		return true;
	}
	if (!p2Lost)
	{
		std::cout << "Player 1 has won" << std::endl;

		return true;
	}
	return false;

}

Game::~Game()
{
	for (int i = 0; i < size; i++)
		delete[] board[i];
	delete board;
}
int Game::getTime() const
{
	return time;

}
Game::Game(const Game & game)
{
	this->time = game.getTime();
	int size = game.getSize();
	if (board == NULL) {
		board = new int*[size];
		for (int i = 0; i < size; ++i) {
			board[i] = new int[size];
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j< size; j++)
		{
			board[i][j] = game.getIndex(i, j);
		}
	}
	this->currentTurn = game.getTurn();
	this->size = game.getSize();
	this->round = round;
}

void Game::applyMove(Move& move, int symbol)
{
	this->round++;
	if (move.fromX >= 0 && move.fromX < size && move.fromY>=0 && move.fromY < size && move.toX>=0 && move.toY < size && move.shootX>=0 && move.shootY < size)	// check if indices are in range.
	{
		board[move.fromX][move.fromY] = 0;
		board[move.toX][move.toX] = symbol;
		board[move.shootX][move.shootY] = symbol + 2;
	}
	
}

void Game::applyandPrintMove(Move& move, int symbol)
{
	round++;
	if (move.fromX >= 0 && move.fromX < size && move.fromY>=0 && move.fromY < size && move.toX>=0 && move.toY < size && move.shootX>=0 && move.shootY < size)	// check if indices are in range.
	{
		board[move.fromX][move.fromY] = 0;
		board[move.toX][move.toY] = symbol;
		board[move.shootX][move.shootY] = symbol + 2;
	}
	



	printMove(move);

}


int Game::updatePlayerTime(int index, int time)		//update times
{

	if (index == 1)
	{
		p1Time = p1Time - time;
		return p1Time;
	}
	if (index == 2)
	{
		p2Time = p2Time - time;
		return p2Time;
	}

	return 0;

}
void Game::applytemp(Move& move,int symbol)	//apply move
{
	board[move.fromX][move.fromY] = 0;
	board[move.toX][ move.toY] = symbol;


}

void Game::unapplytemp(Move& move, int symbol)	//unapplymove
{
	board[move.fromX][move.fromY] = symbol;
	board[move.toX][move.toY] = 0;


}


bool Game::checkCanMove(int i, int j)		//check if players can move
{
	int canMove = false;
	if (i < 0 || i >= size || j < 0 || j >= size)
		return false;
	for (int k = i+1; k < size; k++)		// Check if player can move right then shoot.
	{
		if (board[k][j] == 0 && checkCanShoot(k, j))
			return true;
		else
			break;
	}

	for (int k = i-1; k > 0; k--)		// Check if player can move left then shoot.
	{
		if (board[k][j] == 0 && checkCanShoot(k, j))
			return true;
		else
			break;
	}

	for (int k = i-1, l = j-1; k > 0 && l > 0; k--, l--)		// Check if player can move  left-down then shoot.	
	{
		if (board[k][l] == 0 && checkCanShoot(k, l))
			return true;
		else
			break;
	}

	for (int k = i+1, l = j+1; k < size && l < size; k++, l++)		// Check if player can move to the right-up then shoot.
	{
		if (board[k][l] == 0 && checkCanShoot(k, l))
			return true;
		else
			break;
	}

	for (int k = j+1; k < size; k++)		// Check if player can move to the up then shoot.
	{
		if (board[i][k] == 0 && checkCanShoot(i, k))
			return true;
		else
			break;
	}


	for (int k = j-1; k > 0; k--)		// Check if player can move to the down then shoot.
	{
		if (board[i][k] == 0 && checkCanShoot(i, k))
			return true;
		else
			break;
	}


	for (int k = i-1, l = j+1; k > 0 && l < size; k--, l++)		// Check if player can move to the left-up then shoot.
	{
		if (board[k][l] == 0 && checkCanShoot(k, l))
			return true;
		else
			break;
	}
	for (int k = i+1, l = j-1; k < size && l >0; k++, l--)		// Check if player can move to the right-down then shoot.
	{
		if (board[k][l] == 0 && checkCanShoot(k, l))
			return true;
		else
			break;
	}

	return false;

}

bool Game::checkCanShoot(int i, int j)
{

	if (i < 0 || i >= size || j < 0 || j >= size)
		return false;
	for (int k = i; k < size; k++)		// Check if player shoot to the right.
	{
		if (board[k][j])
			return true;
		else
			break;
	}

	for (int k = i; k > 0; k--)		// Check if player can shoot to the left.
	{
		if (board[k][j])
			return true;
		else
			break;
	}

	for (int k = i, l = j; k > 0 && l > 0; k--, l--)		// Check if player can shoot to left-down.	
	{
		if (board[k][l] == 0)
			return true;
		else
			break;
	}

	for (int k = i, l = j; k < size && l < size; k++, l++)		// Check if player can shoot right-up.
	{
		if (board[k][l] == 0)
			return true;
		else
			break;
	}

	for (int k = j; k < size; k++)		// Check if player can shoot up.
	{
		if (board[i][k] == 0)
			return true;
		else
			break;
	}


	for (int k = j; k > 0; k--)		// Check if player can move to the down then shoot.
	{
		if (board[i][k] == 0)
			return true;
		else
			break;
	}


	for (int k = i, l = j; k > 0 && l < size; k--, l++)		// Check if player can move to the left-up then shoot.
	{
		if (board[k][l] == 0)
			return true;
		else
			break;
	}
	for (int k = i, l = j; k < size && l >0; k++, l--)		// Check if player can move to the right-down then shoot.
	{
		if (board[k][l] == 0)
			return true;
		else
			break;
	}

	return false;

}



void Game::printMove(Move& move)
{

	//std::cout << char(move.fromX+65) << move.fromY << "-" << char(move.toX+65) << move.toY << "-" << char(move.shootX+65) << move.shootY << std::endl;
	std::cout << char(move.fromX+65 ) << move.fromY+1 << "-" << char(move.toX +65) << move.toY+1 << "/" << char(move.shootX+65) << move.shootY+1 <<"/"<<move.PV<<"/"<<move.time<< std::endl;
	std::cout << "Current depth= " << move.depth<<", PV evaluation= "<<move.evaluation <<", number of prunings= "<<move.cuts<<", number of hashs= "<<move.hashCounter<<std::endl<<std::endl;
	/*for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
		{
			std::cout << board[i][j]<< " ";

		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/

}