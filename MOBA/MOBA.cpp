/* 
Doug Spink
MOBA
Version 0.1.3
*/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
#include <ctime>
#define NOMINMAX 
#include <Windows.h>
using namespace::std;


#include "Command.h"
#include "Location.h"
#include "Hero.h"
#include "Item.h"
//#include "Ability.h"


/* ******  map diagram  ******

T1=base, T2=tier2 tower, T3=tier1 tower, T4=mid tower, E=empty

Base = 150HP | tier1 tower = 42HP | tier2 tower = 60HP | mid tower = 102HP

E - T3-T2- T1
| \       / |
T3 E  - T4	T2
|  |  X  |  |
T2 T4 -  E	T3
| /       \ |
T1 -T2-T3 - E

*************************   */


// Prototypes
void legion_input();					// Asks for legion commands
void hellb_input();						// Asks for hellb commands
void exec_commands();					// Executes all commands in command vectors
void load_towers();						// Loads towers with HP into "space" matrix (4x4)
void load_heroes();						// Loads heroes into "hero_list" vector
void load_store();						// Loads items into "store_db" vector
void intro();							// Displays introduction
void hero_selection();					// Picking mode chosen, then heroes selected according
void enter_store(int team,int element);	// Player is presented with store functions
void whois();							// Displays information about selected hero
void tower_check();						// Displays status of tower at specified location
bool add_gank(int team,int element);	// Adds hero into teams corresponding gank vector (true if successfully added hero to gank vector (either by adding hero to a new element or added hero to preexisting element))
bool add_push(int team,int element);	// Adds hero into teams corresponding push vector (true if successfully added hero to gank vector (either by adding hero to a new element or added hero to preexisting element))
bool add_farm(int team,int element);	// Adds hero into teams corresponding farm vector (true if successfully added hero to gank vector (either by adding hero to a new element or added hero to preexisting element))
bool add_move(int team,int element);	// Adds hero into teams corresponding move vector (true if successfully added hero to gank vector (either by adding hero to a new element or added hero to preexisting element))
bool add_nothing(int team,int element);	// Adds hero into teams corresponding nothing vector (true if successfully added hero to gank vector (either by adding hero to a new element or added hero to preexisting element))
void death_turn(int team,int element);	// Turn while dead (same as nothing)
bool gank_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error);	// Checks to see if ganking is possible given location and destination (true if gank is possible)
bool push_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error);	// Checks to see if pushing is possible given location and destination (true if push is possible)
bool farm_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error);	// Checks to see if farming is possible given location and destination (true if farm is possible)
bool move_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error);	// Checks to see if moving is possible given location and destination (true if move is possible)
void gank_fn();
void push_fn();
void farm_fn();
void move_fn();
void nothing_fn();
bool base_vulnerable(int team);			// Checks if entered team's base can be damaged
bool valid_path(int x_origin,int y_origin,int x_dest,int y_dest);		// Checks to see if moving from current location to destination is possible (true if tehre is a path from origin to destination)
bool enemy_hero_present(int x_origin,int y_origin,int x_in,int y_in);	// Checks what team is at origin coord, then checks if other team is at destination (true if there is an enemy hero there)
bool tower_present(int x_in,int y_in);	// Checks if a tower with HP>0 is at the coordinates (true if there is a tower with HP>0)
bool enemy_tower_present(int x_origin,int y_origin,int x_in,int y_in);	// Checks what team is at origin coord, then checks if other team has a tower at the destination (true if there is an enemy tower there)
bool chance_50_gen();					// Generates true 50% of the time
bool chance_25_gen();					// Generates true 25% of the time	
bool outcome_gen(int a,int b);			// Generates win/lose for a vs b. Result is from a's point of view
int pick_element(int elements);			// Picks an element
void wait(int seconds);					// Waits inputed amount of seconds
void pick_countdown();					// Displays countdown before pick sequence (5->1)
void game_countdown();					// Displays countdown before game starts (5->1-> Lets get it on.)
void winner_legion();					// Displays legion wins
void winner_hellb();					// Displays hellbourne wins
void print_map();						// Prints the map
void hero_death(int team,int element);	// Handles hero deaths
void disp_commands(int team,int element);	// Displays commands that are available
void disp_teams();						// Displays team compositions
void clear_commands();					// Clears all command vectors
void respawn_handler();					// Handles hero respawning and respawn timers
void wait_for_enter(int skipnew);		// Waits for enter key from user

// Globals
Location space[4][4];
vector<Hero> hero_list,legion,hellb;
vector<Command> legion_gank,legion_push,legion_farm,legion_move,legion_nothing,hellb_gank,hellb_push,hellb_farm,hellb_move,hellb_nothing;
vector<Item> store_db;
int turn=1;

int main()
{
	int i=0;
	bool start_game=true, valid_input=true, reselect_hero=true;
	string input="";

	srand(time(0));

	load_store();			
	load_towers();			
	load_heroes();			

	//intro();
	//pick_countdown();

	do {
		if (reselect_hero)
			hero_selection();
		system("cls");
		if (!valid_input)
			cout << "Invalid input." << endl << endl;
		disp_teams();
		cout << "Start game? (y/n) : ";
		valid_input=true;
		if (!(cin >> input))
			valid_input=false;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (input=="y")
			start_game=true;
		else if (input=="n")
		{
			legion.clear();
			hellb.clear();
			cin.clear();
			system("cls");
			start_game=false;
			reselect_hero=true;
		}
		else
		{
			start_game=false;
			valid_input=false;
			reselect_hero=false;
		}
	} while (!start_game);
	cin.clear();
	//game_countdown();		

	system("cls");
	
	while((space[0][0].get_hp()>0) && (space[3][3].get_hp()>0))
	{
		system("cls");
		print_map();
		cout << endl << "Turn Number: " << turn << ". " << "Legion Command Phase." << endl << endl;
		legion_input();
		system("cls");
		print_map();
		cout << endl << "Turn Number: " << turn << ". " << "Hellbourne Command Phase." << endl << endl;
		hellb_input();
		system("cls");
		exec_commands();
		turn++;
		respawn_handler();
	}
	
	if (space[3][3].get_hp()==0)
	{
		system("cls");
		winner_legion();	
	}
	else if (space[0][0].get_hp()==0)
	{
		system("cls");
		winner_hellb();	
	}

	return 0;
}

void legion_input()
{
	int i=0,option=1;
	int valid=1,command_i=1,command_value=1;
	bool command_b;

	for(i=0;i<3;i++)
	{
		cout << endl;
		command_value=1;
		do {
			valid=1;
			command_value=1;
			command_b=false;
			disp_commands(0,i);
			if (legion.at(i).get_alive()==1)
			{
				cout << "Enter command : ";
				while ( ! (cin >> command_i))
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					system("cls");
					cout << "Invalid input." << endl <<endl;
					disp_commands(0,i);
					cout << "Enter command : ";
				}
				if (((legion.at(i).get_x()==0) && (legion.at(i).get_y()==0)) || ((legion.at(i).get_x()==3) && (legion.at(i).get_y()==3)))
					command_value++;
				if (((((legion.at(i).get_x()==0) && (legion.at(i).get_y()==0)) || ((legion.at(i).get_x()==3) && (legion.at(i).get_y()==3)))) && (command_i==1))	// shop
				{
					enter_store(0,i);
					valid=0;
				}
				else if (command_i==command_value)
					command_b=add_gank(0,i);
				else if (command_i==command_value+1)
					command_b=add_push(0,i);
				else if (command_i==command_value+2)
					command_b=add_farm(0,i);
				else if (command_i==command_value+3)
					command_b=add_move(0,i);
				else if (command_i==command_value+4)
					command_b=add_nothing(0,i);
				else if (command_i==command_value+5)
				{
					whois();
					valid=0;
				}
				else if (command_i==command_value+6)
				{
					system("cls");
					print_map();
					cout << endl << endl;
					valid=0;
				}
				else if (command_i==command_value+7)
				{
					tower_check();
					valid=0;
				}
				else
				{
					system("cls");
					cout << "Invalid command." << endl <<endl;
					valid=0;
				}
			}
			else
			{
				cout << legion.at(i).get_name() <<" is dead for " << legion.at(i).get_respawn() << " more turn(s). No actions can be taken." << endl;
				death_turn(0,i);
				valid=1;
				command_b=true;
				wait(2);
			}
		} while ((valid==0) || (!command_b));
	}
}

void hellb_input()
{
	int i=0,option=1;
	int valid=1,command_i=1,command_value=1;
	bool command_b;

	for(i=0;i<3;i++)
	{
		cout << endl;
		command_value=1;
		do {
			valid=1;
			command_value=1;
			command_b=false;
			disp_commands(1,i);
			if (hellb.at(i).get_alive()==1)
			{
				cout << "Enter command : ";
				while ( ! (cin >> command_i))
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					system("cls");
					cout << "Invalid input." << endl <<endl;
					disp_commands(1,i);
					cout << "Enter command : ";
				}
				if (((hellb.at(i).get_x()==0) && (hellb.at(i).get_y()==0)) || ((hellb.at(i).get_x()==3) && (hellb.at(i).get_y()==3)))
					command_value++;
				if (((((hellb.at(i).get_x()==0) && (hellb.at(i).get_y()==0)) || ((hellb.at(i).get_x()==3) && (hellb.at(i).get_y()==3)))) && (command_i==1))	// shop
				{
					enter_store(1,i);
					valid=0;
				}
				else if (command_i==command_value)
					command_b=add_gank(1,i);
				else if (command_i==command_value+1)
					command_b=add_push(1,i);
				else if (command_i==command_value+2)
					command_b=add_farm(1,i);
				else if (command_i==command_value+3)
					command_b=add_move(1,i);
				else if (command_i==command_value+4)
					command_b=add_nothing(1,i);
				else if (command_i==command_value+5)
				{
					whois();
					valid=0;
				}
				else if (command_i==command_value+6)
				{
					cout << endl << endl;
					print_map();
					cout << endl << endl;
					valid=0;
				}
				else if (command_i==command_value+7)
				{
					tower_check();
					valid=0;
				}
				else
				{
					system("cls");
					cout << "Invalid command." << endl << endl;
					valid=0;
				}
			}
			else
			{
				cout << hellb.at(i).get_name() <<" is dead for " << hellb.at(i).get_respawn() << " more turn(s). No actions can be taken." << endl;
				death_turn(1,i);
				valid=1;
				command_b=true;
				wait(2);
			}
		} while ((valid==0) || (!command_b));
	}
}

