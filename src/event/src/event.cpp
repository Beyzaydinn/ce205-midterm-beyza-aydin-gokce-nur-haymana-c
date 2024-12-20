#include "../header/event.h"
#include <stdexcept>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <ctype.h>


void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");  // for Windows
#elif defined(linux)
    system("clear");  // for Linux and WSL
#endif
}

#define TABLE_SIZE 100
#define MAX_TREE_NODES 256

// Define the structure for an Attendee
typedef struct AttendeE {
    char nameAttendee[50];
    char surnameAttendee[50];
    char huffmanCode[256]; // Store the Huffman code
} AttendeE;

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
    MinHeapNode** array;  // Pointer to an array of MinHeapNode pointers
} MinHeap;

// Function to create a Min Heap
MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;

    // Only allocate memory for array if capacity is greater than 0
    if (capacity > 0) {
        minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode));
    }
    else {
        minHeap->array = nullptr;
    }

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

// Function declarations  
void kmpSearch(char* pattern);
void computeLPSArray(char* pattern, int M, int* lps);
void compressAttendeeName(AttendeE* attendee);
void buildHuffmanTree(char* str, AttendeE* attendee);
MinHeap* createMinHeap(unsigned capacity);
MinHeapNode* createMinHeapNode(char data, unsigned freq);
void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode);
MinHeapNode* extractMin(MinHeap* minHeap);
void generateHuffmanCodes(MinHeapNode* root, char* code, int top, char* huffmanCode);
void printAttendees(); // New function declaration
void progressiveOverflowAlgorithm(); // New function declaration

typedef struct User {
    char name[50];
    char surname[50];
    char phone[20];
    char password[20];
    struct User* next;  // We use linked list to resolve collisions
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

// Hash function
unsigned int hash(const char* phone) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(phone); i++) {
        hash = (hash * 31 + phone[i]) % TABLE_SIZE;
    }
    return hash;
}

// Saving user to hash table
void saveUser(User* newUser) {
    unsigned int index = hash(newUser->phone);
    newUser->next = hashTable[index];
    hashTable[index] = newUser;
}

// Saving hash table to file
void saveHashTableToFile() {
    FILE* file = fopen("users.bin", "wb");
  /*  if (file == NULL) {
        perror("An error occurred while opening the file");
        return;
    }*/

    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        while (current != NULL) {
            fwrite(current, sizeof(User), 1, file);
            current = current->next;
        }
    }
    fclose(file);
}

void loadHashTableFromFile() {
    FILE* file = fopen("users.bin", "rb");
   /* if (file == NULL) {
        perror("An error occurred while opening the file");
        return;
    }*/

    while (1) {
        User* newUser = (User*)malloc(sizeof(User));
        // Eðer fread 1'den az veri okursa, dosya boþ olabilir veya okuma hatasý vardýr
        if (fread(newUser, sizeof(User), 1, file) != 1) {
            free(newUser); // Dosya bittiðinde veya okuma hatasýnda belleði temizle
            break; // Dosya sonlandýðýnda döngüden çýk
        }
        newUser->next = NULL;
        saveUser(newUser);  // Kullanýcýyý hash tablosuna ekle
    }

    fclose(file); // Dosyayý kapat
}

bool quadraticProbingInsert(User* newUser) {
    unsigned int index = hash(newUser->phone);
    unsigned int i = 0;
    unsigned int originalIndex = index;
    unsigned int startIndex = index;

    while (hashTable[index] != NULL && i < TABLE_SIZE) {
        i++;
        index = (originalIndex + i * i) % TABLE_SIZE;

        if (index == startIndex) {
            printf("Hash table full. User not added.\n");
            return false;
        }
    }

    if (i < TABLE_SIZE) {
        hashTable[index] = newUser;
        return true;
    }
    //else {
    //    printf("Hash table full. User not added.\n");
    //    return false;
    //}
}


// Saving user data to file and hash table
void saveUserData(User user) {
    User* newUser = (User*)malloc(sizeof(User));
    *newUser = user;
    saveUser(newUser); //Add to hash table
    saveHashTableToFile(); // save to file
    clear_screen();
}

