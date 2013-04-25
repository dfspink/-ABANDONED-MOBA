#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Item
{
	friend istream& operator >>(istream &in, Item &I);
	friend ostream& operator <<(ostream&, Item I);
private:
	string name;
	int push,gank,carry,price;
public:
	Item();
	bool operator ==(const Item &rhs) const;
	string get_name();
	int get_push();
	int get_gank();
	int get_carry();
	int get_price();
	void clear();
};
#endif