#include <iostream>
#include "Player.h"
#include <vector>
#ifndef HPlayer_H
#define HPlayer_H



class HPlayer : public Player {

public:
	HPlayer();

	bool play(Game& game);

	Move stringToMove(std::string s);
	






};

#endif
#pragma once