void exec_commands()
{
	cout << "Performing gank commands." << endl;
	if ((legion_gank.size()!=0) || (hellb_gank.size()!=0))
	{
		cout << endl;
		gank_fn();				
	}
	else
		cout << "None." << endl;
	cout << "Press enter to continue.";
	wait_for_enter(1);
	system("cls");

	cout << "Performing push commands." << endl;
	if ((legion_push.size()!=0) || (hellb_push.size()!=0))
	{
		cout << endl;
		push_fn();				
	}
	else
		cout << "None." << endl;
	cout << "Press enter to continue.";
	wait_for_enter(0);
	system("cls");

	cout << "Performing farm commands." << endl;
	if ((legion_farm.size()!=0) || (hellb_farm.size()!=0))
	{
		cout << endl;
		farm_fn();				
	}
	else
		cout << "None." << endl;
	cout << "Press enter to continue.";
	wait_for_enter(0);
	system("cls");

	cout << "Performing move commands." << endl;
	if ((legion_move.size()!=0) || (hellb_move.size()!=0))
	{
		cout << endl;
		move_fn();				
	}
	else
		cout << "None." << endl;
	cout << "Press enter to continue.";
	wait_for_enter(0);
	system("cls");

	cout << "Performing nothing commands." << endl;
	if ((legion_nothing.size()!=0) || (hellb_nothing.size()!=0))
	{
		cout << endl;
		nothing_fn();				
	}
	else
		cout << "None." << endl;
	cout << "Press enter to continue.";
	wait_for_enter(0);
}

void load_towers()
{
	ifstream infile;
	infile.open("Locations.txt");
	int r,c;

	for(r=0;r<4;r++)
		for(c=0;c<4;c++)
			infile>>space[r][c];
	infile.close();
}

void load_heroes()
{
	Hero temp_hero;
	ifstream infile;
	infile.open("Heroes.txt");

	while(infile.good())
	{
		infile>>temp_hero;
		hero_list.push_back(temp_hero);
		temp_hero.clear_stat_growth();
	}
	infile.close();
}

void load_store()
{
	Item temp_item;
	ifstream infile;
	infile.open("Items.txt");

	while(infile.good())
	{
		infile>>temp_item;
		store_db.push_back(temp_item);
		temp_item.clear();
	}
	infile.close();
}

void hero_selection()
{
	int mode=0,hero_select;
	int i=0,h=0,j=0;
	bool chance_b, badpick=false, valid_mode=true, valid_hero_select=true;
	vector<Hero> selection_list;

	selection_list=hero_list;

	do {
		system("cls");
		if (!valid_mode)
			cout << "Invalid input." << endl << endl;
		cout << "Select Mode:" << endl;
		cout << "1: All Pick" << endl;
		cout << "2: Random Draft" << endl;
		cout << endl;
		cout << "Enter mode : ";
		valid_mode=true;
		if (!(cin >> mode))
			valid_mode=false;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (mode==1)
		{
			for(h=1;h<4;h++)
			{
				valid_hero_select=true;
				do {
					system("cls");
					cout << "Picking Phase." << endl << endl << endl;
					if (!valid_hero_select)
						cout << "Invalid hero selection." << endl << endl;
					cout << "Select Legion Hero #" << h << ":" << endl << endl;
					for(i=0;i<selection_list.size();i++)
					{
						if (i<10)
						{
							cout << "  " << i+1 << ": ";
							cout << setw(18) << left << selection_list.at(i).get_name();
						}
						else if (i>=10 && i<100)
						{
							cout << " " << i+1 << ": ";
							cout << setw(18) << left << selection_list.at(i).get_name();
						}
						else
						{
							cout << i+1 << ": ";
							cout << setw(16) << left << selection_list.at(i).get_name();
						}		
						cout << "  |  Push: " << selection_list.at(i).get_push();
						cout << "  Gank: " << selection_list.at(i).get_gank();
						cout << "  Carry: " << selection_list.at(i).get_carry() << endl;
						cout << endl;
					}
					valid_hero_select=1;
					cout << "Choose wisely : ";
					if (!(cin >> hero_select))
					{
						valid_hero_select=false;
					}
					if ((hero_select>selection_list.size()) || (hero_select<1))
							valid_hero_select=false;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				} while (!valid_hero_select);
				selection_list.at(hero_select-1).set_coord(0,0);
				legion.push_back(selection_list.at(hero_select-1));
				selection_list.erase(selection_list.begin()+(hero_select-1));
			}
			selection_list.at(0).set_coord(3,3);	// Move hellbourne heroes to their starting location
			selection_list.at(1).set_coord(3,3);
			selection_list.at(2).set_coord(3,3);
			hellb.push_back(selection_list.at(0));
			hellb.push_back(selection_list.at(1));
			hellb.push_back(selection_list.at(2));
		}
		else if (mode==2)
		{
			for(j=0;j<selection_list.size();j++)
			{
				chance_b=chance_50_gen();
				if ((legion.size()<3) && (hellb.size()<3))
				{
					if (chance_b)
					{
						selection_list.at(j).set_coord(0,0);
						legion.push_back(selection_list.at(j));
					}
					else
					{
						selection_list.at(j).set_coord(3,3);
						hellb.push_back(selection_list.at(j));
					}
				}
				else if (hellb.size()>=3)
				{
					selection_list.at(j).set_coord(0,0);
					legion.push_back(selection_list.at(j));
				}
				else if (legion.size()>=3)
				{
					selection_list.at(j).set_coord(3,3);
					hellb.push_back(selection_list.at(j));
				}
			}
		}
		else
			valid_mode=false;
	} while (!valid_mode);
	system("cls");
	selection_list.clear();
}

