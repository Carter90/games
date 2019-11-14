/** 
 * Name: Carter Frost 
 * Email: CharMatcher@carterfrost.com
 * Filename: CharMatcher.h
 * Description: header file for CharMatcher
 * Code status: Incomplete
 * Program compile:  Yes
 * Errors or warnings: None Known
 */
#include <string>
#include <iostream>
#include <vector>
#ifndef CHARMATCHER_H
#define CHARMATCHER_H


//get the distance between x & y
unsigned distance(unsigned x,unsigned y);//{return(((x > y) ? (x-y) : (y-x)));}

struct pos {
	unsigned x;
	unsigned y;
	pos(){;}
	pos(unsigned nx, unsigned ny){this->x = nx; this->y = ny;}
	void print(){ std::cout << "x:"<< x << ", y:" << y << std::endl; }
	bool pos_dist_is_one(pos p2,int width,int height){
		return((this->x < width) && (p2.x < width) //bounds
			&& ((this->y < height) && (p2.y < height)) //bounds
			&& (((distance(this->x, p2.x) == 1) && ((this->y - p2.y) == 0)) //hozitonal distance 1
			|| ((distance(this->y, p2.y) == 1) && ((this->x - p2.x) == 0) )) //or vertical distance 1
			); 
	} // end pos_dist_is_one
};


struct pos_node{
	pos p;
	pos_node* right = NULL;
	pos_node* up = NULL;
	pos_node* down = NULL;
	unsigned count_right = 0;
	unsigned count_vert = 0;
	bool from_above = false;
	bool from_below= false;
	pos_node(){;}
	pos_node(pos np, bool above=false, bool below=false){
	    this->p = np;
	    this->count_right = 0;
	    this->count_vert = 0;
	    this->from_above = above;
		this->from_below = below;
	}
};

class CharMatcher {
	public:
		CharMatcher() : CharMatcher(15,10,7){};
		CharMatcher(int new_height, int new_width, int new_num_of_symbols);
		~CharMatcher();
        int move(std::string move);
		void consoleUI();
		void draw_board();
		void generate_board();

	private:
		int width;
		int height;
		int num_of_symbols;
		int max_num_of_symbols;
		int score;
		bool debug = false;
		//TODO: try uint32_t for multi-character characters, wchar_t seems to not work
		std::string symbols[11] = {"x","■", "◆","▲","●","▮","◢","▰","◣","◤","◥"}; 
		std::string colors[8] = {"","\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m", "\033[1;37m"};
		std::string reset_color = "\033[0m";
		int** board; //char board*[]; //array of linklists? of structs, power jems? Color?
		bool legal_move(pos p1, pos p2);
		static void clear_screen();
        std::string get_shape_str(int symbol_numb);
		bool is_same_symbol(int a, int b, int c) const;
		bool is_same_symbol(pos p1, pos p2) const;
		bool any_matches() const;
		pos_node* populate_pos_node(pos p,bool=false,bool=false);
		pos_node* get_biggest_match();
		void swap(pos p1, pos p2);
		std::vector<pos> get_three_matches_pos();
        unsigned x_all_nodes(pos_node* pn);
        void shiftdown();
        bool board_has_x();

};
#endif

