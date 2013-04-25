#include <iostream>
using std::ostream;
using std::istream;
using std::endl;

class Location{
	friend ostream& operator <<(ostream&, Location L);		// Output operator--must be declared as friend
	friend istream& operator >>(istream&, Location &L);	// Input operator--must be declared as friend

private:
	int x; //x coordinate
	int y; //y coodinate
	int HP;
	int type[3]; // [1] true if hero [2] true if base [3] true if tower
	int damage;		//tracks dmamage
	int move[8]; // [N S E W NW SW NE SE] true if can move in that direction, false if cannot
	
public:
	Location();
	void take_dam(int amount);
	int get_hp();					// gets hp
	void set_hp(int amount); //sets the hp to ammount
	void set_move(int l, int n);	//sets movement l to n
	int get_move(int l);	
};