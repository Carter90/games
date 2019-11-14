#include <iostream>
#include <random>
#include "CharMatcher.h"
using namespace std;
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
	CharMatcher char_mather;
	char_mather.consoleUI();
	cout << "Qapla'!" <<endl;
	return(0); 
} //end main
