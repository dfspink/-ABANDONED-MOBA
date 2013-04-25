#ifndef HERO_H
#define HERO_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
using namespace::std;

#include "Item.h"

class Hero
{
	friend ostream& operator <<(ostream&, Hero H);		// Output operator
	friend istream& operator >>(istream&, Hero &H);		// Input operator
private:
	string name;
	int push,push_b,gank,gank_b,carry,carry_b;	// _b = base, non_b stat represents inclusion of items/abilities
	int x,y;									// Position of the hero
	int gold;									// Gold the hero currently has
	int level;									// Current level of the hero
	int alive,respawn;									// Flag to tell if hero is alive or dead, 1=alive 0=dead
	float exp;									// EXP the hero currently has
	vector< vector<int> > stat_growth;			// Contents show how the hero's base stats scale with levels (1, 2-3, 4-5, 6-8)			
	
public:
	Hero();
	bool operator ==(const Hero &rhs) const;
	void operator +(const Item &rhs);
	void operator -(const Item &rhs);
	string get_name();
	int get_push();
	int get_gank();
	int get_carry();
	int get_num_item();
	void add_gold(int gold_in);
	void rem_gold(int gold_in);
	int get_gold();
	void add_exp(float exp_in);
	float get_exp();
	void level_up();
	int get_level();
	int get_alive();
	void set_alive(int alive_in);
	int get_respawn();
	void set_respawn(int respawn_in);
	int get_x();
	void set_x(int x_in);
	int get_y();
	void set_y(int y_in);
	void set_coord(int x_in,int y_in);
	void update_stats();
	void clear_stat_growth();
	vector<Item> items;							// Contains items the hero is in possession of
};
#endif