/**
 * @file eventapp.cpp
 * @brief Demonstrates the usage of the event model class by implementing a simple program.
 *
 * This program is designed to process infix notations and perform calculations
 * based on the given expressions. It demonstrates the integration of standard
 * C++ libraries with custom event models to provide a complete example of
 * handling arithmetic operations and events.
 *
 * ## Key Features:
 * - Utilizes stacks for efficient infix expression processing.
 * - Employs custom event handling mechanisms using the event model class.
 * - Integrates external modules for enhanced functionality (e.g., loading data into a hash table).
 *
 * The program includes essential functions like initializing a hash table, loading its data
 * from an external file, and navigating through a main menu interface for user interaction.
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
	memset(hashTable, 0, sizeof(hashTable));
	loadHashTableFromFile();
	mainMenu();
}