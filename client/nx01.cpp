#include "client.h"
#include "player.h"
#include "common.h"
#include <iostream>
#include <utility>

typedef std::vector<card_type> card_list;

const char suit[4]={'S','C','H','D'};

inline int convert_rank(card_type c){
	switch(c.first){
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'T':
			return 10;
		case 'J':
			return 11;
		case 'Q':
			return 12;
		case 'K':
			return 13;
		case 'A':
			return 14;
		default:
			return 0;
	}
}
inline bool cmp_rank(card_type &a,card_type &b){
	return convert_rank(a)>convert_rank(b);
}

void sort_cards_rank(card_list &cards,int n){
	for(int i=0;i<n-1;++i){
		for(int j=i+1;j<n;++j){
			if(!cmp_rank(cards[i],cards[j])) std::swap(cards[i],cards[j]);
		}
	}
}

void sort_cards_suit(card_list &cards,int n){
	for(int i=0;i<n-1;++i){
		for(int j=i+1;j<n;++j){
			if(cards[i].second>cards[j].second) std::swap(cards[i],cards[j]);
		}
	}
}

bool check_straight(card_list &cards,hand_type &hand,int n){
	//sort_cards_suit(cards,n);
	char c;
	for(int i=0;i<4;++i){
		c=suit[i];
		int cnt=0;
		for(int j=0;j<n;++j){
			if(cards[j].second==c) ++cnt;
		}
		if(cnt>=5){
			sort_cards_rank(cards,n);
			int k=0;
			for(int j=0;j<n;++j){
				if(cards[j].second==c){
					hand[k++]=cards[j];
					if(k==5) break;
				}
			}
			return true;
		}
	}
	return false;
}

bool check_flush(card_list &cards,hand_type &hand,int n){
	sort_cards_rank(cards,n);
	int cnt=1;
	hand[0]=cards[0];
	for(int i=0;i<n;++i){
		int j=i+1;
		for(;(j<n)&&(convert_rank(cards[j])==convert_rank(cards[i]));++j);
		if(convert_rank(cards[j])+1==convert_rank(cards[i])){
			hand[cnt]=cards[j];
			++cnt;
			if(cnt==5) return true;
		}else{
			cnt=1;
			hand[0]=cards[j];
		}
	}
	return false;
}

bool check_straight_flush(card_list &cards,hand_type &hand,int n){
	// logical failure here, to be fixed
	/*hand_type h;
	if(check_straight(cards,h,n)){
		if(check_flush(h,hand,5)){
			return true;
		}
	}*/
	return false;
}

bool check_four(card_list &cards,hand_type &hand,int n){
	sort_cards_rank(cards,n);
	for(int i=0;i<n;++i){
		int j=i+1;
		for(;(j<n)&&(convert_rank(cards[j])==convert_rank(cards[i]));++j);
		if(j-i==4){
			for(int k=i;k<j;++k){
				hand[k-i]=cards[k];
			}
			int t=0;
			for(;convert_rank(cards[i])==convert_rank(cards[t]);++t);
			hand[4]=cards[t];
			return true;
		}
	}
	return false;
}

bool check_three(card_list &cards,hand_type &hand,int n){
	sort_cards_rank(cards,n);
	for(int i=0;i<n;++i){
		int j=i+1;
		for(;(j<n)&&(convert_rank(cards[j])==convert_rank(cards[i]));++j);
		if(j-i==3){
			for(int k=i;k<j;++k){
				hand[k-i]=cards[k];
			}
			int t=0;
			for(;convert_rank(cards[i])==convert_rank(cards[t]);++t);
			hand[3]=cards[t];
			for(++t;convert_rank(cards[i])==convert_rank(cards[t]);++t);
			hand[4]=cards[t];
			return true;
		}
	}
	return false;
}

bool check_full_house(card_list &cards,hand_type &hand,int n){
	//hand_type h;
	if(check_three(cards,hand,n)){
		for(int i=0;i<n-1;++i){
			if(convert_rank(cards[i])!=convert_rank(hand[0])){
				if(convert_rank(cards[i])==convert_rank(cards[i+1])){
					//hand[0]=h[0];hand[1]=h[1];hand[2]=h[2];
					hand[3]=cards[i];
					hand[4]=cards[i+1];
					return true;
				}
			}
		}
	}
	return false;
}

bool check_pair(card_list &cards,hand_type &hand,int n){
	sort_cards_rank(cards,n);
	for(int i=0;i<n-1;++i){
		if(convert_rank(cards[i])==convert_rank(cards[i+1])){
			hand[0]=cards[i];
			hand[1]=cards[i+1];
			int k=2;
			for(int j=0;j<n;++j){
				if(convert_rank(cards[j])!=convert_rank(hand[0])){
					hand[k++]=cards[j];
					if(k==5) return true;
				}
			}
		}
	}
	return false;
}

