/**
 * @file event.h
 * 
 * @brief Provides functions for math. utilities
 */

#ifndef EVENT_H
#define EVENT_H

#include "../../utility/header/commonTypes.h" 

void printHashTable();
bool mainMenu();
bool authentication();
bool Register();
bool logIn();
bool guest();
bool createEvent();
bool manageEvent();
bool eventDetails();
bool schedule();
bool feedback();
bool attendee();
bool registerAttendees();
bool trackAttendees();
void addAttendee();
void deleteAttendee();
void updateAttendee();
void compressData();
void decompressData();
void saveHashTableToFile();
void loadHashTableFromFile();
void clear_screen();
void loadHashTableFromFile(void);

#endif // EVENT_H