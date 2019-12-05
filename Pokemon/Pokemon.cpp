/** 
 * @author: Carter Frost 
 * Email: Pokemon@carterfrost.com
 * Filename: Pokemon.cpp
 * Description: C++ terminal based pokemon
 * Code status: Incomplete
 * Program compile: Yes, should use either c++11 or gnu++11
 * Errors or warnings: None
 * Comments: Most of this code was written as examples for writing/using a class
 * for an introduction c++ course.
 * TODO: Use types to calculate damage scaler https://pokemondb.net/type array of arrays
 * TODO: Move vs special move selections?
 * TODO: Create menu for searching and selecting pokemon
 * TODO: Spit into header, pokemon cpp and main then update cmake
 * TODO: Function documentation!!!!!!
 */

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib> // atoi
#include <sstream> // istringstream
//#include <random> // srand & rand

using namespace std;

class Pokemon{
public: 
	Pokemon();
	Pokemon(string name, int hp, unsigned attack, unsigned defense, unsigned speed);
	Pokemon(unsigned n_num, string n_name, string n_type1, string n_type2, unsigned n_total, int n_hp, unsigned n_attack, unsigned n_defense, unsigned n_sattack, unsigned n_sdefense, unsigned n_speed, unsigned n_generation, bool n_legendary);
	void print() const;
		double get_speed() const{ return(speed);}
		int get_hp(){return(hp);}
		unsigned get_attack(){return(attack);}
		string get_name(){return(name);}
		void set_hp(int new_hp){hp = new_hp;}
		bool battle(Pokemon other);
	private: 
		unsigned num = 0;
		string name = "";
		string type1 = ""; //TODO: Create struct for types
		string type2= "";
		unsigned total = 0;
		int hp = 0;
		unsigned attack = 0;
		unsigned defense = 0;
		unsigned sattack = 0;
		unsigned sdefense = 0;
		unsigned speed = 0;
		unsigned generation = 0;
		bool legendary = false;
};

void Pokemon::print() const{
    //TODO: Finish this with all the variables and their labels;
    cout << name << " hp:" << hp << endl;
}

Pokemon::Pokemon(){name = "none";}
Pokemon::Pokemon(string n_name, int n_hp, unsigned n_attack, unsigned n_defense, unsigned n_speed){
	name = move(n_name); //std's move
	hp = n_hp;
	attack = n_attack;
	defense = n_defense; 
	speed = n_speed;
} //end Pokemon constructor


Pokemon::Pokemon(unsigned n_num, string n_name, string n_type1, string n_type2, unsigned n_total, int n_hp, unsigned n_attack, unsigned n_defense, unsigned n_sattack, unsigned n_sdefense, unsigned n_speed, unsigned n_generation, bool n_legendary){
	num= n_num; 
	name= std::move(n_name);
	type1= std::move(n_type1);
	type2= std::move(n_type2);
	total= n_total;
	hp= n_hp;
	attack= n_attack;
	defense= n_defense;
	sattack= n_sattack;
	sdefense= n_sdefense;
	speed= n_speed;
	generation= n_generation;
	legendary= n_legendary;
} // end Pokemon full overloaded constructor

//battle returns true this Pokemon won, false if lost
bool Pokemon::battle(Pokemon other){
    const int effectiveness = 3;  //1 for supper effective, based on type
    const int others_effectiveness = 3; //1 for supper effective ^^
    // TODO: change effectiveness by type and store type in 2d array
	bool my_turn = false;
	if(speed > other.get_speed()){ my_turn = true;}
	do {
		if(my_turn) { other.set_hp(other.get_hp() - (int)attack/effectiveness);
		    //TODO: add colors to names based on type then reset
			cout << name << " attacks " << other.get_name() <<endl;
			cout << other.get_name() << "'s HP:" << other.get_hp() <<endl;
		} //end my turn
		else { hp = hp - (int)other.get_attack()/others_effectiveness;
			cout << other.get_name() << " attacks " << name<<endl;
			cout << name << "'s HP:" << hp <<endl;
		} //others turn
		my_turn = !my_turn;
	}while( hp > 0 && other.get_hp() > 0);
	return(hp > 0);
} //end Pokemon::battle

void readfile(ifstream &poke_file, vector<Pokemon> &pokedex){ // ,
	string spokemon;
	string data;
	unsigned num_new;
	string name_new;
	string type1_new;
	string type2_new;
	unsigned total_new;
	int hp_new;
	unsigned attack_new;
	unsigned defense_new;
	unsigned sattack_new;
	unsigned sdefense_new;
	unsigned speed_new;
	unsigned generation_new;
	bool legendary_new;
	while(getline(poke_file, spokemon)){
		istringstream is(spokemon);
		getline(is, data,','); num_new = stoi(data);
		getline(is, name_new,',');
		getline(is, type1_new,',');
		getline(is, type2_new,',');
		getline(is, data,','); total_new = stoi(data);
		getline(is, data,','); hp_new = stoi(data);
		getline(is, data,','); attack_new = stoi(data);
		getline(is, data,','); defense_new = stoi(data);
		getline(is, data,','); total_new = stoi(data);
		pokedex.emplace_back(num_new, name_new, type1_new, type2_new, total_new, hp_new, attack_new, defense_new, sattack_new, sdefense_new, speed_new, generation_new, legendary_new);
	} //end while file has more pokemon
} // end readfile

int main() {
    srand(time(0));
	vector<Pokemon> pokedex;
	ifstream poke_file("newpokemon.csv");
    if (poke_file.fail()){ cerr << "Check that file" << endl; return(-1);}
	string line; 
	getline(poke_file,line); // throw away header
	readfile(poke_file, pokedex); poke_file.close();
    Pokemon pokemon1 = pokedex.at(rand() % pokedex.size());
    Pokemon pokemon2 = pokedex.at(rand() % pokedex.size());
    pokemon1.print();
    pokemon2.print();
	if (pokemon1.battle(pokemon2)) {cout << pokemon1.get_name() << " wins!";}
	else {cout << pokemon2.get_name() << " wins!";}
} //end main
