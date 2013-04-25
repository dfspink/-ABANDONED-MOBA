#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
using namespace::std;

#include "Command.h"

Command::Command()
{
	command=4;
	x=0;
	y=0;
	heroes.resize(3);
	heroes.at(0)=0;
	heroes.at(1)=0;
	heroes.at(2)=0;
}
Command &Command::operator =(const string &rhs)
{
	if (rhs=="gank")
		command=0;
	else if (rhs=="push")
		command=1;
	else if (rhs=="farm")
		command=2;
	else if (rhs=="move")
		command=3;
	else if (rhs=="nothing")
		command=4;
	return *this;
}
void Command::set_heroes(int element)
{
	heroes[element]=1;
}
void Command::change_heroes(int element,int value)
{
	heroes.at(element)=value;
}
vector<int> Command::get_heroes()
{
	return heroes;
}
void Command::set_coord(int x_in,int y_in)
{
	x=x_in;
	y=y_in;
}
int Command::get_x()
{
	return x;
}
int Command::get_y()
{
	return y;
}