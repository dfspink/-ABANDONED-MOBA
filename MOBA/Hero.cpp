#include "Hero.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
using namespace::std;

Hero::Hero(){}

ostream& operator <<(ostream &out, Hero H)
{
	int i=0;
	
	H.update_stats();
	out << H.name << endl;
	out << "Level: " << H.level << endl;
	out << "Alive: ";
	if (H.alive==1)
		out << "Yes" << endl;
	else
	{
		out << "No" << endl;
		out << "Turns till respawn: " << H.respawn << endl;
	}
	out << "Push: " << H.push_b << " +" << (H.push-H.push_b) << endl;
	out << "Gank: " << H.gank_b << " +" << (H.gank-H.gank_b) << endl;
	out << "Carry: " << H.carry_b << " +" << (H.carry-H.carry_b) << endl;
	out << "Location: ";
	if (H.alive==1)
		out << "(" << H.x << "," << H.y << ")" << endl;
	else
		out << "N/A" << endl;		
	out << "Items: " << endl;
	if (H.items.empty())
		out << "No Items." << endl;
	else
		for (i=0;i<H.items.size();i++)
			out << H.items.at(i);
	out << endl;
	return out;
}
istream& operator >>(istream &in, Hero &H)
{
	int i,j,stat_in_i;
	vector<int> hero_growth_in;
	string name_in,stat_in_s;
	char character_remove;

	do {	// Gets the name of the hero
		in.get(character_remove);
		if (character_remove!=10)	// dont add to string if carriage return
			name_in+=character_remove;
	} while (character_remove!=10);
	H.name=name_in; 

	for(i=0;i<4;i++)	// Gets the stat growth of the hero
	{
		for(j=0;j<3;j++)
		{
			do {	// Gets the name of the hero
				in.get(character_remove);
				if ((character_remove!=32) && (character_remove!=10) && (in.good()))	// dont add to string if space or carriage return
					stat_in_s+=character_remove;
			} while ((character_remove!=32) && (character_remove!=10) && (in.good()));
			stat_in_i=atoi(stat_in_s.c_str());
			hero_growth_in.push_back(stat_in_i);
			stat_in_s.clear();
		}
		H.stat_growth.push_back(hero_growth_in);
		hero_growth_in.clear();
	}
	H.x=0;
	H.y=0;
	H.gold=15000;
	H.level=1;
	H.alive=1;
	H.respawn=0;
	H.exp=0;

	H.push_b=H.stat_growth.at(0).at(0);	// Sets initial base stats of hero
	H.gank_b=H.stat_growth.at(0).at(1);
	H.carry_b=H.stat_growth.at(0).at(2);
	H.push=H.push_b;					// Sets initial total stats of hero
	H.gank=H.gank_b;
	H.carry=H.carry_b;

	return in;
}
bool Hero::operator ==(const Hero &rhs) const
{
	bool answer;
	if (name == rhs.name)
		answer=true;
	else
		answer=false;
	return answer;
}
void Hero::operator +(const Item &rhs)
{
	if(items.size()<6)
	{
		items.push_back(rhs);
		update_stats();
	}
	else
		cout << "Hero can only hold 6 items." << endl;
}
void Hero::operator -(const Item &rhs)
{
	int i,found=0;

	for(i=0;i<items.size();i++)
	{
		if ((items.at(i)==rhs) && (found==0))
		{
			items.erase(items.begin()+i);
			found=1;
		}
	}
	update_stats();
}
string Hero::get_name()
{
	return name;
}
int Hero::get_push()
{
	return push;
}
int Hero::get_gank()
{
	return gank;
}
int Hero::get_carry()
{
	return carry;
}
int Hero::get_num_item()
{
	return items.size();
}
void Hero::add_gold(int gold_in)
{
	gold+=gold_in;
}
void Hero::rem_gold(int gold_in)
{
	gold-=gold_in;
	if (gold<0)
		gold=0;
}
int Hero::get_gold()
{
	return gold;
}
void Hero::add_exp(float exp_in)
{
	exp+=exp_in;
	if ((exp>=600) && (level<=3))
	{
		exp-=600;
		if (exp<0)
			exp=0;
		level_up();
	}
	if ((exp>=800) && (level>3) && (level<=5))
	{
		exp-=1000;
		if (exp<0)
			exp=0;
		level_up();
	}
	else if ((exp>=5) && (level>5) && (level<8))
	{
		exp-=1300;
		if (exp<0)
			exp=0;
		level_up();
	}
	else if ((level==8) && (exp>=5))
		exp=5;
}
float Hero::get_exp()
{
	return exp;
}
void Hero::level_up()
{
	level++;
	if ((level==2) || (level==3))
	{
		push_b+=stat_growth.at(1).at(0);
		gank_b+=stat_growth.at(1).at(1);
		carry_b+=stat_growth.at(1).at(2);
	}
	if ((level==4) || (level==5))
	{
		push_b+=stat_growth.at(2).at(0);
		gank_b+=stat_growth.at(2).at(1);
		carry_b+=stat_growth.at(2).at(2);
	}
	if ((level==6) || (level==7) || (level==8))
	{
		push_b+=stat_growth.at(3).at(0);
		gank_b+=stat_growth.at(3).at(1);
		carry_b+=stat_growth.at(3).at(2);
	}
	update_stats();
}
int Hero::get_level()
{
	return level;
}
int Hero::get_alive()
{
	return alive;
}
void Hero::set_alive(int alive_in)
{
	alive=alive_in;
}
int Hero::get_respawn()
{
	return respawn;
}
void Hero::set_respawn(int respawn_in)
{
	respawn=respawn_in;
}
int Hero::get_x()
{
	return x;
}
void Hero::set_x(int x_in)
{
	x=x_in;
}
int Hero::get_y()
{
	return y;
}
void Hero::set_y(int y_in)
{
	y=y_in;
}
void Hero::set_coord(int x_in,int y_in)
{
	x=x_in;
	y=y_in;
}
void Hero::update_stats()
{
	int i;

	push=push_b;
	gank=gank_b;
	carry=carry_b;

	for (i=0;i<items.size();i++)
	{
		push+=items.at(i).get_push();
		gank+=items.at(i).get_gank();
		carry+=items.at(i).get_carry();
	}
}
void Hero::clear_stat_growth()
{
	vector <vector <int> > empty_vec;
	stat_growth=empty_vec;
}