void enter_store(int team,int element)
{
	int h=1,i=1,j=0,k=1;
	int item_num=0,mode=0, run_status=1;
	bool valid_mode=false, mode_bypass=false,valid_input=false;
	string confirm, noitem;
	if (team==0)	// Legion
	{
		do {
			system("cls");
			do
			{
				cout << "Welcome to the store (" << legion.at(element).get_name() << ")." << endl;
				cout << "Current gold: " << legion.at(element).get_gold() << endl;
				cout << "1: Buy" << endl;
				cout << "2: Sell" << endl;
				cout << "3: Exit" << endl;
				cout << "Enter shop mode : ";
				if (!(cin >> mode))
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					system("cls");
					valid_mode=false;
					cout << "Invalid input." << endl << endl;
				}
				else
				{
					if (mode>=1 && mode<=3)
						valid_mode=true;
					else
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						system("cls");
						valid_mode=false;
						cout << "Invalid input." << endl << endl;
					}
				}
			} while(!valid_mode);
			cout << endl;
			valid_input=true;
			switch (mode)
			{
				case 1:		do {
								if (!valid_input)
								{
									system("cls");
									cout << "Invalid input." << endl << endl;
									cout << "Welcome to the store (" << legion.at(element).get_name() << ")." << endl;
									cout << "Current gold: " << legion.at(element).get_gold() << endl;
									cout << "1: Buy" << endl;
									cout << "2: Sell" << endl;
									cout << "3: Exit" << endl;
									cout << "Enter shop mode : " << mode << endl << endl;
								}
								cout << "  1: Return to store" <<endl;
								for(i=0;i<store_db.size();i++)
								{
									h=i+2;
									if (h<10)
									{
										cout << "  " <<  h << ": ";
										cout << setw(18) << left << store_db.at(i).get_name() << setw(4) << right << " (" << store_db.at(i).get_price()<< ")";
									}
									else if (h>=10 && h<=99)
									{
										cout << " " << h << ": ";
										cout << setw(18) << left << store_db.at(i).get_name() << setw(4) << right << " (" << store_db.at(i).get_price()<< ")";
									}
									else
									{
										cout << h << ": ";
										cout << setw(18) << left << store_db.at(i).get_name() << setw(4) << right << " (" << store_db.at(i).get_price() << ")";
									}
									if (store_db.at(i).get_price()<100)
										cout << setw(7) << "P: ";
									else if (store_db.at(i).get_price()>=100 && store_db.at(i).get_price()<1000)
										cout << setw(6) << "P: ";
									else if (store_db.at(i).get_price()>=1000 && store_db.at(i).get_price()<10000)
										cout << setw(5) << "P: ";
									else
										cout << setw(4) << "P: ";
									cout << store_db.at(i).get_push() << "  G: " << store_db.at(i).get_gank() << "  C: " << store_db.at(i).get_carry() << endl;
								}
								cout << "Enter selection : ";
								if (!(cin >> item_num))
								{
									valid_input=false;
									cin.clear();
									cin.ignore(numeric_limits<streamsize>::max(), '\n');
								}
								else
								{
									if (item_num>1 && item_num<=store_db.size()+1)
									{
										valid_input=true;
										item_num--;item_num--;	// Translate input to store_db vector
										cout << "Are you sure you want to buy the " << store_db.at(item_num).get_name() << " for " <<  store_db.at(item_num).get_price() << "g? (y/n) : ";
										cin >> confirm;
										if ((confirm=="y") && (legion.at(element).items.size()<=6))
										{
											if(legion.at(element).get_gold()>=store_db.at(item_num).get_price())
											{
												legion.at(element)+store_db.at(item_num);
												legion.at(element).rem_gold(store_db.at(item_num).get_price());
											}
											else
												cout << "Not enough gold." << endl;
										}
										else if ((confirm=="y") && (legion.at(element).items.size()>=6))
										{
											cout << "Too many items; could not buy the " << store_db.at(item_num).get_name() << "." << endl;
											wait_for_enter(0);
											system("cls");
										}
										else
										{
											if (confirm!="n")
												cout << "Invalid input." << endl;
											cout << "Did not buy the " << store_db.at(item_num).get_name() << "." << endl;
											cout << "Press enter to return to store.";
											wait_for_enter(1);
											system("cls");
										}
									}
									else if (item_num==1)
										valid_input=true;
									else
										valid_input=false;
								}
							} while (!valid_input);
							
							break;
				case 2:		do {
								if (!valid_input)
								{
									system("cls");
									cout << "Invalid input." << endl << endl;
									cout << "Welcome to the store (" << legion.at(element).get_name() << ")." << endl;
									cout << "Current gold: " << legion.at(element).get_gold() << endl;
									cout << "1: Buy" << endl;
									cout << "2: Sell" << endl;
									cout << "3: Exit" << endl;
									cout << "Enter shop mode : " << mode << endl << endl;
								}
								if (legion.at(element).items.size()==0)
								{
									cout << "No items to sell." << endl;
									cout << "Press enter to return to store.";
									wait_for_enter(1);
									valid_input=1;
									system("cls");
								}
								else
								{
									cout << "1: Return to store" <<endl;
									for(j=0;j<legion.at(element).items.size();j++)
									{
										k=j+2;
										cout << k << ": " << setw(18) << left << legion.at(element).items.at(j).get_name();
										cout << "(" << (.5)*legion.at(element).items.at(j).get_price() << ")" << endl;
									}
									cout << "What would you like to sell? : ";
									if (!(cin >> item_num))
									{
										valid_input=false;
										cin.clear();
										cin.ignore(numeric_limits<streamsize>::max(), '\n');
									}
									else
									{
										if (item_num>1 && item_num<=legion.at(element).items.size()+1)
										{
											valid_input=true;
											item_num--;item_num--;
											cout << "Are you sure you want to sell the " << legion.at(element).items.at(item_num).get_name() << " for " << (.5)*legion.at(element).items.at(item_num).get_price() << "g? (y/n) : ";
											cin >> confirm;
											if (confirm=="y")
											{
												legion.at(element).add_gold((.5)*legion.at(element).items.at(item_num).get_price());
												legion.at(element)-legion.at(element).items.at(item_num);
											}
											else
											{
												cout << "Did not sell the " << legion.at(element).items.at(item_num).get_name() << "." << endl;
												cout << "Press enter to return to store.";
												wait_for_enter(1);
												system("cls");
											}
										}
										else if (item_num==1)
											valid_input=true;
										else
											valid_input=false;
									}
								}
							} while (!valid_input);
							break;
				case 3:		run_status=0;
							system("cls");
							break;
				default:	system("cls");
							cout << "Problem occured." << endl <<endl;
							wait(3);
			}
		} while (run_status==1);
	}
	else	// Hellbourne
		{
		do {
			system("cls");
			do
			{
				cout << "Welcome to the store (" << hellb.at(element).get_name() << ")." << endl;
				cout << "Current gold: " << hellb.at(element).get_gold() << endl;
				cout << "1: Buy" << endl;
				cout << "2: Sell" << endl;
				cout << "3: Exit" << endl;
				cout << "Enter shop mode : ";
				if (!(cin >> mode))
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					system("cls");
					valid_mode=false;
					cout << "Invalid input." << endl << endl;
				}
				else
				{
					if (mode>=1 && mode<=3)
						valid_mode=true;
					else
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						system("cls");
						valid_mode=false;
						cout << "Invalid input." << endl << endl;
					}
				}
			} while(!valid_mode);
			cout << endl;
			valid_input=true;
			switch (mode)
			{
				case 1:		do {
								if (!valid_input)
								{
									system("cls");
									cout << "Invalid input." << endl << endl;
									cout << "Welcome to the store (" << hellb.at(element).get_name() << ")." << endl;
									cout << "Current gold: " << hellb.at(element).get_gold() << endl;
									cout << "1: Buy" << endl;
									cout << "2: Sell" << endl;
									cout << "3: Exit" << endl;
									cout << "Enter shop mode : " << mode << endl << endl;
								}
								cout << "  1: Return to store" <<endl;
								for(i=0;i<store_db.size();i++)
								{
									h=i+2;
									if (h<10)
									{
										cout << "  " <<  h << ": ";
										cout << setw(18) << left << store_db.at(i).get_name() << setw(4) << right << " (" << store_db.at(i).get_price()<< ")";
									}
									else if (h>=10 && h<=99)
									{
										cout << " " << h << ": ";
										cout << setw(18) << left << store_db.at(i).get_name() << setw(4) << right << " (" << store_db.at(i).get_price()<< ")";
									}
									else
									{
										cout << h << ": ";
										cout << setw(18) << left << store_db.at(i).get_name() << setw(4) << right << " (" << store_db.at(i).get_price() << ")";
									}
									if (store_db.at(i).get_price()<100)
										cout << setw(7) << "P: ";
									else if (store_db.at(i).get_price()>=100 && store_db.at(i).get_price()<1000)
										cout << setw(6) << "P: ";
									else if (store_db.at(i).get_price()>=1000 && store_db.at(i).get_price()<10000)
										cout << setw(5) << "P: ";
									else
										cout << setw(4) << "P: ";
									cout << store_db.at(i).get_push() << "  G: " << store_db.at(i).get_gank() << "  C: " << store_db.at(i).get_carry() << endl;
								}
								cout << "Enter selection : ";
								if (!(cin >> item_num))
								{
									valid_input=false;
									cin.clear();
									cin.ignore(numeric_limits<streamsize>::max(), '\n');
								}
								else
								{
									if (item_num>1 && item_num<=store_db.size()+1)
									{
										valid_input=true;
										item_num--;item_num--;	// Translate input to store_db vector
										cout << "Are you sure you want to buy the " << store_db.at(item_num).get_name() << " for " <<  store_db.at(item_num).get_price() << "g? (y/n) : ";
										cin >> confirm;
										if ((confirm=="y") && (hellb.at(element).items.size()<=6))
										{
											if(hellb.at(element).get_gold()>=store_db.at(item_num).get_price())
											{
												hellb.at(element)+store_db.at(item_num);
												hellb.at(element).rem_gold(store_db.at(item_num).get_price());
											}
											else
												cout << "Not enough gold." << endl;
										}
										else if ((confirm=="y") && (hellb.at(element).items.size()>=6))
										{
											cout << "Too many items; could not buy the " << store_db.at(item_num).get_name() << "." << endl;
											wait_for_enter(0);
											system("cls");
										}
										else
										{
											if (confirm!="n")
												cout << "Invalid input." << endl;
											cout << "Did not buy the " << store_db.at(item_num).get_name() << "." << endl;
											cout << "Press enter to return to store.";
											wait_for_enter(1);
											system("cls");
										}
									}
									else if (item_num==1)
										valid_input=true;
									else
										valid_input=false;
								}
							} while (!valid_input);
							
							break;
				case 2:		do {
								if (!valid_input)
								{
									system("cls");
									cout << "Invalid input." << endl << endl;
									cout << "Welcome to the store (" << hellb.at(element).get_name() << ")." << endl;
									cout << "Current gold: " << hellb.at(element).get_gold() << endl;
									cout << "1: Buy" << endl;
									cout << "2: Sell" << endl;
									cout << "3: Exit" << endl;
									cout << "Enter shop mode : " << mode << endl << endl;
								}
								if (hellb.at(element).items.size()==0)
								{
									cout << "No items to sell." << endl;
									cout << "Press enter to return to store.";
									wait_for_enter(1);
									valid_input=1;
									system("cls");
								}
								else
								{
									cout << "1: Return to store" <<endl;
									for(j=0;j<hellb.at(element).items.size();j++)
									{
										k=j+2;
										cout << k << ": " << setw(18) << left << hellb.at(element).items.at(j).get_name();
										cout << "(" << (.5)*hellb.at(element).items.at(j).get_price() << ")" << endl;
									}
									cout << "What would you like to sell? : ";
									if (!(cin >> item_num))
									{
										valid_input=false;
										cin.clear();
										cin.ignore(numeric_limits<streamsize>::max(), '\n');
									}
									else
									{
										if (item_num>1 && item_num<=hellb.at(element).items.size()+1)
										{
											valid_input=true;
											item_num--;item_num--;
											cout << "Are you sure you want to sell the " << hellb.at(element).items.at(item_num).get_name() << " for " << (.5)*hellb.at(element).items.at(item_num).get_price() << "g? (y/n) : ";
											cin >> confirm;
											if (confirm=="y")
											{
												hellb.at(element).add_gold((.5)*hellb.at(element).items.at(item_num).get_price());
												hellb.at(element)-hellb.at(element).items.at(item_num);
											}
											else
											{
												cout << "Did not sell the " << hellb.at(element).items.at(item_num).get_name() << "." << endl;
												cout << "Press enter to return to store.";
												wait_for_enter(1);
												system("cls");
											}
										}
										else if (item_num==1)
											valid_input=true;
										else
											valid_input=false;
									}
								}
							} while (!valid_input);
							break;
				case 3:		run_status=0;
							system("cls");
							break;
				default:	system("cls");
							cout << "Problem occured." << endl <<endl;
							wait(3);
			}
		} while (run_status==1);
	}
}

void intro()
{
	cout << "MOBA v0.1.3;" << endl;
	cout << "Designed for Screen Buffer Size 80 x 80." << endl << endl;
}

