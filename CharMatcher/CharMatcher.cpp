/** 
 * @author: Carter Frost 
 * Email: CharMatcher@carterfrost.com
 * Filename: CharMatcher.cpp
 * Description: C++ terminal based bejewled like game
 * Code status: Incomplete
 * Program compile: Yes, should use either c++11 or gnu++11
 * Errors or warnings: None
 * Comments: 
 * TODO: No more moves, score prompt, swap if match make a x but dont consider the x's to be a match
 */
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h> // for rand
#include <iomanip> // std::setfill, std::setw
#include <ctype.h> //isalpha toupper
#include <sstream>
#include <cmath> // std::abs
using namespace std;

struct pos {
	unsigned x;
	unsigned y;
	void print(){ cout << "x:"<< x << ", y:" << y << endl; }
};

struct pos_pair {
	pos p1;
	pos p2;
};

struct pos_node{
	pos p;
	pos* next = NULL;
	pos* prev = NULL;
	unsigned count = 0;
};

class CharMatcher{
	private:
		int width;
		int height;
		int num_of_symbols;
		int max_num_of_symbols;
		int score;
		bool debug = true;
		//TODO: try uint32_t for multi-character characters, wchar_t seems to not work
		string symbols[11] = {"x","■", "◆","▲","●","▮","◢","▰","◣","◤","◥"}; 
		string colors[8] = {"","\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m", "\033[1;37m"};
		string reset_color = "\033[0m";
		int** board; //char board*[]; //array of linklists? of structs, power jems? Color?
		bool legal_move(pos p1, pos p2){
			bool legal = false;
			if ((p1.x < width) && (p2.x < width) //bounds
				&& ((p1.y < height) && (p2.y < height)) //bounds
				&& ((abs(p1.x - p2.x) == 1) && ((p1.y - p2.y) == 0)) //hozitonal distance 1
					|| ((abs(p1.y - p2.y) == 1) && ((p1.x - p2.x) == 0) ) //or vertical distance 1
				){
				swap(p1,p2); 
				legal = any_matches();
				swap(p1,p2); //put it back
			}
			return(legal);
		} //end legal_move
		
		void clear_screen(){
			// ANSI escape code to clear the screen and to reset the cursor
			cout << "\033[2J\033[1;1H"; 
		} //end clear_screen
		string get_shape_str(int symbol_numb){ 
			if ( symbol_numb <= max_num_of_symbols) {
				return(colors[symbol_numb] + symbols[symbol_numb] + reset_color);} 
			//cerr << "symbol_numb:" << symbol_numb << ";";
			return("n");
		}
		
		//For populating the top of the board
		//string new_char(){return(chars[0]);}
	public:
		CharMatcher() : CharMatcher(15,10,7){}
		CharMatcher(int new_height, int new_width, int new_num_of_symbols){
			this->score =0;
			this->max_num_of_symbols = 11;
			this->height=new_height;
			this->width=new_width;
			this->num_of_symbols=new_num_of_symbols;
			this->board = new int*[width];
			for(int x = 0; x < width; x++) { this->board[x] = new int[height]; }
			generate_board();
		} //end CharMatcher(int new_height, int new_width, int new_num_of_symbols)
		~CharMatcher(){ delete[] board; }
		
