#include "../header/event.h"
#include <stdexcept>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector> // std::vector kullanýmý için
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>

#define TABLE_SIZE 100
#define MAX_ATTENDEES 100
#define MAX_TREE_NODES 256

// Define the structure for an Attendee
typedef struct Attendee {
    char name[50];
    char surname[50];
    char huffmanCode[256]; // Store the Huffman code
} Attendee;

// Define a structure for the Huffman tree nodes
typedef struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode* left, * right;
} MinHeapNode;

// Define a structure for Min Heap
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode** array;
} MinHeap;

// Function declarations
void kmpSearch(char* pattern);
void computeLPSArray(char* pattern, int M, int* lps);
void compressAttendeeName(Attendee* attendee);
void buildHuffmanTree(char* str, Attendee* attendee);
MinHeap* createMinHeap(unsigned capacity);
MinHeapNode* createMinHeapNode(char data, unsigned freq);
void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode);
MinHeapNode* extractMin(MinHeap* minHeap);
void generateHuffmanCodes(MinHeapNode* root, char* code, int top, char* huffmanCode);
void printAttendees(); // New function declaration

// Global variables
Attendee attendees[MAX_ATTENDEES];
int attendeeCount = 0;


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

// Function to compress attendee names using Huffman coding
void compressAttendeeName(Attendee* attendee) {
    char combined[100];
    snprintf(combined, sizeof(combined), "%s %s", attendee->name, attendee->surname);
    buildHuffmanTree(combined, attendee); // Pass attendee to store the Huffman code
}

// Function to create a Min Heap
MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));
    return minHeap;
}

// Function to create a Min Heap Node
MinHeapNode* createMinHeapNode(char data, unsigned freq) {
    MinHeapNode* newNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a node into the Min Heap
void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
    minHeap->array[minHeap->size] = minHeapNode;
    minHeap->size++;
}

// Function to extract the minimum node from the Min Heap
MinHeapNode* extractMin(MinHeap* minHeap) {
    MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    return temp;
}

// Function to build Huffman Tree
void buildHuffmanTree(char* str, Attendee* attendee) {
    // Frequency array
    int freq[MAX_TREE_NODES] = { 0 };
    for (int i = 0; str[i] != '\0'; i++) {
        freq[(unsigned char)str[i]]++;
    }

    MinHeap* minHeap = createMinHeap(MAX_TREE_NODES);

    // Create a min-heap for characters with non-zero frequency
    for (int i = 0; i < MAX_TREE_NODES; i++) {
        if (freq[i]) {
            MinHeapNode* minHeapNode = createMinHeapNode(i, freq[i]);
            insertMinHeap(minHeap, minHeapNode);
        }
    }

    // Build the Huffman Tree
    while (minHeap->size != 1) {
        MinHeapNode* left = extractMin(minHeap);
        MinHeapNode* right = extractMin(minHeap);

        MinHeapNode* top = createMinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // The remaining node is the root of the Huffman Tree
    MinHeapNode* root = extractMin(minHeap);
    char huffmanCode[256] = { 0 }; // Buffer to store the generated code
    generateHuffmanCodes(root, huffmanCode, 0, attendee->huffmanCode);
}

// Function to generate Huffman Codes
void generateHuffmanCodes(MinHeapNode* root, char* code, int top, char* huffmanCode) {
    if (root->left) {
        code[top] = '0';
        generateHuffmanCodes(root->left, code, top + 1, huffmanCode);
    }

    if (root->right) {
        code[top] = '1';
        generateHuffmanCodes(root->right, code, top + 1, huffmanCode);
    }

    // Save the generated Huffman code for the character
    if (!(root->left) && !(root->right)) {
        code[top] = '\0';
        sprintf(huffmanCode, "%s%c: %s\n", huffmanCode, root->data, code); // Append code to huffmanCode
    }
}

// KMP search function
void kmpSearch(char* pattern) {
    int M = strlen(pattern);
    int found = false;

    for (int i = 0; i < attendeeCount; i++) {
        // Here we need to use the stored Huffman code instead of the name for matching
        // Implement KMP algorithm with Huffman coded names
    }

    if (!found) {
        printf("No match found.\n");
    }
}

// Compute LPS array used in KMP pattern search
void computeLPSArray(char* pattern, int M, int* lps) {
    int length = 0;
    lps[0] = 0;
    int i = 1;
    while (i < M) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        }
        else {
            if (length != 0) {
                length = lps[length - 1];
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
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
            clear_screen();
            printf("Invalid login. Returning to main menu.\n");
            return mainMenu();
        }
        break;
    case 3:
        guest();
        return mainMenu();
        break;
    default:
        clear_screen(); 
        printf("Invalid choice. Please try again.\n");
        return mainMenu();
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
    clear_screen();
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
            clear_screen();   
            break;
        case 2:
            if (current->prev != NULL) {
                current = current->prev;
            }
            else {
                printf("No previous event.\n");
            }
            clear_screen(); 
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
        clear_screen(); 
        printf("Invalid choice. Please try again.\n");
        return mainMenu();
    }
    return true;
}

// Main attendee function
bool attendee() {
    int choice;
    printf("1. Register Attendees\n");
    printf("2. Track Attendees\n");
    printf("3. Print Attendees\n"); // New option for printing attendees
    printf("Please enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        registerAttendees();
        break;
    case 2: {
        clear_screen();  
        char searchName[50];
        printf("Enter the name to search: ");
        scanf("%s", searchName);
        kmpSearch(searchName);
        break;
    }
    case 3: // Call printAttendees function
        printAttendees();
        break;
    default:
        clear_screen();  
        printf("Invalid choice. Please try again.\n");
        return mainMenu();
    }
    return true;
}

// Function to register attendees
bool registerAttendees() {
    int count;
    printf("How many people will attend? ");
    scanf("%d", &count);

    if (count <= 0 || count > MAX_ATTENDEES) {
        printf("Invalid number! Please enter a value between 1 and %d.\n", MAX_ATTENDEES);
        return false;
    }

    for (int i = 0; i < count; i++) {
        printf("Enter the name of attendee %d: ", i + 1);
        scanf("%s", attendees[attendeeCount].name);
        printf("Enter the surname of attendee %d: ", i + 1);
        scanf("%s", attendees[attendeeCount].surname);
        compressAttendeeName(&attendees[attendeeCount]); // Compress name and store Huffman code
        attendeeCount++;
    }
    clear_screen();  
    printf("%d attendees registered.\n", count);
    return mainMenu();
}


// Function to print registered attendees
void printAttendees() {
    printf("\nRegistered Attendees:\n");
    for (int i = 0; i < attendeeCount; i++) {
        printf("Name: %s, Surname: %s, Huffman Code: %s\n", attendees[i].name, attendees[i].surname, attendees[i].huffmanCode);
    }
}

bool schedule() {
    return true;
}

bool feedback() {
    return true;
}
