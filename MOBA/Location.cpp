#include "Location.h"

Location::Location()
{
	int i;
	HP=0;
	for	(i=0; i<3; i++)
		type[i]=false;
	damage = 0;
	for(i=0; i<8; i++)
		move[i]=false;
}

ostream& operator <<(ostream &out, Location L)
{
	out<<"Location :"<<'('<<L.x<<','<<L.y<<')'<<endl;
	if(L.type[0]>0)
		out<<"Hero ";
	else
		out<<"Enemy ";
	if(L.type[1]>0)
	{
		out<<"Base "<<endl;
		out<<"HP: "<<L.HP;
	}
	else if(L.type[2]>0)
	{
		out<<"Tower "<<endl;
		out<<"HP: "<<L.HP;
	}
	else
		out<<"Empty location";

	return out;
}
istream& operator >>(istream &in, Location &L)
{
	int i;
	in>>L.x; 
	in>>L.y;
	in.ignore(1);
	in>>L.HP;
	for(int i=0; i<3; i++)
		in>>L.type[i]; 	
	for(i=0; i<8; i++)
		in>>L.move[i]; 
	return in;
}

void Location::take_dam(int amount)
{
	HP=HP-amount;
	if (HP<0)
		HP=0;
}

int Location::get_hp()
{
	return HP;
}

void Location::set_hp(int amount)
{
	HP=amount;
}

void Location::set_move(int l, int n)
{
	move[l]=n;
}

int Location::get_move(int l)
{
	return move[l];
}