		/*
		   A  B  C  D  E  F  G  H  I  J
		00 ◆  ▮  ◆  ●  ▲  ●  ▮  ■  ■  ▮  
		01 ◢  ▲  ◢  ◢  ▲  ▮  ■  ■  ▲  ◢  
		02 ▮  ●  ●  ▲  ●  ◢  ■  ◆  ◢  ◢  
		03 ▲  ■  ▮  ◆  ▮  ▮  ◢  ▲  ▲  ●  
		04 ■  ▲  ◢  ▮  ◢  ◢  ▲  ◢  ▮  ◢  
		05 ▲  ■  ■  ▮  ▲  ●  ◆  ■  ▲  ■  
		06 ●  ▮  ◆  ◢  ▮  ●  ▮  ●  ◢  ■  
		07 ▮  ▮  ■  ▲  ■  ◢  ◆  ▲  ◢  ●  
		08 ◢  ◆  ●  ◢  ●  ●  ■  ▮  ●  ●  
		09 ◢  ◆  ◢  ■  ■  ●  ●  ▲  ▮  ■  
		10 ●  ●  ▲  ●  ●  ▲  ●  ▮  ●  ■  
		11 ■  ■  ◢  ●  ■  ●  ◆  ▮  ◆  ▮  
		12 ◆  ▮  ●  ◢  ▮  ▲  ▲  ◢  ▮  ◢  
		13 ▮  ◆  ■  ▮  ●  ●  ◢  ▮  ◢  ▲  
		14 ▮  ◢  ▲  ◆  ●  ■  ▮  ▲  ◢  ▮  */
		void draw_board(){
			cout << endl <<" ";
			for (int x = 0; x < width; x++){ cout << "  " << (char)(x+'A'); }
			cout << endl;
			for (int y = 0; y < height; y++){ 
				//setfill("0")
				cout << setw(2) << setfill('0') << y;
				cout << " ";
				for (int x = 0; x < width; x++){
					cout << get_shape_str(board[x][y]) << "  "; 
				}//end for printing each board peace on the row
				cout << setw(2) << setfill('0') << y << endl;
			} //end for heights
		cout <<" ";
		for (int x = 0; x < width; x++){ cout << "  " << (char)(x+'A'); }
		cout << endl;
		} //end draw_board
		
		void generate_board(){
			do {
				for (int y = 0; y < this->height; y++){
					for (int x = 0; x < this->width; x++){
						board[x][y]=(rand()%(this->num_of_symbols-1))+1;
					} //end inner for x's
				} //end outer for y's
			} while (any_matches()); // ensure there are no mathes on the new board
		} //end generate_board
		
		//pos board_has_match() {pos poz; return(poz);}
		bool is_same_symbol(int a, int b, int c){
			return((a == b) && (b == c));
		}//end is_same_symbol

		//board_has_match().width == -1
		bool any_matches(){
			//horizontal
			for(int y =0; y < height;y++){
				for(int x =0; x < width - 2;x++){
					if (is_same_symbol(	board[x][y], 
										board[x+1][y], 
										board[x+2][y])) { 
				return(true);
			}}} //end for horizontal matchs
			
			//vertical
			for(int x =0; x < width;x++){
				for(int y =0; y < height-2;y++){
					if (is_same_symbol(	board[x][y], 
										board[x][y+1], 
										board[x][y+2])) { 
					return(true);}}}
			return(false); 
		}
		
		bool no_more_moves(){return(false);}
		
		//TODO: implement a que for items, clear or replace both swaped then fall each row after finished next
		unsigned update_board(){
		
			return(false);} 
		
		//TODO: create matches to matches and it returns a pos struct of the largest one
		pos_node get_biggest_match(){
			vector<pos_node> vertical;
			//vertical
			//horizontal
			//pos matches[] ??
		} //end get_biggest_match
		
