#include "../header/event.h"
#include <stdexcept>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 100

void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");  // Windows için
#elif defined(__linux__)
    system("clear");  // Linux ve WSL için
#endif
}

typedef struct User {
    char name[50];
    char surname[50];
    char phone[20];
    char password[20];
    struct User* next;  // Çarpýþmalarý çözmek için baðlý liste kullanýyoruz
} User;

User* hashTable[TABLE_SIZE];

// Structure definitions
typedef struct Event {
    char type[50];
    char date[20];
    char color[20];
    char concept[50];
    struct Event* prev;
    struct Event* next;
} Event;

Event* head = NULL;
Event* tail = NULL;

// Hash fonksiyonu
unsigned int hash(const char* phone) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(phone); i++) {
        hash = (hash * 31 + phone[i]) % TABLE_SIZE;
    }
    return hash;
}

// Kullanýcýyý hash tablosuna kaydetme
void saveUser(User* newUser) {
    unsigned int index = hash(newUser->phone);
    newUser->next = hashTable[index];
    hashTable[index] = newUser;
}

// Hash tablosunu dosyaya kaydetme
void saveHashTableToFile() {
    FILE* file = fopen("user_data.bin", "wb");
    if (file == NULL) {
        perror("Dosya açýlýrken hata oluþtu");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        while (current != NULL) {
            fwrite(current, sizeof(User), 1, file);
            current = current->next;
        }
    }
    fclose(file);
}

// Hash tablosunu dosyadan yükleme
void loadHashTableFromFile() {
    FILE* file = fopen("user_data.bin", "rb");
    if (file == NULL) {
        perror("Dosya açýlýrken hata oluþtu");
        return;
    }

    while (!feof(file)) {
        User* newUser = (User*)malloc(sizeof(User));
        if (fread(newUser, sizeof(User), 1, file) == 1) {
            newUser->next = NULL;
            saveUser(newUser);
        }
        else {
            free(newUser);
        }
    }
    fclose(file);
}

// Kullanýcý verilerini dosyaya ve hash tablosuna kaydetme
void saveUserData(User user) {
    User* newUser = (User*)malloc(sizeof(User));
    *newUser = user;
    saveUser(newUser); // Hash tablosuna ekle
    saveHashTableToFile(); // Dosyaya kaydet
    clear_screen();
}

// Hash tablosunun içeriðini yazdýrma
void printHashTable() {
    printf("Hash Table Contents:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        if (current == NULL) {
            continue; // Boþ olan hücreleri atla
        }
        printf("Index %d:\n", i);
        while (current != NULL) {
            printf(" Name: %s %s, Phone: %s, Password: %s\n",
                current->name, current->surname, current->phone, current->password);
            current = current->next;
        }
    }
    printf("End of Hash Table.\n");
}

bool validateLogin(const char* phone, const char* password) {
    unsigned int index = hash(phone);
    User* current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->phone, phone) == 0 && strcmp(current->password, password) == 0) {
            return true; // Giriþ baþarýlý
        }
        current = current->next;
    }
    return false; // Giriþ baþarýsýz
}

// Ana menü
bool mainMenu() {
    int choice;
    printf("-----------------------------------\n");
    printf("WELCOME TO OUR PLANNER!!\n");
    printf("-----------------------------------\n");
    printf("1. User Authentication\n");
    printf("2. Event Details\n");
    printf("3. Attendee Management\n");
    printf("4. Schedule Organizer\n");
    printf("5. Feedback Collection\n");
    printf("6. Exit\n");
    printf("Please enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        authentication();
        break;
    case 2:
        eventDetails();
        break;
    case 3:
        attendee();
        break;
    case 4:
        schedule();
        break;
    case 5:
        feedback();
        break;
    case 6:
        exit(0);
    default:
        printf("Invalid choice. Please try again.\n");
        return false;
    }

    return true;
}

// Kullanýcý doðrulama
bool authentication() {
    int login;
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Guest Login\n");
    printf("Please enter your choice: ");
    scanf("%d", &login);
    switch (login) {
    case 1:
        Register();
        break;
    case 2:
        if (logIn()) {
            clear_screen();
            printf("Login successful!\n");
            return mainMenu();
        }
        else {
            printf("Invalid login. Returning to main menu.\n");
            return mainMenu();
        }
        break;
    case 3:
        guest();
        return mainMenu();
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        return false;
    }
    return true;
}

