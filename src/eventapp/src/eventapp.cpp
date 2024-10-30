/**
 * @file eventapp.cpp
 * @brief A simple program to demonstrate the usage of the event model class.
 *
 * This program process infix notations and calculate operations
 *
 */

 // Standard Libraries
#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include "../../event/header/event.h"  // Adjust this include path based on your project structure
#include "../../event/src/event.cpp"

int main()
{
	loadHashTableFromFile();
	mainMenu();
}