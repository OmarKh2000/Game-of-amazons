#include <iostream>
#include "Game.h"
#include "Move.h"
//#include <boost/functional/hash.hpp>
#ifndef Player_H
#define Player_H
const int branchFactor = 150;

/*template <typename Container> // we can make this generic for any container [1]
struct container_hash {
	std::size_t operator()(Container const& c) const {
		return boost::hash_range(c.begin(), c.end());
	}
};*/

class Player {
public:
	int playerSymbol;
	int time;
	Player(int time,int playerSymbol);
	virtual bool play(Game& game) = 0;



};

#endif