// Kullanýcý kaydý
bool Register() {
    User newUser;
    printf("Enter your name: ");
    scanf("%s", newUser.name);
    printf("Enter your surname: ");
    scanf("%s", newUser.surname);
    printf("Enter your phone number: ");
    scanf("%s", newUser.phone);
    printf("Enter your password: ");
    scanf("%s", newUser.password);

    saveUserData(newUser);
    clear_screen();
    printf("Registration successful! You can now log in.\n");

    printHashTable(); // Her kayýttan sonra tabloyu yazdýr
    return mainMenu();
}

// Giriþ iþlemi
bool logIn() {
    char phone[20];
    char password[20];
    printf("Enter your phone number: ");
    scanf("%s", phone);
    printf("Enter your password: ");
    scanf("%s", password);

    return validateLogin(phone, password);
}

// Misafir giriþi fonksiyonu
bool guest() {
    printf("Guest login successful!\n");
    return true;
}

// Function to create a new event
bool createEvent() {
    Event* newEvent = (Event*)malloc(sizeof(Event));

    printf("Enter event type: ");
    scanf(" %[^\n]%*c", newEvent->type);

    printf("Enter event date (e.g., 01-01-2025): ");
    scanf(" %[^\n]%*c", newEvent->date);

    printf("Enter color option: ");
    scanf(" %[^\n]%*c", newEvent->color);

    printf("Enter concept: ");
    scanf(" %[^\n]%*c", newEvent->concept);

    newEvent->prev = tail;
    newEvent->next = NULL;

    if (tail != NULL) {
        tail->next = newEvent;
    }
    else {
        head = newEvent;
    }
    tail = newEvent;
    clear_screen();
    printf("Event created successfully!\n");
    return mainMenu();
}
// Function to manage events
bool manageEvent() {
    if (head == NULL) {
        printf("No events available. Please create an event first.\n");
        return false; // Changed to return false
    }

    Event* current = head;
    int choice;

    while (1) {
        printf("\n--- Event Information ---\n");
        printf("Type: %s\n", current->type);
        printf("Date: %s\n", current->date);
        printf("Color: %s\n", current->color);
        printf("Concept: %s\n", current->concept);

        printf("\n1. Go to the next event\n");
        printf("2. Go to the previous event\n");
        printf("3. Update event information\n");
        printf("4. Return to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            if (current->next != NULL) {
                current = current->next;
            }
            else {
                printf("No next event.\n");
            }
            break;
        case 2:
            if (current->prev != NULL) {
                current = current->prev;
            }
            else {
                printf("No previous event.\n");
            }
            break;
        case 3:
            printf("Enter new type: ");
            scanf(" %[^\n]%*c", current->type);

            printf("Enter new date: ");
            scanf(" %[^\n]%*c", current->date);

            printf("Enter new color: ");
            scanf(" %[^\n]%*c", current->color);

            printf("Enter new concept: ");
            scanf(" %[^\n]%*c", current->concept);

            printf("Event updated successfully!\n");
            clear_screen();
            return mainMenu(); // Ensure mainMenu() returns bool if needed
            break;
        case 4:
            return mainMenu(); // Ensure mainMenu() returns bool if needed
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return true; // Added return statement here
}


// Event Details main menu
bool eventDetails() {
    int event;
    printf("1. Create Event\n");
    printf("2. Manage Event\n");
    printf("Please enter your choice: ");
    scanf("%d", &event);

    switch (event) {
    case 1:
        createEvent();
        break;
    case 2:
        manageEvent();
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        return false;
    }
    return true;
}

bool attendee() {
    int attendees;
    printf("1. Register Attendees\n");
    printf("2. Track Attendees\n");
    printf("Please enter your choice: ");
    scanf("%d", &attendees);
    switch (attendees) {
    case 1:
        registerAttendees();
        break;
    case2:
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        return false;
    }
    return true;
}
bool registerAttendees() {
    return true;
}

bool trackAttendees() {
    return true;
}
bool schedule() {
    return true;
}

bool feedback() {
    return true;
}
