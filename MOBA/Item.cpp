#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include "Item.h"

using namespace std;

Item::Item()
{
	push = 0;
	gank = 0;
	carry = 0;
}
ostream& operator <<(ostream &out, Item I)
{
	out << I.name << endl;
	out << "Push: " << I.push << endl;
	out << "Gank: " << I.gank << endl;
	out << "Carry: " << I.carry << endl;
	return out;
}
istream& operator >>(istream &in, Item &I)
{
	int h=0,i=0,j=0;
	string name_in,price_s;
	char character_remove,push_c=0,gank_c=0,carry_c=0,price_c=0;

	do {	// Gets the name of the item
		in.get(character_remove);
		if (character_remove!=10)	// dont add to string if carriage return
			name_in+=character_remove;
	} while (character_remove!=10);
	I.name=name_in;

	in.get(push_c);		// Get push
	I.push=atoi(&push_c);
	in.ignore(1);
	in.get(gank_c);		// Get gank
	I.gank=atoi(&gank_c);
	in.ignore(1);
	in.get(carry_c);	// Get carry
	I.carry=atoi(&carry_c);
	in.ignore(1);
	do {	// Gets the price of the item
		in.get(price_c);
		if (price_c!=10)	// dont add to price if carriage return
		{
			price_s+=price_c;
		}
	} while ((price_c!=10) && (in.good()));
	I.price=atoi(price_s.c_str());

	return in;
}
bool Item::operator ==(const Item &rhs) const
{
	bool answer;
	if (name == rhs.name)
		answer=true;
	else
		answer=false;
	return answer;
}
string Item::get_name()
{
	return name;
}
int Item::get_push()
{
	return push;
}
int Item::get_gank()
{
	return gank;
}
int Item::get_carry()
{
	return carry;
}
int Item::get_price()
{
	return price;
}
void Item::clear()
{
	name="NULL";
	push=0;
	gank=0;
	carry=0;
	price=0;
}