// Hash tablosunu dosyadan yükleme
void printHashTable() {
    /*   if (hashTable == nullptr) {
           printf("Hash table is not initialized.\n");
           return;
       }*/

    printf("Hash Table Contents:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        if (current == NULL) {
            continue; // Skip empty cells 
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

// Function to build Huffman Tree
void buildHuffmanTree(char* str, AttendeE* attendee) {
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

bool validateLogin(const char* phone, const char* password) {
    if (!phone || !password) {
        return false; // Giriþ parametrelerinin geçersizliði
    }

    unsigned int index = hash(phone);
    User* current = hashTable[index];
    while (current) {
        if (strcmp(current->phone, phone) == 0 && strcmp(current->password, password) == 0) {
            return true; // Giriþ baþarýlý
        }
        current = current->next;
    }
    return false; // Giriþ baþarýsýz
}

void progressiveOverflowAlgorithm() {
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int overflowThreshold = 7; // Set the overflow threshold for demonstration
    int currentSum = 0;

    // Step through each element and calculate sum
    for (int i = 0; i < 10; i++) {
        currentSum += array[i];
        if (currentSum > overflowThreshold) {
            printf("Overflow detected at element %d with sum %d\n", i, currentSum);
            // Corrective action: Reset but include current element in new sum
            currentSum = array[i];
        }
    }
}

// main menu

bool mainMenu() {
    int choice;
    do {
        printf("\n-----------------------------------\n");
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
            return false;
        default:
            printf("Invalid choice. Please try again.\n");
            return false;
        }
    } while (choice != 6);
    return true;
}



void progressiveOverflow() {
    printf("Executing Progressive Overflow algorithm...\n");
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int overflowThreshold = 7; // Set the overflow threshold for demonstration
    int currentSum = 0;

    for (int i = 0; i < 10; i++) {
        currentSum += array[i];
        if (currentSum > overflowThreshold) {
            printf("Overflow detected at index %d with sum %d\n", i, currentSum);
            currentSum = 0; // Reset for simplicity
        }
    }
}

void linearProbing() {
    printf("Executing Linear Probing algorithm...\n");
    int hashTable[10];
    for (int i = 0; i < 10; i++) {
        hashTable[i] = -1; // Baþlangýçta tüm deðerler boþ (-1)
    }

    int keys[] = { 23, 45, 12, 37, 29 };
    int size = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;
        int startIndex = index;
        bool placed = false; // Anahtarýn yerleþtirilip yerleþtirilmediðini takip eder

        do {
            if (hashTable[index] == -1) {
                hashTable[index] = keys[i];
                placed = true; // Anahtar yerleþtirildi
                break;
            }
            index = (index + 1) % 10;
        } while (index != startIndex);

        if (!placed) {
            printf("Hash table is full, cannot place key %d\n", keys[i]);
        }
    }

    // Hash tablosunun çýktýsýný yazdýr
    for (int i = 0; i < 10; i++) {
        if (hashTable[i] != -1) {
            printf("Index %d: %d\n", i, hashTable[i]);
        }
        else {
            printf("Index %d: Empty\n", i);
        }
    }
}



void quadraticProbing() {
    printf("Executing Quadratic Probing algorithm...\n");
    int hashTable[10] = { 0 };
    int keys[] = { 23, 45, 12, 37, 29 };
    int size = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;
        int j = 0;
        while (hashTable[(index + j * j) % 10] != 0) {
            j++;
        }
        hashTable[(index + j * j) % 10] = keys[i];
    }

    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d\n", i, hashTable[i]);
    }
}


void doubleHashing() {
    printf("Executing Double Hashing algorithm...\n");
    int hashTable[10] = { 0 };  // Hash tablosu baþlangýçta 0
    int keys[] = { 23, 45, 12, 37, 29 };  // Eklenecek anahtarlar
    int size = sizeof(keys) / sizeof(keys[0]);  // Anahtarlarýn sayýsý

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;  // Anahtar için ilk hash hesaplamasý
        int step = 7 - (keys[i] % 7);  // Çift hashleme için ikinci hash fonksiyonu
        while (hashTable[index] != 0) {  // Eðer yer doluysa
            index = (index + step) % 10;  // Çakýþma çözümü için adým ekle
        }
        hashTable[index] = keys[i];  // Anahtarý doðru indekse yerleþtir
    }

    // Hash tablosunun çýktýsýný yazdýr
    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d\n", i, hashTable[i]);
    }
}


void useBuckets() {
    printf("Executing Use of Buckets algorithm...\n");
    int buckets[3][10] = { 0 };
    int keys[] = { 23, 45, 12, 37, 29 };
    int size = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        int bucketIndex = keys[i] % 3;
        for (int j = 0; j < 10; j++) {
            if (buckets[bucketIndex][j] == 0) {
                buckets[bucketIndex][j] = keys[i];
                break;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        printf("Bucket %d: ", i);
        for (int j = 0; j < 10; j++) {
            if (buckets[i][j] != 0) {
                printf("%d ", buckets[i][j]);
            }
        }
        printf("\n");
    }
}


void linearQuotient() {
    printf("Executing Linear Quotient algorithm...\n");
    int hashTable[10] = { 0 };
    int keys[] = { 23, 45, 12, 37, 29 };
    int size = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;
        int increment = 1; // Linear increment
        while (hashTable[index] != 0) {
            index = (index + increment) % 10;
        }
        hashTable[index] = keys[i];
    }

    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d\n", i, hashTable[i]);
    }
}

