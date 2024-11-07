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
#define MAX_TREE_NODES 256

// Define the structure for an Attendee
typedef struct Attendee {
    char nameAttendee[50];
    char surnameAttendee[50];
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
    FILE* file = fopen("users.bin", "wb");
    if (file == NULL) {
        perror("An error occurred while opening the file");
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
    FILE* file = fopen("users.bin", "rb");
    if (file == NULL) {
        perror("An error occurred while opening the file");
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
    printf("----------- Authentication Menu -----------\n"); 
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
            mainMenu();
        }
        else {
            clear_screen();
            printf("Invalid login. Returning to main menu.\n");
             mainMenu();
        }
        break;
    case 3:
        guest(); 
        mainMenu();
        break;
    default:
        clear_screen(); 
        printf("Invalid choice. Please try again.\n");
        mainMenu();
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
     mainMenu();
     return true;
}

// Giriþ iþlemi
bool logIn() {
    char phone[20];
    char password[20];
    printf("Enter your phone number: ");
    scanf("%s", phone);
    printf("Enter your password: ");
    scanf("%s", password);

    validateLogin(phone, password);
    return true;
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
    mainMenu();
    return true;
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
            mainMenu(); // Ensure mainMenu() returns bool if needed
            break;
        case 4:
            mainMenu(); // Ensure mainMenu() returns bool if needed
        default:
            printf("Invalid choice. Please try again.\n");
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
        mainMenu();
    }
    return true;
}

#define MAX_ATTENDEES 100
#define MAX_NAME_LENGTH 50

Attendee attendees[MAX_ATTENDEES];
int attendeeCount = 0;

// Graph Representation for SCC (adjacency matrix)
int graph[MAX_ATTENDEES][MAX_ATTENDEES] = { 0 };  // 0 means no edge, 1 means edge exists

// B+ Tree Definitions
#define ORDER 3  // Define the order of the B+ Tree (max children per node)

typedef struct BPlusTreeNode {
    bool isLeaf;
    int numKeys;
    char* keys[ORDER];
    struct BPlusTreeNode* children[ORDER + 1];
} BPlusTreeNode;

BPlusTreeNode* root = NULL;

// Function prototypes
void kmpSearch(char* pattern);
void computeLPSArray(char* pattern, int M, int* lps);
void compressAttendeeName(Attendee* attendee);
bool registerAttendees();
void printAttendees();
void insertToBPlusTree(char* key);
BPlusTreeNode* createBPlusTreeNode(bool isLeaf);
void printBPlusTree(BPlusTreeNode* node);

// SCC Function Prototypes
void dfs(int v, bool visited[], int component[], int* componentIndex);
void dfsUtil(int v, bool visited[], int* order);
void bfs(int startNode, bool visited[]);

void findStronglyConnectedComponents();

// Knuth-Morris-Pratt (KMP) search function
void kmpSearch(char* pattern) {
    int M = strlen(pattern);
    bool found = false;

    for (int i = 0; i < attendeeCount; i++) {
        // Perform KMP search on the Huffman code of each attendee
        char* huffmanCode = attendees[i].huffmanCode;
        int N = strlen(huffmanCode);

        int* lps = new int[M];  // Dinamik bellek ayýrma

        computeLPSArray(pattern, M, lps);

        int j = 0;  // index for pattern
        for (int i = 0; i < N; i++) {
            while (j > 0 && pattern[j] != huffmanCode[i]) {
                j = lps[j - 1];
            }

            if (pattern[j] == huffmanCode[i]) {
                j++;
            }

            if (j == M) {
                printf("Pattern found in Huffman code of attendee: %s %s\n", attendees[i].nameAttendee, attendees[i].surnameAttendee);
                found = true;
                break;
            }
        }
    }

    if (!found) {
        printf("No match found.\n");
    }
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
    // For the sake of this example, we will just encode the name as a simple placeholder.
    // In real scenarios, we should implement an actual Huffman compression function.
    strcpy(attendee->huffmanCode, attendee->nameAttendee);  // Example: Huffman code is just the name for now
}

// Main function for registering attendees
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
        scanf("%s", attendees[attendeeCount].nameAttendee);
        printf("Enter the surname of attendee %d: ", i + 1);
        scanf("%s", attendees[attendeeCount].surnameAttendee);
        compressAttendeeName(&attendees[attendeeCount]); // Compress name and store Huffman code
        insertToBPlusTree(attendees[attendeeCount].nameAttendee); // Insert attendee name into B+ Tree
        attendeeCount++;
    }
    printf("%d attendees registered.\n", count);
    mainMenu();
    return true;
}

// Function to print all registered attendees and their Huffman codes
void printAttendees() {
    printf("\nRegistered Attendees:\n");
    for (int i = 0; i < attendeeCount; i++) {
        printf("Name: %s, Surname: %s, Huffman Code: %s\n", attendees[i].nameAttendee, attendees[i].surnameAttendee, attendees[i].huffmanCode);
    }
    mainMenu();
}

// Function to create a B+ Tree node
BPlusTreeNode* createBPlusTreeNode(bool isLeaf) {
    BPlusTreeNode* newNode = (BPlusTreeNode*)malloc(sizeof(BPlusTreeNode));
    newNode->isLeaf = isLeaf;
    newNode->numKeys = 0;
    for (int i = 0; i < ORDER; i++) {
        newNode->keys[i] = NULL;
        newNode->children[i] = NULL;
    }
    newNode->children[ORDER] = NULL;
    return newNode;
}

