/** 
 * @author: Carter Frost 
 * Email: CharMatcher@carterfrost.com
 * Filename: CharMatcher.cpp
 * Description: C++ terminal based bejewled like game
 * Code status: Incomplete
 * Program compile: Yes, should use either c++11 or gnu++11
 * Errors or warnings: None
 * Bugs: Unsigned issue for calcuclating y postions 4294967295 when running '1 b a' remove seeds to get this
 * Bugs: Node adjestnys can go down down up problem is with directional booleans
 * Comments: 
 * TODO: No more moves, score prompt, swap if match make a x but dont consider the x's to be a match
 */
#include <iostream>
#include <string>
#include <stdlib.h> // for rand
#include <iomanip> // std::setfill, std::setw
#include <ctype.h> //isalpha toupper
#include <unistd.h> //usleep for drawboard delays
#include "CharMatcher.h"
using namespace std;

//get the distance between x & y
unsigned distance(unsigned x,unsigned y){return(((x > y) ? (x-y) : (y-x)));}

bool CharMatcher::legal_move(pos p1, pos p2){
	bool legal = false;
	if (p1.pos_dist_is_one(p2, width, height)){
		swap(p1, p2);
		legal = any_matches();
		swap(p1, p2); //put it back
	} // end if distance between poses is 1

	return(legal);
} //end legal_move

void CharMatcher::clear_screen(){
	// ANSI escape code to clear the screen and to reset the cursor
	cout << "\033[2J\033[1;1H"; 
} //end clear_screen

string CharMatcher::get_shape_str(int symbol_numb){
	if ( symbol_numb <= max_num_of_symbols) {
		return(colors[symbol_numb] + symbols[symbol_numb] + reset_color);} 
	//cerr << "symbol_numb:" << symbol_numb << ";";
	return("n");
} //end get_shape_str

//For populating the top of the board
//string new_char(){return(chars[0]);}

CharMatcher::CharMatcher(int new_height, int new_width, int new_num_of_symbols){
	this->score = 0;
	this->max_num_of_symbols = 11;
	this->height=new_height;
	this->width=new_width;
	this->num_of_symbols=new_num_of_symbols;
	this->board = new int*[width];
	for(int x = 0; x < width; x++) { this->board[x] = new int[height]; }
	generate_board();
} //end CharMatcher(int new_height, int new_width, int new_num_of_symbols)
CharMatcher::~CharMatcher(){ delete[] board; }

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
void CharMatcher::draw_board(){
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
cout << " ";
for (int x = 0; x < width; x++){ cout << "  " << (char)(x+'A'); }
cout << endl;
} //end draw_board

void CharMatcher::generate_board(){
	do {
		for (int y = 0; y < this->height; y++){
			for (int x = 0; x < this->width; x++){
				board[x][y]=(rand()%(this->num_of_symbols-1))+1;
			} //end inner for x's
		} //end outer for y's
	} while (any_matches()); // ensure there are no matches on the new board
} //end generate_board

//pos board_has_match() {pos poz; return(poz);} and is not not and 'x'
 bool CharMatcher::is_same_symbol(int a, int b, int c) const{
	return((a == b) && (b == c) && ((a != 0)) && (b != 0) && (c != 0));
}//end is_same_symbol

bool CharMatcher::is_same_symbol(pos p1, pos p2) const{
	return (board[p1.x][p1.y]==board[p2.x][p2.y])
		&& ((board[p1.x][p1.y]!= 0)
		&& (board[p2.x][p2.y] != 0));
} // end is_same_symbol 2 pos args

//board_has_match().width == -1
bool CharMatcher::any_matches() const{
	//horizontal
	for(int y =0; y < height;y++){
		for(int x =0; x < width - 2;x++){
			if (is_same_symbol(	board[x][y], 
								board[x+1][y], 
								board[x+2][y])) { 
		return(true);
	}}} //end for horizontal matches
	
	//vertical
	for(int x =0; x < width;x++){
		for(int y =0; y < height-2;y++){
			if (is_same_symbol(	board[x][y], 
								board[x][y+1], 
								board[x][y+2])) { 
			return(true);}}}
	return(false); 
}

//TODO: Implment see if any move can be made to continue matching
bool no_more_moves(){return(false);}


/* NOTES: 
struct pos_node{
pos p;
pos* right = NULL;
pos* up = NULL;
pos* down = NULL;
unsigned count = 0;
}; */

vector<pos> CharMatcher::get_three_matches_pos() {
	vector<pos> three_matches;
	//horizontal
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width - 2; x++) {
			if (is_same_symbol(board[x][y],
							   board[x + 1][y],
							   board[x + 2][y])) {
				if (debug){
				    cout << "get_three_matches_pos: horizontal:";
				    pos(x,y).print(); }
				three_matches.push_back(pos(x,y));
			} //end for height
		} //end for width
	} //end for horizontal matches

	//vertical
	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height - 2; y++) {
			if (is_same_symbol(board[x][y],
							   board[x][y + 1],
							   board[x][y + 2])) {
				if (debug){
				    cout << "get_three_matches_pos: vertical:";
				    pos(x,y).print(); }
				three_matches.push_back(pos(x,y));
			}
		}
	}
	return three_matches;
} //end get_three_matches_pos