void brentsMethod() {
    printf("Executing Brent's Method algorithm...\n");
    int hashTable[10] = { 0 };
    int keys[] = { 23, 45, 12, 37, 29 };
    int size = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;
        int step = 1; // Initial step
       /* while (hashTable[index] != 0) {
            int newIndex = (index + step) % 10;
            if (hashTable[newIndex] == 0) {
                index = newIndex;
                break;
            }
            step++;
        }*/
        hashTable[index] = keys[i];
    }

    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d\n", i, hashTable[i]);
    }
}

void handleFileOperation(int choice) {
    switch (choice) {
    case 1:
        progressiveOverflow();
        break;
    case 2:
        linearProbing();
        break;
    case 3:
        quadraticProbing();
        break;
    case 4:
        doubleHashing();
        break;
    case 5:
        useBuckets();
        break;
    case 6:
        linearQuotient();
        break;
    case 7:
        brentsMethod();
        break;
    case 8:
        printf("Returning to Authentication Menu.\n");
        break;
    default:
        printf("Invalid choice. Please try again.\n");
    }
}

void fileOperationsMenu() {
    int choice;
  do {
        printf("----------- File Operations Menu -----------\n");
        printf("1. Progressive Overflow\n");
        printf("2. Linear Probing\n");
        printf("3. Quadratic Probing\n");
        printf("4. Double Hashing\n");
        printf("5. Use of Buckets\n");
        printf("6. Linear Quotient\n");
        printf("7. Brent's Method\n");
        printf("8. Back to Authentication Menu\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        if (choice == 8) {
            handleFileOperation(choice);
            break;
        }
        handleFileOperation(choice);
  } while (choice != 8);
}



bool authentication() {
    int login;
    printf("----------- Authentication Menu -----------\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Guest Login\n");
    printf("4. File Operations for Fast Search Operations\n");
    printf("5. Return to main menu\n");
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
        }
        else {
            clear_screen();
            printf("Invalid login. Returning to main menu.\n");
        }
        break;
    case 3:
        guest();
        break;
    case 4:
        fileOperationsMenu();
        break;
    case 5:
        return false;
    default:
        clear_screen();
        printf("Invalid choice. Please try again.\n");
        break;
    }
    return true;
}

// User registration
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

    printHashTable(); //Print table after each record
    mainMenu();
    return true;
}

bool logIn() {
    char phone[20];
    char password[20];
    printf("Enter your phone number: ");
    scanf("%s", phone);
    printf("Enter your password: ");
    scanf("%s", password);

    return validateLogin(phone, password); // Login doðruluðunu kontrol et
}

// guest mode
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

    // Write event data to the binary file "evet"
    FILE* file = fopen("event.bin", "ab");
    //if (file == NULL) {
    //    perror("Error opening file");
    //    return false;
    //}

    //if (fwrite(newEvent, sizeof(Event), 1, file) != 1) {
    //    perror("Error writing to file");
    //    fclose(file);
    //    return false;
    //}

    fclose(file);

    clear_screen();
    printf("Event created and saved successfully!\n");
    mainMenu();
    return true;
}



// Function to manage events
bool manageEvent() {
    //if (head == NULL) {
    //    printf("No events available. Please create an event first.\n");
    //    return false; // Changed to return false
    //}

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
            return false; // Ensure mainMenu() returns bool if needed
        case 4:
            return false; // Ensure mainMenu() returns bool if needed
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }

    return true; // Added return statement here
}


// Event Details main menu
bool eventDetails() {
    int event;
    printf("\n----------- Event Menu -----------\n");
    printf("1. Create Event\n");
    printf("2. Manage Event\n");
    printf("3. Return to main menu\n");
    printf("Please enter your choice: ");
    scanf("%d", &event);

    switch (event) {
    case 1:
        createEvent();
        break;
    case 2:
        manageEvent();
        break;
    case 3:
        return false;
    default:
        clear_screen();
        printf("Invalid choice. Please try again.\n");
        break;
    }
    return true;
}

#define MAX_ATTENDEES 100
#define MAX_NAME_LENGTH 50

// Struct to hold attendee details
typedef struct {
    char nameAttendee[MAX_NAME_LENGTH];
    char surnameAttendee[MAX_NAME_LENGTH];
    char huffmanCode[MAX_NAME_LENGTH]; // Assuming Huffman code is stored as a string
} Attendee;

Attendee attendees[MAX_ATTENDEES];
int attendeeCount = 0;

// Function prototypes
void kmpSearch(char* pattern);
void computeLPSArray(char* pattern, int M, int* lps);
void compressAttendeeName(Attendee* attendee);
bool registerAttendees();
void printAttendees();
bool attendee();

