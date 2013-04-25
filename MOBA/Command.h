#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
using namespace::std;

#include "Hero.h"

class Command
{
private:
	int command;	// 0=gank, 1=push 2=farm, 3=move, 4=nothing
	int x,y;		// Location for command to execute to/on 
	vector<int> heroes;	// [0]=first hero in corresponding team hero vector, [1]=second hero etc.
public:
	Command();
	Command &operator =(const string &rhs);
	void set_heroes(int element);
	void change_heroes(int element,int value);
	vector<int> get_heroes();
	void set_coord(int x_in,int y_in);
	int get_x();
	int get_y();
};