// TODO: fix bug of going down, down,down and then up and  up on initial "5 6 a"
pos_node* CharMatcher::populate_pos_node(pos p, bool from_above, bool from_below){
	pos_node* pn = new pos_node(p,from_above,from_below);

	//right
	if ((p.pos_dist_is_one(pos(p.x+1,p.y), width, height))
		&& is_same_symbol(p, pos(p.x+1,p.y))){
	    if (debug){cout << "populate_pos_node right" << endl;}
		pn->right = populate_pos_node(pos(p.x+1,p.y));
		pn->count_right += pn->right->count_right + 1;
	} //end if

	//down
	if ((p.pos_dist_is_one(pos(p.x,p.y+1), width, height))
		&& is_same_symbol(p, pos(p.x,p.y+1))
		&& !from_below){
        if (debug){cout << "populate_pos_node down" << endl;}
		pn->down = populate_pos_node(pos(p.x,p.y+1),true,false);
		if(pn->down){pn->count_vert += pn->down->count_vert + 1;}
	} //end if down
/*
	//up
	if ((p.pos_dist_is_one(pos(p.x,p.y+1), width, height))
		&& is_same_symbol(p, pos(p.x,p.y+1))
		&& !from_above){
        cout << "populate_pos_node up" << endl;
        pn->up = populate_pos_node(pos(p.x,p.y-1),false, true);
        if(pn->up){pn->count_vert += pn->up->count_vert + 1;}
	} //end if up
	*/
	
return(pn);
} //end populate_pos_node

unsigned CharMatcher::x_all_nodes(pos_node* pn){
	unsigned node_count =1;
	if(debug){cout << "x_all_nodes:"; pn->p.print();}
	board[pn->p.x][pn->p.y]=0;
	if(pn->down){node_count += x_all_nodes(pn->down);}
	if(pn->up){node_count += x_all_nodes(pn->up);}
	if(pn->right){node_count += x_all_nodes(pn->right);}
	return(node_count);
}

bool CharMatcher::board_has_x(){
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            if(board[x][y] == 0){return(true);}
        } // end x
    } // end y
        return(false);
} // end has_x

void CharMatcher::shiftdown(){
// TODO: add class boolean if invoked from console to display or not to display the board
    srand(9000);
    do {
        for (int y = 0; y < this->height - 1; y++) {
            for (int x = 0; x < this->width; x++) {
                if ((board[x][y] == 0) && (y == 0)) { //top create the random jem
                    board[x][y] = (rand() % (this->num_of_symbols - 1)) + 1;
                    usleep(10000); //time delay
                    clear_screen();
                    draw_board();
                }

                // if board at position is 'x'
                if (board[x][y + 1] == 0) { // get the above jem
                    board[x][y + 1] = board[x][y];
                    board[x][y] = 0; // make the swapped 'x'
                    usleep(10000); //time delay
                    clear_screen();
                    draw_board();
                }

                if ((board[x][y] == 0) && (y == 0)) { //top create the random jem
                    board[x][y] = (rand() % (this->num_of_symbols - 1)) + 1;
                    usleep(10000); //time delay
                    clear_screen();
                    draw_board();
                }

            } //end x width
        } //end y height
    } while (board_has_x());

} // end shiftdown

//TODO: create matches to matches and it returns a pos struct of the largest one
pos_node* CharMatcher::get_biggest_match(){

	unsigned max_count = 0;
    vector<pos> three_matches = get_three_matches_pos();
	pos_node* biggest = 0;
    if (debug){cout << "three_matches.size():" << three_matches.size() << endl;}
    for (unsigned i =0; i < three_matches.size(); i++){
        pos_node* pn = populate_pos_node(three_matches.at(i));
        if((! pn) && debug){cerr << "pn is null";}
        else {
            if(max_count < (pn->count_vert + pn->count_right + 1)){
				biggest = pn;
                max_count = pn->count_vert + pn->count_right + 1;
                if (debug){cout << "max_count:" << max_count << endl;}
            }
        }
        // TODO: add if for max calc
    }
	return(biggest);
} //end get_biggest_match