// Knuth-Morris-Pratt (KMP) search function
void kmpSearch(char* pattern) {
    // Convert pattern to lowercase to make comparison case-insensitive
    for (int i = 0; pattern[i]; i++) {
        pattern[i] = tolower(pattern[i]); // Convert each character to lowercase
    }

    int M = strlen(pattern);
    bool found = false;

    for (int i = 0; i < attendeeCount; i++) {
        // Convert attendee's Huffman code to lowercase before comparison
        char* huffmanCode = attendees[i].huffmanCode;
        int N = strlen(huffmanCode);

        int* lps = (int*)malloc(M * sizeof(int));  // Dynamic memory allocation for LPS array

        computeLPSArray(pattern, M, lps);

        int j = 0;  // index for pattern
        for (int k = 0; k < N; k++) {
            while (j > 0 && pattern[j] != huffmanCode[k]) {
                j = lps[j - 1];
            }

            if (pattern[j] == huffmanCode[k]) {
                j++;
            }

            if (j == M) {
                printf("Pattern found in Huffman code of attendee: %s %s\n", attendees[i].nameAttendee, attendees[i].surnameAttendee);
                found = true;
                break;
            }
        }

        free(lps); // Free allocated memory for LPS array
    }

    /*if (!found) {
        printf("No match found.\n");
    }*/
}

