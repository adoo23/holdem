#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "query.h"
#include <string>

class Player {
public:

	Player(const Client& client):query(client){}

	~Player() { destroy(); }

	void init();
	void destroy();

	std::string login_name();	// get the login name
	void login_name(std::string name);	// set the login name

	decision_type preflop();
	decision_type flop();
	decision_type turn();
	decision_type river();
	hand_type showdown();
	void game_end();

	int pick(hand_type &hand,int n);
	int pick2(int n);
	int get_delta();
	int get_avai();
	int get_pp();

private:
	Query query;

};


#endif	// _PLAYER_H_
