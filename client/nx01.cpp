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

bool check_flush(card_list &cards,hand_type &hand,int n){
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

bool check_straight(card_list &cards,hand_type &hand,int n){
	sort_cards_rank(cards,n);
	/*int cnt=1;
	hand[0]=cards[0];
	for(int i=0;i<n;){
		int j=i+1;
		for(;(j<n)&&(convert_rank(cards[j])==convert_rank(cards[i]));++j);
		if(j>=n) break;
		if(convert_rank(cards[j])+1==convert_rank(cards[i])){
			hand[cnt]=cards[j];
			++cnt;
			if(cnt==5) return true;
		}else{
			cnt=1;
			hand[0]=cards[j];
		}
		i=j+1;
	}*/
	for(int i=0;i<n;++i){
		int cnt=0;
		hand[0]=cards[i];
		int j=i+1;
		for(;(j<n);++j){
			if(convert_rank(cards[j])+1==convert_rank(hand[cnt])){
				hand[++cnt]=cards[j];
				if(cnt==4){
					return true;
				}
			}
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
	/*sort_cards_rank(cards,n);
	int cnt=1;
	hand[0]=cards[0];
	for(int i=0;i<n;++i){
		int j=i+1;
		for(;(j<n)&&(convert_rank(cards[j])==convert_rank(cards[i]));++j);
		for(;(j<n)&&(convert_rank(cards[j])+1==convert_rank(cards[i]))&&
				(cards[j].second!=cards[i].second);++j);
		if((convert_rank(cards[j])+1==convert_rank(cards[i]))){
			hand[cnt]=cards[j];
			++cnt;
			if(cnt==5) return true;
		}else{
			cnt=1;
			hand[0]=cards[j];
		}
	}*/
	for(int i=0;i<n;++i){
		int cnt=0;
		hand[0]=cards[i];
		int j=i+1;
		for(;(j<n);++j){
			if((convert_rank(cards[j])+1==convert_rank(hand[cnt]))&&(cards[j].second==hand[cnt].second)){
				hand[++cnt]=cards[j];
				if(cnt==4){
					return true;
				}
			}
		}
	}
	return false;
}

bool check_four(card_list &cards,hand_type &hand,int n){
	sort_cards_rank(cards,n);
	for(int i=0;i<n-4;++i){
		/*int j=i+1;
		int flag=1;
		for(;(j<n);++j){
			if(convert_rank(cards[j])==convert_rank(cards[i])){
				++flag;
				if(flag==4)	break;
			}else{
				break;
			}
		}*/
		bool flag=true;
		flag=flag && (convert_rank(cards[i])==convert_rank(cards[i+1]));
		flag=flag && (convert_rank(cards[i])==convert_rank(cards[i+2]));
		flag=flag && (convert_rank(cards[i])==convert_rank(cards[i+3]));
		if(flag){
			for(int k=0;k<4;++k){
				hand[k]=cards[k+i];
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
	for(int i=0;i<n-3;++i){
		/*int j=i+1;
		bool flag=1;
		for(;(j<n);++j){
			if(convert_rank(cards[j])==convert_rank(cards[i])){
				++flag;
				if(flag==3) break;
			}else{
				break;
			}
		}*/
		//if(j>=n) break;
		bool flag=true;
		flag=flag && (convert_rank(cards[i])==convert_rank(cards[i+1]));
		flag=flag && (convert_rank(cards[i])==convert_rank(cards[i+2]));
		if(flag){
			for(int k=0;k<3;++k){
				hand[k]=cards[k+i];
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
		sort_cards_rank(all_cards,n);
		for(int i=0;i<5;++i) hand[i]=all_cards[i];
		flag=0;
	}
	return flag;
}
int Player::pick2(int n){
	hand_type hand;
	card_list all_cards;
	all_cards.resize(7);
	//all_cards[0]=query.hole_cards()[0];
	//all_cards[1]=query.hole_cards()[1];
	all_cards[0].first='0';
	all_cards[0].second='?';
	all_cards[1].first='0';
	all_cards[1].second='?';
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
		sort_cards_rank(all_cards,n);
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

int Player::get_avai(){
	int k=query.my_id();
	return query.chips()[k];
}

int Player::get_pp(){
	const std::vector<PLAYER_STATUS>	ps=query.player_status();
	int n=ps.size();
	int cnt=0;
	for(int i=0;i<n;++i)
		if(ps[i]!=FOLDED) ++cnt;
	return cnt;
}

decision_type Player::preflop() {
	int t=query.current_bets(query.my_id());
	int bl=query.blind();
	if(t>bl) return make_decision(CALL);
	int a=convert_rank(query.hole_cards()[0]);
	int b=convert_rank(query.hole_cards()[1]);
	int d=get_delta();
	bool flag;
	if(a==b){
		if(d>1000) flag=false;
		flag=true;
	}else if(d>bl){
		if(d>bl*3) flag=false;
		else if(a+b>=20){
			flag=true;
		}else flag=false;
	}else{
		if(a+b>=17){
			flag=true;
		}else flag=false;
	}
	if(flag) return make_decision(CALL);
	else return make_decision(CHECK);
}

decision_type Player::flop() {
	int bl=query.blind();
	int d=get_delta();
	//if(d==0) return make_decision(CALL);
	hand_type hand;
	int p=pick(hand,3);
	int p2=pick2(3);
	std::cout<<p<<std::endl;
	if((p>=4)&&(p2<p)) return make_decision(RAISE,4*bl);
	//if((p>=1)&&(get_pp()<=3)) return make_decision(CALL);
	if(d>=300){
		if(p>=3){
			return make_decision(CALL);	
		}else{
			return make_decision(CHECK);
		}
	}else{
		if(p>=2){
			return make_decision(CALL);
		}else{
			return make_decision(CHECK);
		}
	}
}

decision_type Player::turn() {
	int bl=query.blind();
	int d=get_delta();
	//if(d==0) return make_decision(CALL);
	hand_type hand;
	int p=pick(hand,4);
	int p2=pick2(4);
	std::cout<<p<<std::endl;
	if((p>=4)&&(p2<p)) return make_decision(RAISE,4*bl);
	//if((p>=1)&&(get_pp()<=3)) return make_decision(CALL);
	if(d>=300){
		if(p>=4){
			return make_decision(CALL);	
		}else{
			return make_decision(CHECK);	
		}
	}else{
		if(p>=2){
			return make_decision(CALL);
		}else{
			return make_decision(CHECK);
		}
	}
}

decision_type Player::river() {
	int d=get_delta();
	//if(d==0) return make_decision(CALL);
	hand_type hand;
	int p=pick(hand,5);
	int p2=pick2(5);
	std::cout<<p<<std::endl;
	if((p>=4)&&(p2<p)) return make_decision(RAISE,10000);
	//if((p>=1)&&(get_pp()<=3)) return make_decision(CALL);
	if(d>=300){
		if(p>=4){
			return make_decision(CALL);	
		}else{
			return make_decision(FOLD);	
		}
	}else{
		if(p>=2){
			return make_decision(CALL);
		}else{
			return make_decision(CHECK);
		}
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
	int p=pick(hand,5);
	std::cout<<p<<std::endl;
	return std::move(hand);
}

void Player::game_end() {}