//return score 0 if nothing to match
void CharMatcher::swap(pos p1, pos p2){
	unsigned moves_score = 0;
	// 	TODO: check distance, currently can switch any on the board
	// TODO: check bounds can swap garbage
	int temp = board[p1.x][p1.y];
	board[p1.x][p1.y] = board[p2.x][p2.y];
	board[p2.x][p2.y] = temp;
}
//validates move
int CharMatcher::move(string move){
	if(move.empty()) {return(-3);}
	pos sym_pos1;
	pos sym_pos2;
	size_t space1 = move.find(' ');
	size_t space2 = move.rfind(' ');
	if ((space1 == string::npos) || (space2 == string::npos)){
		return(-4);
	} //end if cant find first or second space
	
	string token1 = move.substr(0,space1);
	string token2 = move.substr(space1+1,space2-space1);
	string token3 = move.substr(space2+1);
	bool d1 = isdigit(token1[0]);
	bool d2 = isdigit(token2[0]);
	bool d3 = isdigit(token3[0]);
	//Vertical Swap
	if ((d1 && d2 && !d3) || (d1 && !d2 && d3) || (!d1 && d2 && d3)){
		if (debug){cout << "Horz swap";}
		if(!d3){ //if token3 is a letter
			sym_pos1.x = toupper(token3[0]) - (unsigned)'A';
			sym_pos1.y = (unsigned)stoi(token1);
			sym_pos2.x = toupper(token3[0]) - (unsigned)'A';
			sym_pos2.y = (unsigned)stoi(token2);
		} //end if token3 is a letter
		else if(!d2) { //if token2 is a letter
			sym_pos1.x = toupper(token2[0]) - (unsigned)'A';
			sym_pos1.y = (unsigned)stoi(token1);
			sym_pos2.x = toupper(token2[0]) - (unsigned)'A';
			sym_pos2.y = (unsigned)stoi(token3);
		} //end if token2 is a letter
		else { //if token1 is a letter
			sym_pos1.x = toupper(token1[0]) - (unsigned)'A';
			sym_pos1.y = (unsigned)stoi(token2);
			sym_pos2.x = toupper(token1[0]) - (unsigned)'A';
			sym_pos2.y = (unsigned)stoi(token3);
		} // end else (if token1 is a letter)
	//Horizontal Swap
	} else if ((d1 && !d2 && !d3) 
			|| (!d1 && d2 && !d3) 
			|| (!d1 && !d2 && d3)){
		if (debug){cout << "Vert Swap";}
		if(d1){ //if token1 is a digit
			sym_pos1.x = toupper(token2[0]) - (unsigned)'A';
			sym_pos1.y = (unsigned)stoi(token1);
			sym_pos2.x = toupper(token3[0]) - (unsigned)'A';
			sym_pos2.y = (unsigned)stoi(token1);
		} //end if token1 is a digit
		else if(d2){ //if token2 is a digit
			sym_pos1.x = toupper(token1[0]) - (unsigned)'A';
			sym_pos1.y = (unsigned)stoi(token2);
			sym_pos2.x = toupper(token3[0]) - (unsigned)'A';
			sym_pos2.y = (unsigned)stoi(token2);
		} //end //if token2 is a digit
		else{ //if token3 is a digit
			sym_pos1.x = toupper(token1[0]) - (unsigned)'A';
			sym_pos1.y = (unsigned)stoi(token3);
			sym_pos2.x = toupper(token2[0]) - (unsigned)'A';
			sym_pos2.y = (unsigned)stoi(token3);
		} //end if token3 is a digit
	 } else {cout << "What?"; return(-2);}
	 //f 8 g not working
	if (debug){
		cout << endl << "sym_pos1:"; sym_pos1.print();
		cout << "sym_pos2:"; sym_pos2.print();
	}
	if (legal_move(sym_pos1, sym_pos2)){
		swap(sym_pos1,sym_pos2);
		unsigned moves_score = 0;
		draw_board();
		usleep(100); //time delay
		while (any_matches()){ // change to while //move to update board
			moves_score += x_all_nodes(get_biggest_match()); //remove biggest match
			shiftdown();
		}
		//if no match then put it back and
		////symbols[0]; // x out the matches
		//do it (Swap)
		//returned string is feed back like invlid move, good, sweet, exilent, extraorinary e1337, I cant beleve its not butter
		return(moves_score);
	} //end if legal_move
	
	//now check the move
return(-1);
} //end move

void CharMatcher::consoleUI(){
    cout << "Rev0.1" << endl;
	string input = "NONE";
	int status = 0;
	clear_screen();
	draw_board();
	cout << "Enter moves like \"a b 12\" or \"11 a 12\":";
	while (!no_more_moves() && getline(cin,input)
		   && input != "stop" && input != "done" && input != "gg"){
		clear_screen();
		status = move(input);
		clear_screen();
		draw_board();
		cout.width(8);
        if (status <= 0){cout << "Last Move Error" << endl;} else {score += status;}
		cout << right << "\tScore: " << score << endl;
		cout << R"(Enter moves like "a b 12" or "11 a 12")" << endl;
		cout <<"Last Move:"<< input << " was "<< status << " matches" <<endl << "Next:";
	} //end while

} //end consoleUI