// Compute the LPS array for the pattern (used in KMP search)
void computeLPSArray(char* pattern, int M, int* lps) {
    int length = 0; // length of the previous longest prefix suffix
    lps[0] = 0; // LPS[0] is always 0
    int i = 1;

    // Preprocessing the pattern
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

// Function to compress and store the Huffman code for each attendee's name
void compressAttendeeName(Attendee* attendee) {
    int len = strlen(attendee->nameAttendee);
    // Directly store the name for simplicity (as a placeholder for actual Huffman coding)
    for (int i = 0; i < len; i++) {
        attendee->huffmanCode[i] = attendee->nameAttendee[i]; // Directly storing name characters
    }
    attendee->huffmanCode[len] = '\0'; // Null terminate the string
}

// Main function for registering attendees
bool registerAttendees() {
    int count;
    FILE* file = fopen("attendee.bin", "ab"); // Binary modunda aç (ekleme için)
   /* if (file == NULL) {
        perror("Error opening file");
        return false;
    }*/

    printf("How many people will attend? ");
    scanf("%d", &count);

    if (count <= 0 || count > MAX_ATTENDEES) {
        printf("Invalid number! Please enter a value between 1 and %d.\n", MAX_ATTENDEES);
        fclose(file);
        return false;
    }

    for (int i = 0; i < count; i++) {
        printf("Enter the name of attendee %d: ", i + 1);
        scanf("%s", attendees[attendeeCount].nameAttendee);
        printf("Enter the surname of attendee %d: ", i + 1);
        scanf("%s", attendees[attendeeCount].surnameAttendee);
        compressAttendeeName(&attendees[attendeeCount]); // Huffman kodunu sýkýþtýr ve sakla

        // Katýlýmcý bilgilerini binary dosyasýna yaz
        if (fwrite(&attendees[attendeeCount], sizeof(Attendee), 1, file) != 1) {
         /*   perror("Error writing to file");
            fclose(file);*/
            return false;
        }

        attendeeCount++; // Kayýt sayýsýný artýr
    }

    printf("%d attendees registered and saved in binary format.\n", count);
    fclose(file); // Dosyayý kapat
    return true;
}


// Function to print all registered attendees and their Huffman codes
void printAttendees() {
    printf("\nRegistered Attendees:\n");
    for (int i = 0; i < attendeeCount; i++) {
        printf("Name: %s, Surname: %s, Huffman Code: %s\n", attendees[i].nameAttendee, attendees[i].surnameAttendee, attendees[i].huffmanCode);
    }
}

// Main attendee menu function
bool attendee() {
    int choice;
    printf("----------- Attendee Menu -----------\n");
    printf("1. Register Attendees\n");
    printf("2. Track Attendees\n");
    printf("3. Print Attendees\n");
    printf("4. Manage Attendees List\n"); // New option for managing the list
    printf("5. Return to main menu\n");
    printf("Please enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        registerAttendees();
        return false;
    case 2: {
        char searchName[MAX_NAME_LENGTH];
        printf("Enter the name to search: ");
        scanf("%s", searchName);
        kmpSearch(searchName);  // Search in Huffman code
        return false;
    }
    case 3:
        printAttendees();
        return false;
    case 4: {
        int subChoice;
        printf("--------- Manage Attendees List ---------\n");
        printf("1. Add Attendee\n");
        printf("2. Remove Attendee\n");
        printf("3. Display Activity History\n");
        printf("4. Back to Main Menu\n");
        printf("Please enter your choice: ");
        scanf("%d", &subChoice);

        switch (subChoice) {
        case 1: {
            char nameToAdd[MAX_NAME_LENGTH];
            printf("Enter the name of the attendee to add: ");
            scanf("%s", nameToAdd);
            addToXORList(nameToAdd); // Add the attendee to XOR list
            printf("Attendee added: %s\n", nameToAdd);
            attendee();  // Go back to attendee menu
            break;
        }
        case 2: {
            char nameToRemove[MAX_NAME_LENGTH];
            printf("Enter the name of the attendee to remove: ");
            scanf("%s", nameToRemove);
            removeFromXORList(nameToRemove); // Remove the attendee from XOR list
            printf("Attendee removed: %s\n", nameToRemove);
            attendee();  // Go back to attendee menu
            break;
        }
        case 3:
            displayXORList();  // Display the XOR list
            attendee();  // Go back to attendee menu
            break;
        case 4:
            return false;
        default:
            printf("Invalid choice. Please try again.\n");
            attendee();
        }
        break;
    }
    case 5:
        return false;
    default:
        printf("Invalid choice. Please try again.\n");
        return false;
    }
    return true;
}

#define MAX_SIZE 100 
#define STACK_SIZE 100
#define QUEUE_SIZE 100

// Sparse Matrix Structure
typedef struct {
    int row[MAX_SIZE];
    int col[MAX_SIZE];
    char value[MAX_SIZE][100];  // Store activity details
    int size;  // Number of non-zero entries
} SparseMatrix;

SparseMatrix activityMatrix;  // Global sparse matrix for activities

// Stack Structure
typedef struct {
    char items[STACK_SIZE][100];
    int top;
} Stack;

Stack activityStack;  // Stack for storing activities

// Queue Structure
typedef struct {
    char items[QUEUE_SIZE][100];
    int front, rear;
} Queue;

Queue activityQueue;  // Queue for storing activities

// XOR Linked List Structure
typedef struct XORNode {
    char value[100];
    struct XORNode* both; // XOR of previous and next node
} XORNode;

XORNode* xorHead = NULL; // Head of the XOR linked list

// Function to get the XOR of two pointers
XORNode* XOR(XORNode* a, XORNode* b) {
    return (XORNode*)((uintptr_t)(a) ^ (uintptr_t)(b));
}

// Function to add a new node to the XOR linked list
void addToXORList(const char* value) {
    XORNode* newNode = (XORNode*)malloc(sizeof(XORNode));
    strcpy(newNode->value, value);
    newNode->both = XOR(xorHead, NULL); // Set both to XOR of head and NULL

    if (xorHead != NULL) {
        // Update head's both pointer
        XORNode* next = XOR(xorHead->both, NULL);
        xorHead->both = XOR(newNode, next);
    }
    xorHead = newNode; // Move head to the new node
}

// Function to remove a node from the XOR linked list
void removeFromXORList(const char* value) {
    XORNode* current = xorHead;
    XORNode* prev = NULL;
    XORNode* next;

    while (current != NULL) {
        if (strcmp(current->value, value) == 0) {
            if (prev != NULL) {
                prev->both = XOR(prev->both, current);
            }
            else {
                xorHead = XOR(xorHead->both, NULL); // If it's the head node, adjust head
            }

            next = XOR(prev, current->both);
            free(current);  // Free the current node
            current = next; // Move to the next node
        }
        else {
            next = XOR(prev, current->both);
            prev = current;
            current = next;
        }
    }
}

// Function to display the XOR linked list
void displayXORList() {
    XORNode* current = xorHead;
    XORNode* prev = NULL;
    XORNode* next;

    printf("Activity History: \n");
    while (current != NULL) {
        printf("%s -> ", current->value);
        next = XOR(prev, current->both); // Get the next node using XOR
        prev = current;
        current = next;
    }
    printf("NULL\n");
}

// Function to initialize the XOR linked list
void initializeXORList() {
    xorHead = NULL;  // Initialize head as NULL
}

// Function to initialize the sparse matrix
void initializeSparseMatrix() {
    activityMatrix.size = 0;  // Initialize size to zero
}

// Function to initialize the stack
void initializeStack() {
    activityStack.top = -1;  // Stack is empty
}

// Function to initialize the queue
void initializeQueue() {
    activityQueue.front = 0;
    activityQueue.rear = 0;  // Queue is empty
}

// Function to check if stack is full
bool isStackFull() {
    return activityStack.top == STACK_SIZE - 1;
}

// Function to check if stack is empty
bool isStackEmpty() {
    return activityStack.top == -1;
}

// Function to push activity onto stack
void pushStack(const char* activity) {
    if (!isStackFull()) {
        strcpy(activityStack.items[++activityStack.top], activity);
    }
    else {
        printf("Error: Stack is full!\n");
    }
}

// Function to pop activity from stack
void popStack() {
    if (!isStackEmpty()) {
        printf("Popped activity: %s\n", activityStack.items[activityStack.top--]);
    }
    else {
        printf("Error: Stack is empty!\n");
    }
}

// Function to check if queue is full
bool isQueueFull() {
    return activityQueue.rear == QUEUE_SIZE;
}

// Function to check if queue is empty
bool isQueueEmpty() {
    return activityQueue.front == activityQueue.rear;
}

// Function to enqueue activity into queue
void enqueue(const char* activity) {
    if (!isQueueFull()) {
        strcpy(activityQueue.items[activityQueue.rear++], activity);
    }
    //else {
    //    printf("Error: Queue is full!\n");
    //}
}

// Function to dequeue activity from queue
void dequeue() {
    if (!isQueueEmpty()) {
        printf("Dequeued activity: %s\n", activityQueue.items[activityQueue.front++]);
    }
    else {
        printf("Error: Queue is empty!\n");
    }
}

// Function to add an activity to the sparse matrix
void addActivityToMatrix(int row, int col, char* activity) {
    // Remove the newline character if it exists
    activity[strcspn(activity, "\n")] = 0;

    if (activityMatrix.size < MAX_SIZE) {
        activityMatrix.row[activityMatrix.size] = row;
        activityMatrix.col[activityMatrix.size] = col;
        strcpy(activityMatrix.value[activityMatrix.size], activity);
        activityMatrix.size++;

        // Push to stack and enqueue
        pushStack(activity);
        enqueue(activity);
        // Add to XOR linked list
        addToXORList(activity);  // Adding activity to XOR linked list
    }
    else {
        printf("Error: Sparse matrix is full!\n");
    }
}

// Function to display the activities in the sparse matrix
void displayActivities() {
    printf("Activities in Sparse Matrix:\n");
    for (int i = 0; i < activityMatrix.size; i++) {
        printf("Row: %d, Column: %d, Activity: %s\n",
            activityMatrix.row[i], activityMatrix.col[i], activityMatrix.value[i]);
    }
    printf("Press Enter to continue...\n");
    getchar();  // Wait for user to press Enter
}

void planTimelines() {
    char timeline[100];  // Buffer for input
    printf("Enter the timeline details (e.g., start and end dates): ");
    fgets(timeline, sizeof(timeline), stdin);  // Get input from user
    printf("Timeline planned: %s\n", timeline);  // Show entered timeline
    printf("Press Enter to continue...\n");
    getchar();  // Wait for user to press Enter
}

void organizeActivities() {
    char activity[100];  // Buffer for input
    int row, col;
    printf("Enter the row index for the activity: ");
    scanf("%d", &row);
    printf("Enter the column index for the activity: ");
    scanf("%d", &col);
    getchar();  // Clear the buffer

    printf("Enter the activity details: ");
    fgets(activity, sizeof(activity), stdin);  // Get input from user

    // Add the activity to the sparse matrix
    addActivityToMatrix(row, col, activity);
    printf("Activity organized: %s\n", activity);  // Show entered activity
    printf("Press Enter to continue...\n");
    getchar();  // Wait for user to press Enter
}
// Function to display the schedule submenu
bool schedule() {
    int choice;
    while (true) {
        printf("----------- Schedule Menu -----------\n");
        printf("1. Plan Timelines\n");
        printf("2. Organize Activities\n");
        printf("3. Display Activities\n");  // New option to display activities
        printf("4. Display Activity History\n");  // Updated option to display XOR activities
        printf("5. Pop Activity from Stack\n");  // New option to pop activity from stack
        printf("6. Dequeue Activity\n");  // New option to dequeue activity
        printf("7. Return to Main Menu\n");
        printf("Please enter your choice: ");

        // Prompt the user to make a choice
        scanf("%d", &choice);
        getchar();  // Clear the buffer

        switch (choice) {
        case 1:
            planTimelines();  // Plan timelines
            break;
        case 2:
            organizeActivities();  // Organize activities
            break;
        case 3:
            displayActivities();  // Display activities
            break;
        case 4:
            displayXORList();  // Display activity history from XOR linked list
            break;
        case 5:
            popStack();  // Pop activity from stack
            break;
        case 6:
            dequeue();  // Dequeue activity
            break;
        case 7:
            return false; // Return to Main Menu
        default:
            printf("Invalid choice. Please try again.\n");
            return 0;
        }
    }

    return true; // Added return statement here
}


// Function to heapify a subtree with root at given index
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

// Function to perform heap sort
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}
#define MAX_FEEDBACKS 4
#define MAX_KEYS 3  // Maximum number of keys for B+ tree