void whois()
{
	int i, hero_select;
	bool valid_input=true, running_status=true;
	char character_remove;

	do {
		do {
			system("cls");
			if (!valid_input)
			{
				system("cls");
				cout << "Invalid input." << endl << endl;
			}
			cout << "Who is list:" << endl << endl;
			cout << "1: Exit" <<endl << endl;
			cout << "Legion Team:" << endl;
			cout << "2: " << legion.at(0).get_name() << endl;
			cout << "3: " << legion.at(1).get_name() << endl;
			cout << "4: " << legion.at(2).get_name() << endl << endl;
			cout << "Hellbourne Team:" << endl;
			cout << "5: " << hellb.at(0).get_name() << endl;
			cout << "6: " << hellb.at(1).get_name() << endl;
			cout << "7: " << hellb.at(2).get_name() << endl << endl;
			cout << "Enter hero to check : ";
			if (!(cin >> hero_select))
			{
				valid_input=false;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			else
			{
				valid_input=true;
				if (hero_select!=1)
				{
					hero_select--; hero_select--;
					switch (hero_select)
					{
						case 0:		cout << endl << legion.at(0);
									cout << "Press enter to return to selection.";
									wait_for_enter(1);
									break;
						case 1:		cout << endl << legion.at(1);
									cout << "Press enter to return to selection.";
									wait_for_enter(1);
									break;
						case 2:		cout << endl << legion.at(2);
									cout << "Press enter to return to selection.";
									wait_for_enter(1);
									break;
						case 3:		cout << endl << hellb.at(0);
									cout << "Press enter to return to selection.";
									wait_for_enter(1);
									break;
						case 4:		cout << endl << hellb.at(1);
									cout << "Press enter to return to selection.";
									wait_for_enter(1);
									break;
						case 5:		cout << endl << hellb.at(2);
									cout << "Press enter to return to selection.";
									wait_for_enter(1);
									break;
						default:	valid_input=false;

					}
				}
				else
					running_status=false;
			}
		} while (!valid_input);
	} while (running_status);
	system("cls");
}

void tower_check()
{
	int x_in,y_in;

	cout << "What tower do you want to check? (x y) : ";
	cin >> x_in >> y_in;
	cout << endl;
	if (((x_in==0) && (y_in==0)) || ((x_in==3) && (y_in==3)))
	{
		if (x_in==0)
			cout << "The Legion base ";
		else if (x_in==3)
			cout << "The Hellbourne base ";
		if (space[x_in][y_in].get_hp()!=0)
			cout << "has " << space[x_in][y_in].get_hp() << "HP." << endl;
	}
	else
	{
		if (tower_present(x_in,y_in))
		{
			cout << "The tower at (" << x_in << "," << y_in << ") ";
			if (space[x_in][y_in].get_hp()!=0)
				cout << "has " << space[x_in][y_in].get_hp() << "HP." << endl;
			else
				cout << "has been destroyed." << endl;
		}
		else
			cout << "No tower at specified location." << endl;
	}
	cout << endl;
}

bool add_gank(int team,int element)
{
	bool answer=false;
	int x_in,y_in,x_from_v,y_from_v;
	int i;
	string confirm;
	Command temp_command;

	if (team==0)	// Legion
	{
		cout << "Where do you want " << legion.at(element).get_name() << " to gank? (x y) : ";
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << legion.at(element).get_name() << " to gank (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (gank_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,1)))
		{
			// Check to see if someone is ganking same location, add this hero to heroes vector if it exists
			for(i=0;i<legion_gank.size();i++)
			{
				x_from_v=legion_gank.at(i).get_x();
				y_from_v=legion_gank.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					legion_gank.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is ganking same location, create new command element in legion_gank vector
			{
				temp_command="gank";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				legion_gank.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!gank_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,0)))
			answer=false;
		else
		{
			cout << "Did not save gank command for " << legion.at(element).get_name() << endl;
			answer=false;
		}

	}
	else	// Hellbourne
	{
		cout << "Where do you want " << hellb.at(element).get_name() << " to gank? (x y) :" << endl;
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << hellb.at(element).get_name() << " to gank (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (gank_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,1)))
		{
			// Check to see if someone is ganking same location, add this hero to heroes vector if it exists
			for(i=0;i<hellb_gank.size();i++)
			{
				x_from_v=hellb_gank.at(i).get_x();
				y_from_v=hellb_gank.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					hellb_gank.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is ganking same location, create new command element in legion_gank vector
			{
				temp_command="gank";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				hellb_gank.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!gank_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,0)))
			answer=false;
		else
		{
			cout << "Did not save gank command for " << hellb.at(element).get_name() << endl;
			answer=false;
		}
	}
	return answer;
}

bool add_push(int team,int element)
{
	bool answer=false;
	int x_in,y_in,x_from_v,y_from_v;
	int i;
	string confirm;
	Command temp_command;

	if (team==0)	// Legion
	{
		cout << "Where do you want " << legion.at(element).get_name() << " to push? (x y) : ";
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << legion.at(element).get_name() << " to push (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (push_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,1)))
		{
			// Check to see if someone is pushing same location, add this hero to heroes vector if it exists
			for(i=0;i<legion_push.size();i++)
			{
				x_from_v=legion_push.at(i).get_x();
				y_from_v=legion_push.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					legion_push.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is pushing same location, create new command element in legion_push vector
			{
				temp_command="push";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				legion_push.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!push_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,0)))
			answer=false;
		else
		{
			cout << "Did not save push command for " << legion.at(element).get_name() << endl;
			answer=false;
		}
	}
	else	// Hellbourne
	{
		cout << "Where do you want " << hellb.at(element).get_name() << " to push? (x y) : ";
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << hellb.at(element).get_name() << " to push (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (push_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,1)))
		{
			// Check to see if someone is pushing same location, add this hero to heroes vector if it exists
			for(i=0;i<hellb_push.size();i++)
			{
				x_from_v=hellb_push.at(i).get_x();
				y_from_v=hellb_push.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					hellb_push.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is pushing same location, create new command element in hellb_push vector
			{
				temp_command="push";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				hellb_push.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!push_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,0)))
			answer=false;
		else
		{
			cout << "Did not save push command for " << hellb.at(element).get_name() << endl;
			answer=false;
		}
	}
	return answer;
}

bool add_farm(int team,int element)
{
	bool answer=false;
	int x_in,y_in,x_from_v,y_from_v;
	int i;
	string confirm;
	Command temp_command;

	if (team==0)	// Legion
	{
		cout << "Where do you want " << legion.at(element).get_name() << " to farm? (x y) : ";
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << legion.at(element).get_name() << " to farm (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (farm_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,0)))
		{
			// Check to see if someone is farming same location, add this hero to heroes vector if it exists
			for(i=0;i<legion_farm.size();i++)
			{
				x_from_v=legion_farm.at(i).get_x();
				y_from_v=legion_farm.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					legion_farm.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is farming same location, create new command element in legion_farm vector
			{
				temp_command="farm";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				legion_farm.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!farm_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,1)))
			answer=false;
		else
		{
			cout << "Did not save farm command for " << legion.at(element).get_name() << endl;
			answer=false;
		}
	}
	else	// Hellbourne
	{
		cout << "Where do you want " << hellb.at(element).get_name() << " to farm? (x y) : ";
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << hellb.at(element).get_name() << " to farm (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (farm_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,0)))
		{
			// Check to see if someone is farming same location, add this hero to heroes vector if it exists
			for(i=0;i<hellb_farm.size();i++)
			{
				x_from_v=hellb_farm.at(i).get_x();
				y_from_v=hellb_farm.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					hellb_farm.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is farming same location, create new command element in hellb_farm vector
			{
				temp_command="farm";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				hellb_farm.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!farm_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,1)))
			answer=false;
		else
		{
			cout << "Did not save farm command for " << hellb.at(element).get_name() << endl;
			answer=false;
		}
	}
	return answer;
}

bool add_move(int team,int element)
{
	bool answer=false;
	int x_in,y_in,x_from_v,y_from_v;
	int i;
	string confirm;
	Command temp_command;

	if (team==0)	// Legion
	{
		cout << "Where do you want " << legion.at(element).get_name() << " to move? (x y) : ";
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << legion.at(element).get_name() << " to move (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (move_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,0)))
		{
			// Check to see if someone is moveing same location, add this hero to heroes vector if it exists
			for(i=0;i<legion_move.size();i++)
			{
				x_from_v=legion_move.at(i).get_x();
				y_from_v=legion_move.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					legion_move.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is moveing same location, create new command element in legion_move vector
			{
				temp_command="move";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				legion_move.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!move_loc_valid(legion.at(element).get_x(),legion.at(element).get_y(),x_in,y_in,1)))
			answer=false;
		else
		{
			cout << "Did not save move command for " << legion.at(element).get_name() << endl;
			answer=false;
		}
	}
	else	// Hellbourne
	{
		cout << "Where do you want " << hellb.at(element).get_name() << " to move? (x y) : ";
		cin >> x_in >> y_in;
		cout << "Are you sure you want " << hellb.at(element).get_name() << " to move (" << x_in << "," << y_in << ")? (y/n) : ";
		cin >> confirm;
		if ((confirm=="y") && (move_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,0)))
		{
			// Check to see if someone is moveing same location, add this hero to heroes vector if it exists
			for(i=0;i<hellb_move.size();i++)
			{
				x_from_v=hellb_move.at(i).get_x();
				y_from_v=hellb_move.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					hellb_move.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is moveing same location, create new command element in hellb_move vector
			{
				temp_command="move";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				hellb_move.push_back(temp_command);
				answer=true;
			}
		}
		else if ((confirm=="y") && (!move_loc_valid(hellb.at(element).get_x(),hellb.at(element).get_y(),x_in,y_in,1)))
			answer=false;
		else
		{
			cout << "Did not save move command for " << hellb.at(element).get_name() << endl;
			answer=false;
		}
	}
	return answer;
}

bool add_nothing(int team,int element)
{
	bool answer=false;
	int x_in,y_in,x_from_v,y_from_v;
	int i;
	string confirm;
	Command temp_command;

	if (team==0)
	{
		x_in=legion.at(element).get_x();
		y_in=legion.at(element).get_y();
	}
	else
	{
		x_in=hellb.at(element).get_x();
		y_in=hellb.at(element).get_y();
	}

	if (team==0)	// Legion
	{
		cout << "Are you sure you want " << legion.at(element).get_name() << " to do nothing? (y/n) : ";
		cin >> confirm;

		if (confirm=="y")
		{
			// Check to see if someone is nothing same location, add this hero to heroes vector if it exists
			for(i=0;i<legion_nothing.size();i++)
			{
				x_from_v=legion_nothing.at(i).get_x();
				y_from_v=legion_nothing.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					legion_nothing.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is nothing same location, create new command element in legion_nothing vector
			{
				temp_command="nothing";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				legion_nothing.push_back(temp_command);
				answer=true;
			}
		}
		else
		{
			cout << "Did not save nothing command for " << legion.at(element).get_name() << endl;
			answer=false;
		}
	}
	else	// Hellbourne
	{
		cout << "Are you sure you want " << hellb.at(element).get_name() << " to do nothing? (y/n) : ";
		cin >> confirm;
		if (confirm=="y")
		{
			// Check to see if someone is nothing same location, add this hero to heroes vector if it exists
			for(i=0;i<hellb_nothing.size();i++)
			{
				x_from_v=hellb_nothing.at(i).get_x();
				y_from_v=hellb_nothing.at(i).get_y();
				if ((x_in==x_from_v) && (y_in==y_from_v))
				{
					hellb_nothing.at(i).set_heroes(element);
					answer=true;
				}
			}
			if (!answer) // If no one is nothing same location, create new command element in hellb_nothing vector
			{
				temp_command="nothing";
				temp_command.set_heroes(element);
				temp_command.set_coord(x_in,y_in);
				hellb_nothing.push_back(temp_command);
				answer=true;
			}
		}
		else
		{
			cout << "Did not save nothing command for " << hellb.at(element).get_name() << endl;
			answer=false;
		}
	}

	return answer;
}

void death_turn(int team,int element)
{
	int i=0;
	int x_from_v,y_from_v,x_in,y_in;
	bool answer=false;
	Command temp_command;

	if (team==0)
	{
		x_in=legion.at(element).get_x();
		y_in=legion.at(element).get_y();
	}
	else
	{
		x_in=hellb.at(element).get_x();
		y_in=hellb.at(element).get_y();
	}

	if (team==0)
	{
		for(i=0;i<legion_nothing.size();i++)
		{
			x_from_v=legion_nothing.at(i).get_x();
			y_from_v=legion_nothing.at(i).get_y();
			if ((x_in==x_from_v) && (y_in==y_from_v))
			{
				legion_nothing.at(i).set_heroes(element);
				answer=true;
			}
		}
		if (!answer) // If no one is nothing same location, create new command element in legion_nothing vector
		{
			temp_command="nothing";
			temp_command.set_heroes(element);
			temp_command.set_coord(x_in,y_in);
			legion_nothing.push_back(temp_command);
			answer=true;
		}
	}
	else
	{
		for(i=0;i<hellb_nothing.size();i++)
		{
			x_from_v=hellb_nothing.at(i).get_x();
			y_from_v=hellb_nothing.at(i).get_y();
			if ((x_in==x_from_v) && (y_in==y_from_v))
			{
				hellb_nothing.at(i).set_heroes(element);
				answer=true;
			}
		}
		if (!answer) // If no one is nothing same location, create new command element in hellb_nothing vector
		{
			temp_command="nothing";
			temp_command.set_heroes(element);
			temp_command.set_coord(x_in,y_in);
			hellb_nothing.push_back(temp_command);
			answer=true;
		}
	}
}

bool gank_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error)
{
	// gank location must be 1 unit away and there must be a enemy hero there
	bool answer=false;


	if (!valid_path(x_origin,y_origin,x_dest,y_dest))
	{
		if (print_error==1)
			cout << "Invalid coordinate selected. Must be 1 unit away, and be possible to get there." << endl;
	}
	else
		if (enemy_hero_present(x_origin,y_origin,x_dest,y_dest))
			answer=true;
		else
			if (print_error==1)
				cout << "No enemy hero at the selected coordinate." << endl;

	return answer;
}

bool push_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error)
{
	// push must be 1 unit away and there must be an enemy tower there. 
	bool answer=false;
	int team=0;
	
	if (((x_dest==0) && (y_dest==0)) || ((x_dest==3) && (y_dest==3)))	// Base check
	{
		if (x_dest==0)	// See if Legion base can be attacked
		{
			if (!valid_path(x_origin,y_origin,x_dest,y_dest))
			{
				if (print_error==1)
					cout << "Invalid coordinate selected. Must be 1 unit away, and be possible to get there." << endl;
			}
			else
			{
				if ((base_vulnerable(0)))
					answer=true;
				else
				{
					if (print_error==1)
						cout << "The Legion base is invulnerable. You must kill a surrounding tower first." << endl;
				}
			}
		}
		else		// See if Hellbourne base can be attacked
		{
			if (!valid_path(x_origin,y_origin,x_dest,y_dest))
			{
				if (print_error==1)
					cout << "Invalid coordinate selected. Must be 1 unit away, and be possible to get there." << endl;
			}
			else
			{
				if (base_vulnerable(1))
					answer=true;
				else
				{
					if (print_error==1)
						cout << "The Hellbourne base is invulnerable. You must kill a surrounding tower first." << endl;
				}
			}
		}
	}
	else	// Normal tower check
	{
		if (!valid_path(x_origin,y_origin,x_dest,y_dest))
		{
			if (print_error==1)
				cout << "Invalid coordinate selected. Must be 1 unit away, and be possible to get there." << endl;
		}
		else
		{
			if ((enemy_tower_present(x_origin,y_origin,x_dest,y_dest)) && (space[x_dest][y_dest].get_hp()!=0))
				answer=true;
			else
				if (print_error==1)
					cout << "No enemy tower at the selected coordinate." << endl;
		}
	}

	return answer;
}

bool farm_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error)
{
	// farm must target 1 unit away and there cannot be an enemy hero or friendly/enemy tower there.
	bool answer=false;

	if (!valid_path(x_origin,y_origin,x_dest,y_dest))
	{
		if (print_error==1)
			cout << "Invalid coordinate selected. Must be 1 unit away, and be possible to get there." << endl;
	}
	else
	{
		if (!tower_present(x_dest,y_dest))
		{
			answer=true;
		}
		else
		{
			if (print_error==1)
			{
				cout << "Cannot farm selected coordinate because a tower is at the location." << endl;
			}
		}
	}
	
	return answer;
}