bool check_two_pairs(card_list &cards,hand_type &hand,int n){
	if(check_pair(cards,hand,n)){
		for(int i=0;i<n-1;++i){
			if(convert_rank(cards[i])!=convert_rank(hand[0])){
				if(convert_rank(cards[i])==convert_rank(cards[i+1])){
					//hand[0]=h[0];hand[1]=h[1];hand[2]=h[2];
					hand[2]=cards[i];
					hand[3]=cards[i+1];
					for(int j=0;j<n;++j){
						if(convert_rank(cards[j])!=convert_rank(hand[2])){
							if(convert_rank(cards[j])!=convert_rank(hand[0])){
								hand[4]=cards[j];
							}
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

int Player::pick(hand_type &hand,int n){
	card_list all_cards;
	all_cards.resize(7);
	all_cards[0]=query.hole_cards()[0];
	all_cards[1]=query.hole_cards()[1];
	const auto& community_cards = query.community_cards();
	for(int i=0;i<n;++i) all_cards[i+2]=community_cards[i];
	n+=2;
	/*bool flag=check_straight_flush(all_cards,hand,n) || check_four(all_cards,hand,n) ||
		check_full_house(all_cards,hand,n) || check_flush(all_cards,hand,7) ||
		check_straight(all_cards,hand,7) || check_three(all_cards,hand,7) ||
		check_two_pairs(all_cards,hand,7) || check_pair(all_cards,hand,7);*/
	int flag=0;
	if(check_straight_flush(all_cards,hand,n)){
		flag=8;
	}else if(check_four(all_cards,hand,n)){
		flag=7;
	}else if(check_full_house(all_cards,hand,n)){
		flag=6;
	}else if(check_flush(all_cards,hand,n)){
		flag=5;
	}else if(check_straight(all_cards,hand,n)){
		flag=4;
	}else if(check_three(all_cards,hand,n)){
		flag=3;
	}else if(check_two_pairs(all_cards,hand,n)){
		flag=2;
	}else if(check_pair(all_cards,hand,n)){
		flag=1;
	}
	if(!flag){
		sort_cards_rank(all_cards,7);
		for(int i=0;i<5;++i) hand[i]=all_cards[i];
		flag=0;
	}
	return flag;
}

void Player::init() {}
void Player::destroy() {}

std::string Player::login_name() {
	//std::string login_name;
	//std::cout << "[UI] login name: ";
	//std::getline(std::cin, login_name);

	return std::move("NX01");
}

void Player::login_name(std::string name) {
	std::cout << "[UI] succefully login as " << name << std::endl;
}

#define GET_BET_FROM_USER	\
	while (true) {	\
		std::string action;	\
		int amt;	\
		std::cin >> action;	\
		if (action.size() < 4) {	\
			std::cout << "[UI] invalid action, " << action << std::endl;	\
			continue;	\
		}	\
		switch (action[3]) {	\
		case 's': /*raise*/ \
			std::cin >> amt;	\
			return make_decision(RAISE, amt);	\
		case 'c': /*check*/ \
			return make_decision(CHECK);	\
		case 'l': /*call*/	\
			return make_decision(CALL);	\
		case 'd': /*fold*/	\
			return make_decision(FOLD);	\
		default:	\
			std::cout << "[UI] invalid action, " << action << std::endl;	\
		}	\
	}

#define PRINT_CARD(card) \
	std::cout << "[UI] " << rank_of(card.first) << ' ' << suit_of(card.second) << std::endl

int Player::get_delta(){
	int n=query.number_of_players();
	int k=query.my_id();
	int max=0;
	for(int i=0;i<n;++i){
		if(i==k) continue;
		int tmp=query.current_bets(i);
		if(tmp>max) max=tmp;
	}
	return max-query.current_bets(k);
}

decision_type Player::preflop() {
	int a=convert_rank(query.hole_cards()[0]);
	int b=convert_rank(query.hole_cards()[1]);
	int d=get_delta();
	bool flag;
	if(a==b){
		flag=true;
	}else if(d>=200){
		if(a+b>=20){
			flag=true;
		}else flag=false;
	}else{
		if(a+b>=17){
			flag=true;
		}else flag=false;
	}
	if(flag) return make_decision(CALL);
	else return make_decision(FOLD);
}

decision_type Player::flop() {
	int d=get_delta();
	if(d==0) return make_decision(CALL);
	hand_type hand;
	if(pick(hand,3)){
		return make_decision(CALL);	
	}else{
		return make_decision(FOLD);	
	}
}

decision_type Player::turn() {
	int d=get_delta();
	if(d==0) return make_decision(CALL);
	hand_type hand;
	if(pick(hand,4)>1){
		return make_decision(CALL);	
	}else{
		return make_decision(FOLD);	
	}
}

decision_type Player::river() {
	int d=get_delta();
	if(d==0) return make_decision(CALL);
	hand_type hand;
	if(pick(hand,5)>1){
		return make_decision(CALL);	
	}else{
		return make_decision(FOLD);	
	}
}

#undef GET_BET_FROM_USER


hand_type Player::showdown() {
	std::cout << "[UI] hole cards" << std::endl;
	for (int i = 0; i < 2; ++i) {
		PRINT_CARD(query.hole_cards()[i]);
	}

	std::cout << "[UI] community cards" << std::endl;
	const auto& community_cards = query.community_cards();
	for (int i = 0; i < 5; ++i) {
		PRINT_CARD(community_cards[i]);
	}	

	//std::cout << "[UI] Choose card: (hole card: 0 - 1, community cards: 2 - 6)" << std::endl;
	hand_type hand;
	/*for_each(hand.begin(), hand.end(), 
			[&, this](card_type& card) {
				int id;
				std::cin >> id;
				if (id <= 1) card = query.hole_cards()[id];
					else card = community_cards[id - 2];
			});
			*/
	pick(hand,5);
	return std::move(hand);
}

void Player::game_end() {}

