#include "../header/event.h"
#include <stdexcept>
int choice;

void mainMenu() {
	printf("-----------------------------------\n");
	printf("WELCOME TO OUR PLANNER!!\n");
	printf("-----------------------------------\n");
	printf("1. User Authentication\n");
	printf("2. Event Details\n");
	printf("3. Attendee Management\n");
	printf("4. Schedule Organizer\n");
	printf("5. Feedback Collection\n");
	printf("6. Exit\n");
	printf("Please enter your choice:");
	scanf("%d", choice);
	void menu();
}

void menu() {
	if (choice == 1) {
		void authentication();
	}
	else if (choice == 2) {
		void eventDetails();
	}
	else if (choice == 3) {
		void attendee();
	}
	else if (choice == 4) {
		void schedule();
	}
	else if (choice == 5) {
		void feedback();
	}
	else if (choice == 6) {
		void exit();
	}
	else {
		printf("Invalid choice. Please try again.");
		return mainMenu();
	}
}

void authentication() {

}