bool move_loc_valid(int x_origin,int y_origin,int x_dest,int y_dest,int print_error)
{
	// move must target 1 unit away and there cannot be a enemy hero there
	bool answer=false;

	if (!valid_path(x_origin,y_origin,x_dest,y_dest))
	{
		if (print_error==1)
			cout << "Invalid coordinate selected. Must be 1 unit away, and be possible to get there." << endl;
	}
	else
		if (!enemy_hero_present(x_origin,y_origin,x_dest,y_dest))
			answer=true;
		else
			if (print_error==1)
				cout << "Cannot move to selected coordinate because an enemy hero is at the location." << endl;
	
	return answer;
}

void gank_fn()
{	// ganker=gank+(1/2)carry
	// gankee=(1/3)gank+carry

	bool team_b=false,result=false,fail_b=false;
	double ganker_d=0,gankee_d=0;
	int ganker=0,gankee=0;
	int team=0,element=0;
	int count=0,num_ganker=0,num_gankee=0;
	int i=0,j=0,k=0,m=0,n=0,passes=0,passes_2=0;
	vector<int> temp_heroes;

	// decide what element gets executed
	while ((legion_gank.size()!=0) || (hellb_gank.size()!=0))
	{
		num_ganker=0;
		num_gankee=0;
		passes=0;
		passes_2=0;
		team_b=chance_50_gen();
		if (legion_gank.size()==0)
			team_b=false;
		else if (hellb_gank.size()==0)
			team_b=true;
		if (team_b==true)
			element=pick_element(legion_gank.size());
		else
			element=pick_element(hellb_gank.size());
		passes=0;
		if (team_b)	// execute a legion command
		{
			team=0;
			temp_heroes=legion_gank.at(element).get_heroes();
			cout << "Executing gank by Legion member(s)." << endl << endl;
			for(i=0;i<3;i++)
			{
				if (temp_heroes.at(i)==1)
				{
					if (passes>0)
					{
						cout << " + " << legion.at(i).get_name();
					}
					else
					{
						cout << "   " << legion.at(i).get_name();
					}
					ganker_d+=legion.at(i).get_gank()+(1/2)*legion.at(i).get_carry(); // Finds ganker total
					passes++;
					num_ganker++;
				}
			}
			if (passes>0)
			{
				cout << endl << "      vs." << endl;
				for(j=0;j<3;j++)
				{
					if((legion_gank.at(element).get_x()==hellb.at(j).get_x()) && (legion_gank.at(element).get_y()==hellb.at(j).get_y()))
					{
						if (passes_2>0)
							cout << " + " << hellb.at(j).get_name();
						else
							cout << "   " << hellb.at(j).get_name();
						gankee_d+=hellb.at(j).get_carry()+(1/3)*hellb.at(j).get_gank(); // Finds gankee total
						passes_2++;
						num_gankee++;
					}
				}
			}
			else
				cout << "   Legion members died before their turn." << endl;
			if (passes_2==0)
			{
				cout << "   No one. All Hellbourne members previously at (" << legion_gank.at(element).get_x() << "," << legion_gank.at(element).get_y() << ") already died." << endl;
			}
		}
		else	// execute a hellb command
		{
			team=1;
			temp_heroes=hellb_gank.at(element).get_heroes();
			cout << "Executing gank by Hellbourne member(s)." << endl << endl;
			for(i=0;i<3;i++)
			{
				if (temp_heroes.at(i)==1)
				{	
					if (passes>0)
					{
						cout << " + " << hellb.at(i).get_name();
					}
					else
					{
						cout << "   " << hellb.at(i).get_name();
					}
					ganker_d+=hellb.at(i).get_gank()+(1/2)*hellb.at(i).get_carry();
					passes++;
					num_ganker++;
				}
			}
			if (passes>0)
			{
				cout << endl << "      vs." << endl;
				for(j=0;j<3;j++)
				{
					if((hellb_gank.at(element).get_x()==legion.at(j).get_x()) && (hellb_gank.at(element).get_y()==legion.at(j).get_y()))
					{
						if (passes_2>0)
							cout << " + " << legion.at(j).get_name();
						else
							cout << "   " << legion.at(j).get_name();
						gankee_d+=legion.at(j).get_carry()+(1/3)*legion.at(j).get_gank(); // Finds gankee total
						passes_2++;
						num_gankee++;
					}
				}
			}
			else
				cout << "   Hellbourne members died before their turn." << endl;
			if (passes_2==0)
			{
				cout << "   No one. All Legion members previously at (" << hellb_gank.at(element).get_x() << "," << hellb_gank.at(element).get_y() << ") already died." << endl;
			}
		}
		if (passes>0)
		{

			ganker=ganker_d*(1+(.2*num_ganker));	// truncate double
			gankee=gankee_d*(1+(.2*num_gankee));	// truncate double
			result=outcome_gen(ganker,gankee);
		
			cout << endl << endl << "   Result: ";

			if ((result) && (team==0))			// legion successful
			{// kill hellb members at location where legion ganked
				cout << "Legion successful." << endl;
				cout << "      Hellbourne members that died:" << endl;
				for(n=0;n<3;n++)
				{
					if (temp_heroes.at(n)==1)
					{
						legion.at(n).add_exp((300*num_gankee)/num_ganker);
						legion.at(n).add_gold((350*num_gankee)/num_ganker);
					}
				}
				for(k=0;k<3;k++)
				{
					if((legion_gank.at(element).get_x()==hellb.at(k).get_x()) && (legion_gank.at(element).get_y()==hellb.at(k).get_y()))
					{
						cout << "         " << hellb.at(k).get_name() << endl;
						hero_death(1,k);
					}
				}
				cout << endl;
			}
			else if	((!result) && (team==0))	// legion unsuccessful
			{// 25% chance @ death for each legion hero that participated
				cout << "Legion unsuccessful." << endl;
				cout << "      Legion members that died:" << endl;
				for(m=0;m<3;m++)
				{
					if(temp_heroes.at(m)==1)
					{
						fail_b=chance_25_gen();
						if (fail_b)
						{
							cout << "         " << legion.at(m).get_name() << endl;
							count++;
							hero_death(0,m);
						}
					}
				}
				if (count==0)
					cout << "         None." << endl << endl;
				else
					cout << endl;
			}
			else if ((result) && (team==1))		// hellb successful
			{// kill legion members at location where the hellb ganked
				cout << "Hellbourne successful." << endl;
				cout << "      Legion members that died:" << endl;
				for(n=0;n<3;n++)
				{
					if (temp_heroes.at(n)==1)
					{
						hellb.at(n).add_exp((300*num_gankee)/num_ganker);
						hellb.at(n).add_gold((350*num_gankee)/num_ganker);
					}
				}
				for(k=0;k<3;k++)
				{
					if((hellb_gank.at(element).get_x()==legion.at(k).get_x()) && (hellb_gank.at(element).get_y()==legion.at(k).get_y()))
					{
						cout << "         " << legion.at(k).get_name() << endl;
						hero_death(0,k);
					}
				}
				cout << endl;
			}
			else if	((!result) && (team==1))	// hellb unsuccessful
			{// 25% chance @ death for each hellb hero that participated
				cout << "Hellbourne unsuccessful." << endl;
				cout << "      Hellbourne members that died:" << endl;
				for(m=0;m<3;m++)
				{
					if(temp_heroes.at(m)==1)
					{
						fail_b=chance_25_gen();
						if (fail_b)
						{
							cout << "         " << hellb.at(m).get_name() << endl;
							count++;
							hero_death(1,m);
						}
					}
				}
				if (count==0)
					cout << "         None." << endl << endl;
				else
					cout << endl;
			}
		}
		if (team==0)
			legion_gank.erase(legion_gank.begin()+element);	// erase executed command
		else
			hellb_gank.erase(hellb_gank.begin()+element);	// erase executed command
	}
}