int feedbackRatings[MAX_FEEDBACKS];
int feedbackCount = 0;

// Global variables required for SCC
int discoveryTime[MAX_FEEDBACKS], lowLink[MAX_FEEDBACKS];
bool inStack[MAX_FEEDBACKS];
int stack[MAX_FEEDBACKS], stackTop = -1, timeCounter = 0;

// B+ 
typedef struct BPlusLeafNode {
    int keys[MAX_KEYS];
    struct BPlusLeafNode* next;
    int numKeys;
} BPlusLeafNode;

// B+ 
typedef struct BPlusInternalNode {
    int keys[MAX_KEYS];
    void* children[MAX_KEYS + 1];
    int numKeys;
} BPlusInternalNode;

// B+
typedef struct BPlusTree {
    void* root;  // Can be root, internal node or leaf
} BPlusTree;


BPlusTree* createBPlusTree() {
    BPlusTree* tree = (BPlusTree*)malloc(sizeof(BPlusTree));
    //if (tree == NULL) {
    //    perror("Failed to allocate memory for BPlusTree");
    //    exit(EXIT_FAILURE); // Bellek tahsisi baþarýsýzsa programý durdur
    //}
    BPlusLeafNode* rootLeaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
    //if (rootLeaf == NULL) {
    //    perror("Failed to allocate memory for BPlusLeafNode");
    //    exit(EXIT_FAILURE); // Bellek tahsisi baþarýsýzsa programý durdur
    //}
    rootLeaf->numKeys = 0;
    rootLeaf->next = NULL;
    tree->root = rootLeaf;
    return tree;
}


