/**
 * @file event.h
 * 
 * @brief Provides functions for math. utilities
 */

#ifndef EVENT_H
#define EVENT_H

#include "../../utility/header/commonTypes.h" 
typedef struct User;
void addToXORList(const char* value);
void removeFromXORList(const char* value);
void saveUser(User* newUser);
void saveUserData(User user);
void displayXORList();
void printHashTable();
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
void quadraticProbing();

#endif // EVENT_H