void push_fn()
{	// pusher=push
	// pushee=carry

	bool team_b=false,result=false,fail_b=false;
	double pusher_d=0,pushee_d=0;
	int pusher=0,pusher_no_bonus=0,pushee=0;
	int team=0,element=0;
	int count=0,num_pusher=0,num_pushee=0;
	int i=0,j=0,k=0,m=0,n=0,passes=0,passes_2=0;
	vector<int> temp_heroes;

	// decide what element gets executed
	while (((legion_push.size()!=0) || (hellb_push.size()!=0)) && (space[0][0].get_hp()!=0) && (space[3][3].get_hp()!=0))
	{
		num_pusher=0;
		num_pushee=0;
		if (legion_push.size()==0)
			team_b=false;
		else if (hellb_push.size()==0)
			team_b=true;
		if (team_b==true)
			element=pick_element(legion_push.size());
		else
			element=pick_element(hellb_push.size());
		passes=0;
		if (team_b==true)	// execute a legion command
		{
			team=0;
			temp_heroes=legion_push.at(element).get_heroes();
			cout << "Executing push by Legion member(s)." << endl << endl;
			for(i=0;i<3;i++)
			{
				if (temp_heroes.at(i)==1)
				{
					if (passes>0)
					{
						cout << " + " << legion.at(i).get_name();
					}
					else
					{
						cout << legion.at(i).get_name();
					}
					pusher_d+=legion.at(i).get_push(); // Finds pusher total
					passes++;
					num_pusher++;
				}
			}
			cout << endl << "  vs." << endl;
			for(j=0;j<3;j++)
			{
				if((legion_push.at(element).get_x()==hellb.at(j).get_x()) && (legion_push.at(element).get_y()==hellb.at(j).get_y()))
				{
					if (passes_2>0)
						cout << " + " << hellb.at(j).get_name();
					else
						cout << hellb.at(j).get_name();
					pushee_d+=hellb.at(j).get_carry(); // Finds pushee total
					passes_2++;
					num_pushee++;
				}
			}
			if (passes_2==0)
				cout << "No one." << endl;
		}
		else	// execute a hellb command
		{
			team=1;
			temp_heroes=hellb_push.at(element).get_heroes();
			cout << "Executing push by Hellbourne member(s)." << endl << endl;
			for(i=0;i<3;i++)
			{
				if (temp_heroes.at(i)==1)
				{
					if (passes>0)
					{
						cout << " + " << hellb.at(i).get_name();
					}
					else
					{
						cout << hellb.at(i).get_name();
					}
					pusher_d+=hellb.at(i).get_push(); // Finds pusher total
					passes++;
					num_pusher++;
				}
			}
			cout << endl << "  vs." << endl;
			for(j=0;j<3;j++)
			{
				if((hellb_push.at(element).get_x()==hellb.at(j).get_x()) && (hellb_push.at(element).get_y()==hellb.at(j).get_y()))
				{
					if (passes_2>0)
						cout << " + " << hellb.at(j).get_name();
					else
						cout << hellb.at(j).get_name();
					pushee_d+=hellb.at(j).get_carry(); // Finds pushee total
					passes_2++;
					num_pushee++;
				}
			}
			if (passes_2==0)
				cout << "No one." << endl;
		}
		pusher_no_bonus=pusher_d;				// truncate double
		pusher=pusher_d*(1+(.2*num_pusher));	// truncate double
		pushee=pushee_d*(1+(.2*num_pushee));	// truncate double
		result=outcome_gen(pusher,pushee);
		
		cout << endl << endl << "Result: ";

		if ((result) && (team==0))			// legion successful
		{// damage tower
			cout << "Legion successful." << endl;
			space[legion_push.at(element).get_x()][legion_push.at(element).get_y()].take_dam(pusher_no_bonus);

			cout << "The Legion inflicted " << pusher_no_bonus << " damage to the ";
			if ((legion_push.at(element).get_x()==3) && (legion_push.at(element).get_y()==3))
				cout << "Hellbourne base ";
			else
				cout << "tower ";
			cout << "at (" << legion_push.at(element).get_x() << "," << legion_push.at(element).get_y() << ")." << endl;
			if (space[legion_push.at(element).get_x()][legion_push.at(element).get_y()].get_hp()==0)		// If tower died
			{
				cout << "The Legion destroyed ";
				if ((legion_push.at(element).get_x()==3) && (legion_push.at(element).get_y()==3))
				cout << "the Hellbourne base." << endl;
					else
				cout << "a Hellbourne tower." << endl;
				for(n=0;n<3;n++)
					legion.at(n).add_gold(325);
			}
			else
				cout << "The Hellbourne tower has " << space[legion_push.at(element).get_x()][legion_push.at(element).get_y()].get_hp() << "HP remaining." << endl;
		}
		else if	((!result) && (team==0))	// legion unsuccessful
		{// 25% chance @ death for each legion hero that participated
			cout << "Legion unsuccessful." << endl;
			cout << "Legion members that died:" << endl;
			for(m=0;m<3;m++)
			{
				if(temp_heroes.at(m)==1)
				{
					fail_b=chance_25_gen();
					if (fail_b)
					{
						cout << "   " << legion.at(m).get_name() << endl;
						count++;
						hero_death(0,m);
					}
				}
			}
			if (count==0)
				cout << "   None." << endl;
		}
		else if ((result) && (team==1))		// hellb successful
		{// damage tower
			cout << "Hellbourne successful." << endl;
			space[hellb_push.at(element).get_x()][hellb_push.at(element).get_y()].take_dam(pusher_no_bonus);
			cout << "The Hellbourne inflicted " << pusher_no_bonus << " damage to the ";
			if ((hellb_push.at(element).get_x()==0) && (hellb_push.at(element).get_y()==0))
				cout << "Legion base ";
			else
				cout << "tower ";
			cout << "at (" << hellb_push.at(element).get_x() << "," << hellb_push.at(element).get_y() << ")." << endl;
			if (space[hellb_push.at(element).get_x()][hellb_push.at(element).get_y()].get_hp()==0)		// If tower died
			{
				cout << "The Hellbourne destroyed ";
				if ((legion_push.at(element).get_x()==3) && (legion_push.at(element).get_y()==3))
				cout << "the Legion base." << endl;
					else
				cout << "a Legion tower." << endl;
				for(n=0;n<3;n++)
					hellb.at(n).add_gold(325);
			}
			else
				cout << "The Legion tower has " << space[hellb_push.at(element).get_x()][hellb_push.at(element).get_y()].get_hp() << "HP remaining." << endl;
		}
		else if	((!result) && (team==0))	// hellb unsuccessful
		{// 25% chance @ death for each hellb hero that participated
			cout << "Hellbourne unsuccessful." << endl;
			cout << "Hellbourne members that died:" << endl;
			for(m=0;m<3;m++)
			{
				if(temp_heroes.at(m)==1)
				{
					fail_b=chance_25_gen();
					if (fail_b)
					{
						cout << "   " << hellb.at(m).get_name() << endl;
						count++;
						hero_death(0,m);
					}
				}
			}
			if (count==0)
				cout << "   None." << endl;
		}
		if (team==0)
			legion_push.erase(legion_push.begin()+element);	// erase executed command
		else
			hellb_push.erase(legion_push.begin()+element);	// erase executed command
	}
}

void farm_fn()
{
	int element=0;
	int num_farmer=0,passes=0;
	int i=0;
	vector<int> temp_heroes;

	while ((legion_farm.size()!=0) || (hellb_farm.size()!=0))
	{
		if (legion_farm.size()!=0)
			element=0;
		else
			element=1;
		i=0;
		passes=0;
		num_farmer=0;
		if (element<=legion_farm.size())	// execute a legion command
		{
			temp_heroes=legion_farm.at(element).get_heroes();
			cout << "Executing farm by Legion member(s)." << endl << endl;
			num_farmer=temp_heroes.at(0)+temp_heroes.at(1)+temp_heroes.at(2);
			while(passes<num_farmer)
			{
				if (temp_heroes.at(i)==1)
				{
					if (passes>0)
					{
						cout << " + " << legion.at(i).get_name();
					}
					else
					{
						cout << legion.at(i).get_name();
					}
					legion.at(i).add_gold(120/num_farmer);
					legion.at(i).add_exp(250/num_farmer);
					passes++;
				}
				i++;
			}
			cout << endl << endl;
			legion_farm.erase(legion_farm.begin()+element);	// erase executed command
		}
		else	// execute a hellb command
		{
			element--;
			temp_heroes=hellb_farm.at(element).get_heroes();
			cout << "Executing farm by Hellbourne member(s)." << endl << endl;
			num_farmer=temp_heroes.at(0)+temp_heroes.at(1)+temp_heroes.at(2);
			while(passes<num_farmer)
			{
				if (temp_heroes.at(i)==1)
				{
					if (passes>0)
					{
						cout << " + " << hellb.at(i).get_name();
					}
					else
					{
						cout << hellb.at(i).get_name();
					}
					hellb.at(i).add_gold(120/num_farmer);
					hellb.at(i).add_exp(250/num_farmer);
					passes++;
				}
				i++;
			}
			cout << endl << endl;
			hellb_farm.erase(hellb_farm.begin()+element);	// erase executed command
		}
	}
}