// Add a key to a leaf node
void insertIntoLeaf(BPlusLeafNode* leaf, int key) {
    if (leaf->numKeys < MAX_KEYS) {
        int i = leaf->numKeys - 1;
        while (i >= 0 && leaf->keys[i] > key) {
            leaf->keys[i + 1] = leaf->keys[i];
            i--;
        }
        leaf->keys[i + 1] = key;
        leaf->numKeys++;
    }
    else {
        // Detach a leaf node
        BPlusLeafNode* newLeaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
        newLeaf->numKeys = 0;
        newLeaf->next = leaf->next;
        leaf->next = newLeaf;

        int mid = MAX_KEYS / 2;
        newLeaf->numKeys = MAX_KEYS - mid;
        for (int i = 0; i < newLeaf->numKeys; i++) {
            newLeaf->keys[i] = leaf->keys[mid + i];
        }
        leaf->numKeys = mid;

        // Adding a new key
        if (key > newLeaf->keys[0]) {
            insertIntoLeaf(newLeaf, key);
        }
        else {
            insertIntoLeaf(leaf, key);
        }
    }
}

// Add key to B + tree
void insert(BPlusTree* tree, int key) {
    BPlusLeafNode* root = (BPlusLeafNode*)tree->root;
    if (root->numKeys < MAX_KEYS) {
        insertIntoLeaf(root, key);
    }
    else {
        //Creating new roots and separating leaves
        BPlusInternalNode* newRoot = (BPlusInternalNode*)malloc(sizeof(BPlusInternalNode));
        newRoot->numKeys = 1;
        newRoot->keys[0] = root->keys[MAX_KEYS / 2];
        newRoot->children[0] = root;
        BPlusLeafNode* newLeaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
        newLeaf->numKeys = 0;
        newLeaf->next = NULL;
        newRoot->children[1] = newLeaf;

        tree->root = newRoot;
        insertIntoLeaf(newLeaf, key);
    }
}

// Helper functions for SCC
void pushStackSCC(int node) {
    stack[++stackTop] = node;
    inStack[node] = true;
}

int popStackSCC() {
    int node = stack[stackTop--];
    inStack[node] = false;
    return node;
}

// SCC (Tarjan's Algorithm)
void SCC(int node, int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS], int n) {
    discoveryTime[node] = lowLink[node] = ++timeCounter;
    pushStackSCC(node);

    for (int i = 0; i < n; i++) {
        if (adjMatrix[node][i]) {
            if (discoveryTime[i] == -1) {
                SCC(i, adjMatrix, n);
                lowLink[node] = (lowLink[node] < lowLink[i]) ? lowLink[node] : lowLink[i];
            }
            else if (inStack[i]) {
                lowLink[node] = (lowLink[node] < discoveryTime[i]) ? lowLink[node] : discoveryTime[i];
            }
        }
    }

    if (lowLink[node] == discoveryTime[node]) {
        printf("SCC Found: ");
        int w;
        do {
            w = popStackSCC();
            printf("Feedback %d (Rating: %d) ", w + 1, feedbackRatings[w]);
        } while (w != node);
        printf("\n");
    }
}

//Feedback collection function
void gatherFeedbacks(BPlusTree* tree) {
    char feedback[256];
    int rating;

    printf("Enter your feedback (max 255 characters): ");
    fgets(feedback, sizeof(feedback), stdin);
    feedback[strcspn(feedback, "\n")] = 0;

    printf("Rate the application (1 to 5): ");
    scanf("%d", &rating);
    getchar();

    if (rating >= 1 && rating <= 5) {
        feedbackRatings[feedbackCount++] = rating;
        insert(tree, rating);
        printf("Feedback received: %s\n", feedback);
        printf("Rating received: %d\n", rating);
    }
    else {
        printf("Invalid rating. Please enter a value between 1 and 5.\n");
    }

    printf("Press Enter to continue...\n");
    getchar();
}