		//return score 0 if nothing to match
		unsigned swap(pos p1, pos p2){
			unsigned moves_score = 0;
			// 	TODO: check distance, currently can swich any on the board
			// TODO: check bounds can swap garbage
			int temp = board[p1.x][p1.y];
			board[p1.x][p1.y] = board[p2.x][p2.y];
			board[p2.x][p2.y] = temp;
			draw_board();
			update_board();
			if (any_matches()){ // change to while //move to update board
				get_biggest_match();
			//remove biggest match
			cout << "nice" << endl; }
			//time delay
			//if no match then put it back and
			return(moves_score);
		}
		//validate move
		string move(string move){
			if(move.size() == 0) {return("NONE");}
			pos sym_pos1;
			pos sym_pos2;
			size_t space1 = move.find(" ");
			size_t space2 = move.rfind(" ");
			if ((space1 == string::npos) || (space2 == string::npos)){
				return("No space");
			} //end if cant find first or second space
			
			string token1 = move.substr(0,space1);
			string token2 = move.substr(space1+1,space2-space1);
			string token3 = move.substr(space2+1);
			bool d1 = isdigit(token1[0]);
			bool d2 = isdigit(token2[0]);
			bool d3 = isdigit(token3[0]);
			//Vertal Swap
			if ((d1 && d2 && !d3) || (d1 && !d2 && d3) || (!d1 && d2 && d3)){
				if (debug){cout << "Horz swap";}
				if(!d3){ //if token3 is a letter
					sym_pos1.x = toupper(token3[0]) - 'A';
					sym_pos1.y = stoi(token1);
					sym_pos2.x = toupper(token3[0]) - 'A';
					sym_pos2.y = stoi(token2);
				} //end if token3 is a letter
				else if(!d2) { //if token2 is a letter
					sym_pos1.x = toupper(token2[0]) - 'A';
					sym_pos1.y = stoi(token1);
					sym_pos2.x = toupper(token2[0]) - 'A';
					sym_pos2.y = stoi(token3);
				} //end if token2 is a letter
				else { //if token1 is a letter
					sym_pos1.x = toupper(token1[0]) - 'A';
					sym_pos1.y = stoi(token2);
					sym_pos2.x = toupper(token1[0]) - 'A';
					sym_pos2.y = stoi(token3);
				} // end else (if token1 is a letter)
			//Horizontal Swap
			} else if ((d1 && !d2 && !d3) 
					|| (!d1 && d2 && !d3) 
					|| (!d1 && !d2 && d3)){
				if (debug){cout << "Vert Swap";}
				if(d1){ //if token1 is a digit
					sym_pos1.x = toupper(token2[0]) - 'A';
					sym_pos1.y = stoi(token1);
					sym_pos2.x = toupper(token3[0]) - 'A';
					sym_pos2.y = stoi(token1);
				} //end if token1 is a digit
				else if(d2){ //if token2 is a digit
					sym_pos1.x = toupper(token1[0]) - 'A';
					sym_pos1.y = stoi(token2);
					sym_pos2.x = toupper(token3[0]) - 'A';
					sym_pos2.y = stoi(token2);
				} //end //if token2 is a digit
				else{ //if token3 is a digit
					sym_pos1.x = toupper(token1[0]) - 'A';
					sym_pos1.y = stoi(token3);
					sym_pos2.x = toupper(token2[0]) - 'A';
					sym_pos2.y = stoi(token3);
				} //end if token3 is a digit
			 } else {cout << "What?"; return("No");}
			 //f 8 g not working
			if (debug){
				cout << endl << "sym_pos1:"; sym_pos1.print();
				cout << "sym_pos2:"; sym_pos2.print();
			}
			if (legal_move(sym_pos1,sym_pos2)){
				int moves_score = swap(sym_pos1,sym_pos2);
				////symbols[0]; // x out the matches
				//do it (Swap)
				//returned string is feed back like invlid move, good, sweet, exilent, extraorinary e1337, I cant beleve its not butter
				return("Legal");
			} //end if legal_move
			
			//now check the move
		return("invalid");
		} //end move
		
		int from_letter(string s){return(0);}
		string to_letter(int i){return("");}
		
		void consoleUI(){
			string input ="";
			string status ="";
			do {
				clear_screen();
				status = move(input);
				draw_board();
				cout.width(8);
				cout << right << "\tScore: " << score << endl;
				cout << "Enter moves like \"a b 12\" or \"11 a 12\"" << endl;
				cout <<"Last Move:"<< input << " was "<< status <<endl << "Next:";
			}
			while (!no_more_moves() && getline(cin,input));
		} //end consoleUI
}; //end class

	/**
	 * main
	 * Creates an instance of the CharMatcher class and runs the consoleUI
	 * @params Ignored
	 * @returns 0 compleated sucessfully
	 */	
int main(int argc, char *argv[]) {
	srand(time(0));
	cout << "Greetings!";
	//TODO: prompt for size?
	CharMatcher char_mather;// = new CharMatcher();
	char_mather.consoleUI();
	cout << "Qapla'!" <<endl;
	return(0); 
} //end main