void move_fn()
{
	bool team_b=true,fail_b=false;
	int team=0,element=0;
	int count=0,passes=0,num_moveer=0;
	int i=0,j=0,k=0,m=0,n=0;
	int failed_move=0;
	vector<int> temp_heroes;


	while ((legion_move.size()!=0) || (hellb_move.size()!=0))
	{
		failed_move=0;
		team_b=chance_50_gen();
		if (legion_move.size()==0)
			team_b=false;
		else if (hellb_move.size()==0)
			team_b=true;
		if (team_b==true)
			element=pick_element(legion_move.size());
		else
			element=pick_element(hellb_move.size());
		passes=0;
		if (team_b)	// execute a legion command
		{
			team=0;
			temp_heroes=legion_move.at(element).get_heroes();
			cout << "Executing move by Legion member(s)." << endl << endl;
			for(i=0;i<3;i++)
			{	
				if (temp_heroes.at(i)==1)
				{
					failed_move=0;
					if (move_loc_valid(legion.at(i).get_x(),legion.at(i).get_y(),legion_move.at(element).get_x(),legion_move.at(element).get_y(),0))
					{
						if (passes>0)
						{
							cout << " + " << legion.at(i).get_name();
						}
						else
						{
							cout << legion.at(i).get_name();
						}
						legion.at(i).set_coord(legion_move.at(element).get_x(),legion_move.at(element).get_y());
						passes++;
					}
					else 
					{
					failed_move=1;
					temp_heroes.at(i)=0;
					}
				}
			}
			if (failed_move==1)
				cout << "Cannot move to selected coordinate because an enemy hero is at the location." << endl;
			else
				cout << " moved to location (" << legion_move.at(element).get_x() << "," << legion_move.at(element).get_y() << ")." << endl;
			cout << endl << endl;
			legion_move.erase(legion_move.begin()+element);	// erase executed command
		}
		else	// execute a hellb command
		{
			team=1;
			temp_heroes=hellb_move.at(element).get_heroes();
			cout << "Executing move by Hellbourne member(s)." << endl << endl;
			for(i=0;i<3;i++)
			{	
				if (temp_heroes.at(i)==1)
				{
					failed_move=0;
					if (move_loc_valid(hellb.at(i).get_x(),hellb.at(i).get_y(),hellb_move.at(element).get_x(),hellb_move.at(element).get_y(),0))
					{
						if (passes>0)
						{
							cout << " + " << hellb.at(i).get_name();
						}
						else
						{
							cout << hellb.at(i).get_name();
						}
						hellb.at(i).set_coord(hellb_move.at(element).get_x(),hellb_move.at(element).get_y());
						passes++;
					}
					else 
					{
						failed_move=1;
						temp_heroes.at(i)=0;
					}
				}
			}
			if (failed_move==1)
				cout << "Cannot move to selected coordinate because an enemy hero is at the location." << endl;
			else
				cout << " moved to location (" << hellb_move.at(element).get_x() << "," << hellb_move.at(element).get_y() << ")." << endl;
			cout << endl << endl;
			hellb_move.erase(hellb_move.begin()+element);	// erase executed command
		}
	}
}

void nothing_fn()
{
	vector<int> temp_heroes;
	int i=0,j=0;

	if (legion_nothing.size()!=0)
	{
		cout << "The following Legion heroes did nothing: " << endl;
		for(i=0;i<legion_nothing.size();i++)
		{	
			temp_heroes=legion_nothing.at(i).get_heroes();
			for(j=0;j<3;j++)
			{
				if (temp_heroes.at(j)==1)
				{
					cout << legion.at(j).get_name() << endl;
				}
			}
		}
		cout << endl;
	}
	if (hellb_nothing.size()!=0)
	{
		cout << "The following Hellbourne heroes did nothing: " << endl;
		for(i=0;i<hellb_nothing.size();i++)
		{	
			temp_heroes=hellb_nothing.at(i).get_heroes();
			for(j=0;j<3;j++)
			{
				if (temp_heroes.at(j)==1)
				{
					cout << hellb.at(j).get_name() << endl;
				}
			}
		}
		cout << endl;
	}
}

bool valid_path(int x_origin,int y_origin,int x_dest,int y_dest)
{
	bool answer=false;
	// 0,0 valid
	if ((x_origin==0) && (y_origin==0))
	{	// valid 0,1 ; 1,1 ; 1;0
		if (((x_dest==0) && (y_dest==1)) || ((x_dest==1) && (y_dest==1)) || ((x_dest==1) && (y_dest==0)))
			answer=true;
	}
	// 1,0 valid
	else if ((x_origin==1) && (y_origin==0))
	{	// valid 0,0 ; 2,0
		if (((x_dest==0) && (y_dest==0)) || ((x_dest==2) && (y_dest==0)))
			answer=true;
	}
	// 2,0 valid
	else if ((x_origin==2) && (y_origin==0))
	{	// valid 1,0 ; 3,0
		if (((x_dest==1) && (y_dest==0)) || ((x_dest==3) && (y_dest==0)))
			answer=true;
	}
	// 3,0 valid
	else if ((x_origin==3) && (y_origin==0))
	{	// valid 2,0 ; 3,1 ; 2,1
		if (((x_dest==2) && (y_dest==0)) || ((x_dest==3) && (y_dest==1)) || ((x_dest==2) && (y_dest==1)))
			answer=true;
	}
	// 0,1 valid
	else if ((x_origin==0) && (y_origin==1))
	{	// valid 0,0 ; 0,2
		if (((x_dest==0) && (y_dest==0)) || ((x_dest==0) && (y_dest==2)))
			answer=true;
	}
	// 1,1 valid
	else if ((x_origin==1) && (y_origin==1))
	{	// valid 0,0 ; 1,2 ; 2,1 ; 2,2
		if (((x_dest==0) && (y_dest==0)) || ((x_dest==1) && (y_dest==2)) || ((x_dest==2) && (y_dest==1)) || ((x_dest==2) && (y_dest==2)))
			answer=true;
	}
	// 2,1 valid
	else if ((x_origin==2) && (y_origin==1))
	{	// valid 1,1 ; 1,2 ; 2,2 ; 3,0
		if (((x_dest==1) && (y_dest==1)) || ((x_dest==1) && (y_dest==2)) || ((x_dest==2) && (y_dest==2)) || ((x_dest==3) && (y_dest==0)))
			answer=true;
	}
	// 3,1 valid
	else if ((x_origin==3) && (y_origin==1))
	{	// valid 3,2 ; 3,0
		if (((x_dest==3) && (y_dest==2)) || ((x_dest==3) && (y_dest==0)))
			answer=true;
	}
	// 0,2 valid
	else if ((x_origin==0) && (y_origin==2))
	{	// valid 0,3 ; 0,1
		if (((x_dest==0) && (y_dest==3)) || ((x_dest==0) && (y_dest==1)))
			answer=true;
	}
	// 1,2 valid
	else if ((x_origin==1) && (y_origin==2))
	{	// valid 0,3 ; 1,1 ; 2,2 ; 2,1
		if (((x_dest==0) && (y_dest==3)) || ((x_dest==1) && (y_dest==1)) || ((x_dest==2) && (y_dest==2)) || ((x_dest==2) && (y_dest==1)))
			answer=true;
	}
	// 2,2 valid
	else if ((x_origin==2) && (y_origin==2))
	{	// valid 1,1 ; 1,2 ; 2,1 ; 3,3
		if (((x_dest==1) && (y_dest==1)) || ((x_dest==1) && (y_dest==2)) || ((x_dest==2) && (y_dest==1)) || ((x_dest==3) && (y_dest==3)))
			answer=true;
	}
	// 3,2 valid
	else if ((x_origin==3) && (y_origin==2))
	{	// valid 3,3 ; 3,1
		if (((x_dest==3) && (y_dest==3)) || ((x_dest==3) && (y_dest==1)))
			answer=true;
	}
	// 0,3 valid
	else if ((x_origin==0) && (y_origin==3))
	{	// valid 0,2 ; 1,2 ; 2,3
		if (((x_dest==0) && (y_dest==2)) || ((x_dest==1) && (y_dest==2))|| ((x_dest==2) && (y_dest==3)))
			answer=true;
	}
	// 1,3 valid
	else if ((x_origin==1) && (y_origin==3))
	{	// valid 0,3 ; 2,3
		if (((x_dest==0) && (y_dest==3)) || ((x_dest==2) && (y_dest==3)))
			answer=true;
	}
	// 2,3 valid
	else if ((x_origin==2) && (y_origin==3))
	{	// valid 0,3 ; 2,3
		if (((x_dest==1) && (y_dest==3)) || ((x_dest==3) && (y_dest==3)))
			answer=true;
	}
	// 3,3 valid
	else if ((x_origin==3) && (y_origin==3))
	{	// valid 2,3 ; 2,2 ; 3,2
		if (((x_dest==2) && (y_dest==3)) || ((x_dest==2) && (y_dest==2))|| ((x_dest==3) && (y_dest==2)))
			answer=true;
	}
	return answer;
}

bool enemy_hero_present(int x_origin,int y_origin,int x_in,int y_in)
{
	bool answer=false;
	int team=1;
	int i=0,j=0;

	for(i=0;i<3;i++)	// See if any legion heroes are at the x_origin,y_origin
	{
		if ((x_origin==legion.at(i).get_x()) && (y_origin==legion.at(i).get_y()))
			team=0;
	}
	if (team==0)	// If hero at origin is legion, look for hellb at x_in,y_in
	{
		for(j=0;j<3;j++)
		{
			if ((x_in==hellb.at(j).get_x()) && (y_in==hellb.at(j).get_y()))
				answer=true;	// if other team has hero at x,y; answer=true
		}
	}
	else			// If hero at origin is hellbourne, look for legion at x_in,y_in
	{
		for(j=0;j<3;j++)
		{
			if ((x_in==legion.at(j).get_x()) && (y_in==legion.at(j).get_y()))
				answer=true;	// if other team has hero at x,y; answer=true
		}
	}

	return answer;
}

bool tower_present(int x_in,int y_in)
{
	bool answer=false;

	if (space[x_in][y_in].get_hp()>0)	// Check of tower is alive at x_in,y_in
		answer=true;

	return answer;
}