// Function to insert a key into the B+ Tree
void insertToBPlusTree(char* key) {
    if (root == NULL) {
        root = createBPlusTreeNode(true);
    }

    // Insert into the B+ Tree (this would require splitting nodes as per B+ Tree insertion logic)
    // Simplified insertion for now
    BPlusTreeNode* currentNode = root;

    if (currentNode->numKeys < ORDER - 1) {
        currentNode->keys[currentNode->numKeys] = key;
        currentNode->numKeys++;
    }
    else {
        // Node split logic for when the node is full
        printf("Node split logic needed here.\n");
    }
}

// Function to print B+ Tree (In-order traversal)
void printBPlusTree(BPlusTreeNode* node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < node->numKeys; i++) {
        printf("%s ", node->keys[i]);
    }
    printf("\n");

    if (node->isLeaf) {
        return;
    }

    for (int i = 0; i <= node->numKeys; i++) {
        printBPlusTree(node->children[i]);
    }
}

// DFS algorithm (Depth-First Search)
void dfs(int v, bool visited[], int component[], int* componentIndex) {
    visited[v] = true;
    component[*componentIndex] = v;
    (*componentIndex)++;

    for (int i = 0; i < attendeeCount; i++) {
        // Here, you should add the edges in your graph.
       // graph[v][i] == 1 indicates the presence of an edge between v and i.

        if (graph[v][i] == 1 && !visited[i]) {
            dfs(i, visited, component, componentIndex);
        }
    }
}

// DFS for topological sorting
void dfsUtil(int v, bool visited[], int* order) {
    visited[v] = true;

    for (int i = 0; i < attendeeCount; i++) {
        if (graph[v][i] == 1 && !visited[i]) {
            dfsUtil(i, visited, order);
        }
    }

    order[(*order)] = v;
    (*order)++;
}


// BFS algorithm (Breadth-First Search)
void bfs(int startNode, bool visited[]) {
    int queue[MAX_ATTENDEES];
    int front = 0, rear = 0;

    visited[startNode] = true;
    queue[rear++] = startNode;

    while (front < rear) {
        int currentNode = queue[front++];  // Remove a node from the queue
        printf("Visiting node: %d\n", currentNode);

        for (int i = 0; i < attendeeCount; i++) {
            // Here, you should add the edges in your graph.
            // graph[currentNode][i] == 1 indicates the presence of an edge between currentNode and i.

            if (graph[currentNode][i] == 1 && !visited[i]) {
                visited[i] = true;
                queue[rear++] = i;  // Add a new node to the queue

            }
        }
    }
}


// Finding Strongly Connected Components

void findStronglyConnectedComponents() {
    bool visited[MAX_ATTENDEES] = { 0 };
    int component[MAX_ATTENDEES];
    int componentIndex = 0;

    // After filling the graph, the DFS function will run.
    // After filling your graph, you can find the strongly connected components with this function.

    for (int i = 0; i < attendeeCount; i++) {
        if (!visited[i]) {
            dfs(i, visited, component, &componentIndex);

            printf("SCC: ");
            for (int j = 0; j < componentIndex; j++) {
                printf("%d ", component[j]);
            }
            printf("\n");
            componentIndex = 0;
        }
    }
}

// Main attendee menu function
bool attendee() {
    int choice;
    printf("----------- Attendee Menu -----------\n");
    printf("1. Register Attendees\n");
    printf("2. Track Attendees\n");
    printf("3. Print Attendees\n");
    printf("4. Return to Main Menu\n");
    printf("Please enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        // Call the function to register attendees
        registerAttendees();
        break;
    case 2: {
        char searchName[50];
        printf("Enter the name to search: ");
        scanf("%s", searchName);
        // Search for the attendee's name in the Huffman code
        kmpSearch(searchName);
        mainMenu();  // Call main menu after search
        break;
    }
    case 3:
        // Print all the attendees
        printAttendees();
        break;

    case 4:

        return mainMenu (); // Return to Main Menu

    default:
        // Handle invalid choices
        printf("Invalid choice. Please try again.\n");
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
    else {
        printf("Error: Queue is full!\n");
    }
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
            return mainMenu (); // Return to Main Menu
        default:
            printf("Invalid choice. Please try again.\n");
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

// Function to gather feedback
void gatherFeedbacks() {
    clear_screen();
    char feedback[256];  // Buffer for input
    printf("Enter your feedback (max 255 characters): ");
    fgets(feedback, sizeof(feedback), stdin);  // Get input from user
    feedback[strcspn(feedback, "\n")] = 0;  // Remove newline character
    printf("Feedback received: %s\n", feedback);  // Show entered feedback
    printf("Press Enter to continue...\n");
    getchar();  // Wait for user to press Enter
}

// Function to display the feedback submenu
bool feedback() {
    int choice;
    while (true) {
        clear_screen();  // Clear the console
        printf("----------- Feedback Menu -----------\n");
        printf("1. Gather Feedbacks\n");
        printf("2. Return to Main Menu\n");
        printf("Please enter your choice: ");

        // Prompt the user to make a choice
        scanf("%d", &choice);
        getchar();  // Clear the buffer

        switch (choice) {
        case 1:
            gatherFeedbacks();  // Gather feedbacks
            break;
        case 2:
            return mainMenu() ;  // Return to main menu
        default:
            clear_screen();
            printf("Invalid choice. Please try again.\n");
            // Invalid input, continue the loop
        }
    }
    return true ;
}