// Degree sorting function
void displaySortedRatings() {
    if (feedbackCount == 0) {
        printf("No ratings available.\n");
        return;
    }

    int sortedRatings[MAX_FEEDBACKS];
    memcpy(sortedRatings, feedbackRatings, feedbackCount * sizeof(int)); // Copy degrees

    // Bubble sort
    for (int i = 0; i < feedbackCount - 1; i++) {
        for (int j = 0; j < feedbackCount - i - 1; j++) {
            if (sortedRatings[j] > sortedRatings[j + 1]) {
                int temp = sortedRatings[j];
                sortedRatings[j] = sortedRatings[j + 1];
                sortedRatings[j + 1] = temp;
            }
        }
    }

    printf("Sorted Ratings:\n");
    for (int i = 0; i < feedbackCount; i++) {
        printf("%d ", sortedRatings[i]);
    }
    printf("\nPress Enter to return to Feedback Menu...\n");
    getchar();
}

// BFS function
void BFS(int startNode, int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS], int n) {
    int queue[MAX_FEEDBACKS], front = 0, rear = 0;
    int visited[MAX_FEEDBACKS] = { 0 };

    queue[rear++] = startNode;
    visited[startNode] = 1;

    printf("BFS Traversal starting from Feedback %d:\n", startNode + 1);
    while (front < rear) {
        int node = queue[front++];
        printf("Visited Feedback %d with Rating %d\n", node + 1, feedbackRatings[node]);

        for (int i = 0; i < n; i++) {
            if (adjMatrix[node][i] && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
}

// DFS function
void DFS(int node, int visited[MAX_FEEDBACKS], int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS], int n) {
    visited[node] = 1;
    printf("Visited Feedback %d with Rating %d\n", node + 1, feedbackRatings[node]);

    for (int i = 0; i < n; i++) {
        if (adjMatrix[node][i] && !visited[i]) {
            DFS(i, visited, adjMatrix, n);
        }
    }
}

// Print leaf nodes and their contents in a B+ tree
void printLeafNodes(BPlusTree* tree) {
    BPlusLeafNode* current = (BPlusLeafNode*)tree->root;
    while (current != NULL) {
        printf("Leaf Node: ");
        for (int i = 0; i < current->numKeys; i++) {
            printf("%d ", current->keys[i]);
        }
        printf("\n");
        current = current->next;
    }
}

// Feedback
bool feedback() {
    BPlusTree* tree = createBPlusTree();
    int choice;
    while (1) {
        printf("\n----------- Feedback Menu -----------\n");
        printf("1. Gather Feedback\n");
        printf("2. View Sorted Ratings\n");
        printf("3. Print B+ Tree\n");
        printf("4. Perform BFS\n");
        printf("5. Perform DFS\n");
        printf("6. Find SCC (Tarjan Algorithm)\n");
        printf("7. Return to Main Menu\n");
        printf("Please enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            gatherFeedbacks(tree);
            break;
        case 2:
            displaySortedRatings();
            break;
        case 3:
            printf("Feedbacks stored in B+ Tree:\n");
            printLeafNodes(tree);
            break;
        case 4: {
            int startNode;
            printf("Enter starting feedback number for BFS (1 to %d): ", feedbackCount);
            scanf("%d", &startNode);
            getchar();

            if (startNode < 1 || startNode > feedbackCount) {
                printf("Invalid node.\n");
                break;
            }
            startNode--;

            int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 };
            adjMatrix[0][1] = adjMatrix[1][0] = 1;
            adjMatrix[1][2] = adjMatrix[2][1] = 1;

            BFS(startNode, adjMatrix, feedbackCount);
        } break;
        case 5: {
            int startNode;
            printf("Enter starting feedback number for DFS (1 to %d): ", feedbackCount);
            scanf("%d", &startNode);
            getchar();

            if (startNode < 1 || startNode > feedbackCount) {
                printf("Invalid node.\n");
                break;
            }
            startNode--;

            int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 };
            adjMatrix[0][1] = adjMatrix[1][0] = 1;
            adjMatrix[1][2] = adjMatrix[2][1] = 1;

            int visited[MAX_FEEDBACKS] = { 0 };
            DFS(startNode, visited, adjMatrix, feedbackCount);
        } break;
        case 6: {
            int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 };
            adjMatrix[0][1] = adjMatrix[1][0] = 1;
            adjMatrix[1][2] = adjMatrix[2][1] = 1;

            memset(discoveryTime, -1, sizeof(discoveryTime));
            memset(lowLink, -1, sizeof(lowLink));
            memset(inStack, false, sizeof(inStack));
            stackTop = -1;
            timeCounter = 0;

            printf("Finding SCCs:\n");
            for (int i = 0; i < feedbackCount; i++) {
                if (discoveryTime[i] == -1) {
                    SCC(i, adjMatrix, feedbackCount);
                }
            }
        } break;
        case 7:
            return false;
        default:
            printf("Invalid choice. Try again.\n");
        }
        return 0;
    }
    return true;
}