bool base_vulnerable(int team)
{
	bool answer=false;

	if (team==0)	// Check if legion base can be attacked (one of surrounding towers is dead)
	{
		// check if any tower at following locations are dead: 0,1 ; 1,1 ; 1,0
		if ((space[0][1].get_hp()==0) || (space[1][1].get_hp()==0) || (space[1][0].get_hp()==0))
			answer=true;
	}
	else			// Check if hellbourne base can be attacked (one of surrounding towers is dead)
	{
		// check if any tower at following locations are dead: 2,3 ; 2,2 ; or 3,2
		if ((space[2][3].get_hp()==0) || (space[2][2].get_hp()==0) || (space[3][2].get_hp()==0))
			answer=true;
	}
	
	return answer;
}

bool enemy_tower_present(int x_origin,int y_origin,int x_in,int y_in)
{
	bool answer=false;
	int i;
	int team=1;

	for(i=0;i<3;i++) 	// See if any legion heroes are at the x_origin,y_origin
	{
		if ((x_origin==legion.at(i).get_x()) && (y_origin==legion.at(i).get_y()))
			team=0;
	}

	if (team==0)	// If hero at origin is legion
	{ // compare x_in,y_in to hellb tower locations: 1,3 ; 2,3 ; 2,2 ; 3,3 ; 3,2 ; 3,1
		if (((x_in==1) && (y_in==3)) || ((x_in==2) && (y_in==3)) || ((x_in==2) && (y_in==2)) || ((x_in==3) && (y_in==3)) || ((x_in==3) && (y_in==2)) || ((x_in==3) && (y_in==1)))
			answer=true;
	}
	else			// If hero at origin is hellbourne, look for legion at x_in,y_in
	{// compare x_in,y_in to legion tower locations: 0,2 ; 0,1 ; 0,0 ; 1,1 ; 1,0 ; 2,0
		if (((x_in==0) && (y_in==2)) || ((x_in==0) && (y_in==1)) || ((x_in==0) && (y_in==0)) || ((x_in==1) && (y_in==1)) || ((x_in==1) && (y_in==0)) || ((x_in==2) && (y_in==0)))
			answer=true;
	}
	
	return answer;
}

bool chance_50_gen()
{
	bool answer;
	int value;

	value=rand()%2;
	if (value==1)
		answer=true;
	else if (value==0)
		answer=false;

	return answer;
}

bool chance_25_gen()
{
	bool answer;
	int value;

	value=rand()%4;
	if (value==1)
		answer=true;
	else
		answer=false;

	return answer;
}

bool outcome_gen(int a,int b)
{
	int result=0,c=0;
	bool answer=false;

	if (a>(8*b))
		answer=true;
	else if (b>(8*a))
		answer=false;
	else
	{
		c=a+b;				// example: a=12,b=24 -> c=36
		result=rand()%c+1;	// c= betwen 1 and 36
		if (result<=a)
			answer=true;
		else
			answer=false;
	}

	return answer;
}

int pick_element(int elements)
{
	int answer=0;

	answer=rand()%elements;
	
	return answer;
}

void wait(int seconds)
{
	clock_t endwait;
	endwait = clock () + seconds * CLOCKS_PER_SEC ;
	while (clock() < endwait);
}

void pick_countdown()
{
	cout << "Game Starting:" << endl;
	cout << "5..." << endl;
	wait(1);
	cout << "4..." << endl;
	wait(1);
	cout << "3..." << endl;
	wait(1);
	cout << "2..." << endl;
	wait(1);
	cout << "1..." << endl;
	wait(1);
	cout << endl << endl;
}

void game_countdown()
{
	cout << "5..." << endl;
	wait(1);
	cout << "4..." << endl;
	wait(1);
	cout << "3..." << endl;
	wait(1);
	cout << "2..." << endl;
	wait(1);
	cout << "1..." << endl;
	wait(1);
	cout << endl << endl << "Lets get it on!" << endl << endl;
	wait (2);
}

void winner_legion()
{
	int i;
	system("cls");
	for(i=0;i<40;i++)
		cout << endl;
	cout << "                                  Legion Wins.";
	for(i=0;i<38;i++)
		cout << endl;
}

void winner_hellb()
{
	int i;
	system("cls");
	for(i=0;i<40;i++)
		cout << endl;
	cout << "                               Hellbourne Wins.";
	for(i=0;i<38;i++)
		cout << endl;
}

void print_map()
{
	cout << "E -T3-T2-HB" << endl;
	cout << "| \\     / |" << endl;
	cout << "T3 E -T4 T2" << endl;
	cout << "|  | X |  |" << endl;
	cout << "T2 T4- E T3" << endl;
	cout << "| /     \\ |" << endl;
	cout << "LB-T2-T3- E" << endl;
}

void hero_death(int team,int element)
{
	int i;
	int respawn=0;
	// legion_gank,legion_push,legion_farm,legion_move,legion_nothing
	// hellb_gank,hellb_push,hellb_farm,hellb_move,hellb_nothing;


	if (team==0)	// kill legion
	{
		legion.at(element).set_alive(0);				// set alive=0 (dead)
		respawn=2+(1/2)*legion.at(element).get_level();	// set respawn=2+(1/2)*level (truncated)
		legion.at(element).set_respawn(respawn);		// "
		legion.at(element).set_coord(-1,-1);			// remove from map
		legion.at(element).rem_gold(200);				// lose 200 gold

		for(i=0;i<3;i++)	// clear hero's commands
		{
			if (legion_gank.size()>i)
				legion_gank.at(i).change_heroes(element,0);
			if (legion_push.size()>i)
				legion_push.at(i).change_heroes(element,0);
			if (legion_farm.size()>i)
				legion_farm.at(i).change_heroes(element,0);
			if (legion_move.size()>i)
				legion_move.at(i).change_heroes(element,0);
			if (legion_nothing.size()>i)
				legion_nothing.at(i).change_heroes(element,0);
		}
	}
	else			// kill hellbourne
	{
		hellb.at(element).set_alive(0);					// set alive=0 (dead)
		respawn=1+(1/2)*hellb.at(element).get_level();	// set respawn=1+(1/2)*level (truncated)
		hellb.at(element).set_respawn(respawn);			// "
		hellb.at(element).set_coord(-1,-1);				// remove from map
		hellb.at(element).rem_gold(200);				// lose 200 gold

		for(i=0;i<3;i++)	// clear hero's commands
		{
			if (hellb_gank.size()>i)
				hellb_gank.at(i).change_heroes(element,0);
			if (hellb_push.size()>i)
				hellb_push.at(i).change_heroes(element,0);
			if (hellb_farm.size()>i)
				hellb_farm.at(i).change_heroes(element,0);
			if (hellb_move.size()>i)
				hellb_move.at(i).change_heroes(element,0);;
			if (hellb_nothing.size()>i)
				hellb_nothing.at(i).change_heroes(element,0);
		}
	}
}

void disp_commands(int team,int element)
{
	int option=1;

	if ((team==0) && (legion.at(element).get_alive()==1))
	{
		cout << "What do you want " << legion.at(element).get_name() << " to do?" << endl;
		cout << "Current location: (" << legion.at(element).get_x() << "," << legion.at(element).get_y() << ")." << endl;
		if (((legion.at(element).get_x()==0) && (legion.at(element).get_y()==0)) || ((legion.at(element).get_x()==3) && (legion.at(element).get_y()==3)))
		{
			cout << option << ": Access the store." << endl; option++;
		}
		cout << option << ": Gank." << endl; option++;
		cout << option << ": Push." << endl; option++;
		cout << option << ": Farm." << endl; option++;
		cout << option << ": Move." << endl; option++;
		cout << option << ": Nothing." << endl; option++;
		cout << option << ": Who is." << endl; option++;
		cout << option << ": Print Map." << endl; option++;
		cout << option << ": Check Tower." << endl;
	}
	else if ((team==1) && (hellb.at(element).get_alive()==1))
	{
		cout << "What do you want " << hellb.at(element).get_name() << " to do?" << endl;
		cout << "Current location: (" << hellb.at(element).get_x() << "," << hellb.at(element).get_y() << ")." << endl;
		if (((hellb.at(element).get_x()==0) && (hellb.at(element).get_y()==0)) || ((hellb.at(element).get_x()==3) && (hellb.at(element).get_y()==3)))
		{
			cout << option << ": Access the store." << endl; option++;
		}
		cout << option << ": Gank." << endl; option++;
		cout << option << ": Push." << endl; option++;
		cout << option << ": Farm." << endl; option++;
		cout << option << ": Move." << endl; option++;
		cout << option << ": Nothing." << endl; option++;
		cout << option << ": Who is." << endl; option++;
		cout << option << ": Print Map." << endl; option++;
		cout << option << ": Check Tower." << endl;
	}
}

void disp_teams()
{
	cout << "Legion Team:" << endl;
	cout << legion.at(0).get_name() << ", " << legion.at(1).get_name()  << ", " << legion.at(2).get_name() << endl << endl;
	cout << "Hellbourne Team:" << endl;
	cout << hellb.at(0).get_name() << ", " << hellb.at(1).get_name()  << ", " << hellb.at(2).get_name() << endl << endl;

}

void clear_commands()
{
	legion_gank.clear();
	legion_push.clear();
	legion_farm.clear();
	legion_move.clear();
	legion_nothing.clear();
	hellb_gank.clear();
	hellb_push.clear();
	hellb_farm.clear();
	hellb_move.clear();
	hellb_nothing.clear();
}

void respawn_handler()
{
	int i=0;
	int respawn_new=0;

	for(i=0;i<3;i++)
	{
		if (legion.at(i).get_alive()==0)
		{
			respawn_new=legion.at(i).get_respawn();
			respawn_new--;
			if (respawn_new==0)
			{
				legion.at(i).set_alive(1);
				legion.at(i).set_coord(0,0);
				respawn_new=0;
			}
			legion.at(i).set_respawn(respawn_new);
		}
		if (hellb.at(i).get_alive()==0)
		{
			respawn_new=hellb.at(i).get_respawn();
			respawn_new--;
			if (respawn_new==0)
			{
				hellb.at(i).set_alive(1);
				hellb.at(i).set_coord(3,3);
				respawn_new=0;
			}
			hellb.at(i).set_respawn(respawn_new);
		}
	}
}

void wait_for_enter(int skipnew)
{
	string waitstring;

	if (skipnew==1)
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin,waitstring);
	cin.clear();
}