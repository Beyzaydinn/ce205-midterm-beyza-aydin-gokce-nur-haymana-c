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

/**
 * @brief Clears the console screen for better user interface experience.
 *
 * This function is used to clear the console screen, ensuring a clean
 * and organized output environment during program execution. It employs
 * platform-specific system commands to achieve this functionality.
 *
 * ## Platform-Specific Behavior:
 * - **Windows**: Executes the `cls` command via the `system()` function
 *   to clear the console.
 * - **Linux and WSL (Windows Subsystem for Linux)**: Executes the `clear`
 *   command via the `system()` function to achieve the same result.
 *
 * @warning The use of the `system()` function may pose security risks
 *          if untrusted input is passed to it. However, this implementation
 *          is safe as it uses fixed commands.
 *
 * @note This function is particularly useful in applications where dynamic
 *       console output is required, such as menu-driven programs or real-time
 *       updates.
 *
 * @see system() for more details about executing system-specific commands.
 */

void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");  // for Windows
#elif defined(linux)
    system("clear");  // for Linux and WSL
#endif
}


/**
 * @brief Defines the size of a hash table.
 *
 * This macro specifies the number of elements that the hash table can hold.
 */
#define TABLE_SIZE 100

 /**
  * @brief Defines the maximum number of nodes in a tree.
  *
  * This macro sets the upper limit for the number of nodes that can be managed in a tree structure.
  */
#define MAX_TREE_NODES 256

  /**
   * @brief Structure to represent an attendee.
   *
   * This structure stores details of an attendee, including their first name,
   * last name, and associated Huffman code.
   */
typedef struct AttendeE {
    char nameAttendee[50];      /**< First name of the attendee. */
    char surnameAttendee[50];   /**< Last name of the attendee. */
    char huffmanCode[256];      /**< Huffman code associated with the attendee. */
} AttendeE;

/**
 * @brief Structure for a node in the Huffman tree.
 *
 * This structure represents a node in the Huffman tree, including its character data,
 * frequency, and pointers to left and right child nodes.
 */
typedef struct MinHeapNode {
    char data;                   /**< Character stored in the node. */
    unsigned freq;               /**< Frequency of the character. */
    struct MinHeapNode* left;    /**< Pointer to the left child node. */
    struct MinHeapNode* right;   /**< Pointer to the right child node. */
} MinHeapNode;

/**
 * @brief Structure for a Min Heap.
 *
 * This structure represents a Min Heap, which is used in the Huffman coding process.
 * It contains the current size, maximum capacity, and an array of pointers to MinHeapNode elements.
 */
typedef struct MinHeap {
    unsigned size;               /**< Current size of the Min Heap. */
    unsigned capacity;           /**< Maximum capacity of the Min Heap. */
    MinHeapNode** array;         /**< Array of pointers to MinHeapNode elements. */
} MinHeap;

/**
 * @brief Creates a Min Heap with the specified capacity.
 *
 * This function dynamically allocates memory for a Min Heap structure,
 * initializes its size to 0, and allocates memory for the array of
 * MinHeapNode pointers based on the specified capacity.
 *
 * @param capacity The maximum capacity of the Min Heap.
 * @return A pointer to the newly created MinHeap structure.
 *
 * @note If the specified capacity is 0 or less, the array pointer is set to nullptr.
 *       Ensure to free the allocated memory after usage.
 */
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

/**
 * @brief Creates a new Min Heap Node.
 *
 * This function dynamically allocates memory for a Min Heap Node,
 * initializes it with the provided character data and frequency,
 * and sets its child pointers to NULL.
 *
 * @param data The character to be stored in the node.
 * @param freq The frequency associated with the character.
 * @return A pointer to the newly created MinHeapNode.
 *
 * @note Ensure to free the allocated memory for the node after usage.
 */
MinHeapNode* createMinHeapNode(char data, unsigned freq) {
    MinHeapNode* newNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = newNode->right = NULL;
    return newNode;
}

/**
 * @brief Inserts a node into the Min Heap.
 *
 * This function adds a given Min Heap Node to the Min Heap's array
 * and increments the size of the Min Heap. It assumes that the Min Heap
 * has sufficient capacity to accommodate the new node.
 *
 * @param minHeap Pointer to the MinHeap where the node will be inserted.
 * @param minHeapNode Pointer to the MinHeapNode to be inserted.
 *
 * @note Ensure that the Min Heap has sufficient capacity before calling this function.
 */
void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
    minHeap->array[minHeap->size] = minHeapNode;
    minHeap->size++;
}

/**
 * @brief Extracts the minimum node from the Min Heap.
 *
 * This function removes the node with the smallest frequency from the Min Heap.
 * The last node in the heap is moved to the root position, and the size of the Min Heap is decremented.
 *
 * @param minHeap Pointer to the MinHeap from which the minimum node will be extracted.
 * @return A pointer to the extracted MinHeapNode containing the smallest frequency.
 *
 * @note This function assumes that the Min Heap is not empty. It does not perform
 *       heapify operations, so maintaining heap properties is the caller's responsibility.
 */
MinHeapNode* extractMin(MinHeap* minHeap) {
    MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    return temp;
}

/**
 * @brief Searches for occurrences of a pattern in a given text using the KMP algorithm.
 *
 * This function performs a Knuth-Morris-Pratt (KMP) search for a given pattern in a predefined text.
 * It uses the LPS (Longest Prefix Suffix) array for efficient searching.
 *
 * @param pattern The pattern to search for.
 */
void kmpSearch(char* pattern);

/**
 * @brief Computes the Longest Prefix Suffix (LPS) array for a given pattern.
 *
 * This function prepares the LPS array, which is used in the KMP search algorithm to avoid unnecessary comparisons.
 *
 * @param pattern The pattern for which to compute the LPS array.
 * @param M Length of the pattern.
 * @param lps Pointer to the array where the LPS values will be stored.
 */
void computeLPSArray(char* pattern, int M, int* lps);

/**
 * @brief Compresses the name of an attendee using Huffman encoding.
 *
 * This function generates a Huffman code for the name of the attendee and stores it in the attendee's structure.
 *
 * @param attendee Pointer to the AttendeE structure containing the attendee's name.
 */
void compressAttendeeName(AttendeE* attendee);

/**
 * @brief Builds a Huffman tree from a string and stores the resulting codes in an attendee structure.
 *
 * This function constructs a Huffman tree for the given string and associates the generated codes with an attendee.
 *
 * @param str The string for which the Huffman tree is built.
 * @param attendee Pointer to the AttendeE structure to store the Huffman codes.
 */
void buildHuffmanTree(char* str, AttendeE* attendee);

/**
 * @brief Creates a Min Heap with the specified capacity.
 *
 * @param capacity The maximum capacity of the Min Heap.
 * @return A pointer to the newly created MinHeap structure.
 */
MinHeap* createMinHeap(unsigned capacity);

/**
 * @brief Creates a new Min Heap Node.
 *
 * @param data The character to be stored in the node.
 * @param freq The frequency associated with the character.
 * @return A pointer to the newly created MinHeapNode.
 */
MinHeapNode* createMinHeapNode(char data, unsigned freq);

/**
 * @brief Inserts a node into the Min Heap.
 *
 * @param minHeap Pointer to the MinHeap where the node will be inserted.
 * @param minHeapNode Pointer to the MinHeapNode to be inserted.
 */
void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode);

/**
 * @brief Extracts the minimum node from the Min Heap.
 *
 * @param minHeap Pointer to the MinHeap from which the minimum node will be extracted.
 * @return A pointer to the extracted MinHeapNode containing the smallest frequency.
 */
MinHeapNode* extractMin(MinHeap* minHeap);

/**
 * @brief Generates Huffman codes from a Huffman tree.
 *
 * This function traverses the Huffman tree to generate codes for characters and
 * stores the generated codes in the provided Huffman code string.
 *
 * @param root Pointer to the root node of the Huffman tree.
 * @param code Temporary array to store the current code during traversal.
 * @param top Index for the current position in the code array.
 * @param huffmanCode String to store the final generated Huffman code.
 */
void generateHuffmanCodes(MinHeapNode* root, char* code, int top, char* huffmanCode);

/**
 * @brief Prints the list of attendees and their Huffman codes.
 *
 * This function outputs the details of all attendees, including their names and Huffman codes.
 */
void printAttendees();

/**
 * @brief Executes the Progressive Overflow Algorithm (POA).
 *
 * This function implements a Progressive Overflow Algorithm (POA) for an unspecified task.
 */
void progressiveOverflowAlgorithm();


/**
 * @brief Structure to represent a user.
 *
 * This structure contains the user's details, including their name, surname,
 * phone number, password, and a pointer to the next user for resolving collisions
 * in the hash table using a linked list.
 */
typedef struct User {
    char name[50];               /**< User's first name. */
    char surname[50];            /**< User's last name. */
    char phone[20];              /**< User's phone number. */
    char password[20];           /**< User's password. */
    struct User* next;           /**< Pointer to the next user in case of collisions. */
} User;

/**
 * @brief Hash table for storing user records.
 *
 * This array of pointers is used to implement a hash table with a size defined by TABLE_SIZE.
 * Each index in the array points to a linked list of User structures to handle collisions.
 */
User* hashTable[TABLE_SIZE];

/**
 * @brief Structure to represent an event.
 *
 * This structure contains details about an event, including its type,
 * date, color, and concept. It also contains pointers to the previous
 * and next events, allowing for a doubly linked list implementation.
 */
typedef struct Event {
    char type[50];              /**< Type of the event. */
    char date[20];              /**< Date of the event. */
    char color[20];             /**< Color associated with the event. */
    char concept[50];           /**< Concept or theme of the event. */
    struct Event* prev;         /**< Pointer to the previous event in the list. */
    struct Event* next;         /**< Pointer to the next event in the list. */
} Event;

/**
 * @brief Head pointer of the doubly linked list of events.
 *
 * This pointer points to the first event in the list, allowing for traversal
 * and management of the events.
 */
Event* head = NULL;

/**
 * @brief Tail pointer of the doubly linked list of events.
 *
 * This pointer points to the last event in the list, facilitating operations
 * that require access to the end of the list.
 */
Event* tail = NULL;

/**
 * @brief Computes a hash value for a given phone number.
 *
 * This function calculates a hash value for the input phone number
 * using a simple hash function based on the polynomial accumulation method.
 * The result is then constrained to fit within the defined hash table size.
 *
 * @param phone The phone number to be hashed.
 * @return An unsigned integer representing the hash value of the phone number,
 *         which is in the range of [0, TABLE_SIZE - 1].
 */
unsigned int hash(const char* phone) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(phone); i++) {
        hash = (hash * 31 + phone[i]) % TABLE_SIZE;
    }
    return hash;
}

/**
 * @brief Saves a new user to the hash table.
 *
 * This function inserts a new user into the hash table by calculating the
 * appropriate index based on the user's phone number. It uses a linked list
 * to handle collisions by chaining users at the same index.
 *
 * @param newUser Pointer to the User structure that needs to be saved in the hash table.
 */
void saveUser(User* newUser) {
    unsigned int index = hash(newUser->phone);
    newUser->next = hashTable[index];
    hashTable[index] = newUser;
}

/**
 * @brief Saves the hash table containing user data to a binary file for persistent storage.
 *
 * This function serializes the hash table structure by iterating through each index
 * and writing the user records stored in it to a binary file named `"users.bin"`.
 * If a collision has occurred at a particular index, the function traverses the linked list
 * at that index to ensure all user records are saved.
 *
 * ## Implementation Details:
 * - A binary file is used to store the data for compactness and efficiency.
 * - Each user record is written sequentially using the `fwrite()` function.
 * - Collisions in the hash table are resolved by iterating through the linked list
 *   at each index, ensuring no data is missed.
 *
 * ## Usage Notes:
 * - The function currently does not include active error checking for file operations.
 *   Uncomment the provided error handling block for production-grade reliability.
 *   This will ensure issues such as failure to open the file are properly reported.
 * - The structure of the `User` object must remain consistent between the program
 *   execution and when the data is loaded back from the file.
 *
 * ## Security Warning:
 * - Ensure the binary file is stored securely to prevent unauthorized access to user data.
 * - The function uses raw binary operations, so care should be taken when modifying
 *   the structure of the `User` object.
 *
 * @note This function is part of a larger system that handles hash table-based user
 *       management, enabling features such as persistent data storage and recovery.
 *
 * @see fopen(), fwrite(), fclose()
 */
void saveHashTableToFile() {
    FILE* file = fopen("users.bin", "wb");
    /*
    if (file == NULL) {
        perror("An error occurred while opening the file");
        return;
    }
    */

    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        while (current != NULL) {
            fwrite(current, sizeof(User), 1, file);
            current = current->next;
        }
    }
    fclose(file);
}


/**
 * @brief Loads user records from a binary file into the hash table.
 *
 * This function reads user records from a binary file named "users.bin"
 * and inserts each user into the hash table. It allocates memory for a new
 * User structure for each record read. If less than one record is read,
 * it indicates the end of the file or a read error, and the function will
 * clean up the allocated memory before exiting the loop.
 *
 * Note: Error checking for file opening is commented out. Uncomment the error
 * handling code if needed.
 */
void loadHashTableFromFile() {
    FILE* file = fopen("users.bin", "rb");
    /*
    if (file == NULL) {
        perror("An error occurred while opening the file");
        return;
    }
    */

    while (1) {
        User* newUser = (User*)malloc(sizeof(User));
        // If fread reads less than one, the file may be empty or an error occurred
        if (fread(newUser, sizeof(User), 1, file) != 1) {
            free(newUser); // Clean up memory if end of file or read error occurs
            break; // Exit the loop when the end of the file is reached
        }
        newUser->next = NULL;
        saveUser(newUser);  // Add the user to the hash table
    }

    fclose(file); // Close the file
}

/**
 * @brief Inserts a new user into the hash table using quadratic probing.
 *
 * This function attempts to insert a new user into the hash table at the
 * index calculated from the user's phone number. If the calculated index
 * is occupied, it uses quadratic probing to find the next available slot.
 * If the table is full, it notifies the user and returns false.
 *
 * @param newUser Pointer to the User structure to be inserted into the hash table.
 * @return True if the user was successfully added, false if the hash table is full.
 */
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


/**
 * @brief Saves user data to the hash table and a file.
 *
 * This function takes a User structure as input, allocates memory for a new
 * User, copies the provided user data into it, and then inserts the new user
 * into the hash table. After adding the user to the hash table, it saves the
 * entire hash table to a binary file. The screen is cleared after the operation.
 *
 * @param user The User structure containing the data to be saved.
 */
void saveUserData(User user) {
    User* newUser = (User*)malloc(sizeof(User));
    *newUser = user;
    saveUser(newUser); // Add to hash table
    saveHashTableToFile(); // Save to file
    clear_screen();
}
/**
 * @brief Prints the contents of the hash table.
 *
 * This function iterates through each index of the hash table and prints the
 * user data stored at that index. If an index is empty, it skips to the next
 * index. For each user, it displays their name, surname, phone number, and
 * password. The function ends by indicating the completion of the hash table
 * output.
 */
void printHashTable() {
    /*   if (hashTable == nullptr) {
           printf("Hash table is not initialized.\n");
           return;
       } */

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

/**
 * @brief Constructs a Huffman tree and generates Huffman codes for characters in a given string.
 *
 * This function performs the following steps to create a Huffman tree and generate Huffman codes:
 * 1. Calculates the frequency of each character in the input string.
 * 2. Builds a min-heap (priority queue) where each node represents a character and its frequency.
 * 3. Iteratively merges the two nodes with the smallest frequencies to form the Huffman tree.
 * 4. Traverses the Huffman tree to generate unique binary codes for each character, which are
 *    stored in the provided `AttendeE` structure.
 *
 * ## Parameters:
 * - `str` (char*): The input string for which the Huffman tree is to be built. Each character in
 *   this string is considered for frequency calculation and encoding.
 * - `attendee` (AttendeE*): A pointer to an `AttendeE` structure where the generated Huffman codes
 *   for each character will be stored. The structure is expected to have a `huffmanCode` field for storage.
 *
 * ## Implementation Details:
 * - A frequency array (`freq`) is used to calculate how often each character appears in the string.
 * - A min-heap is constructed to manage nodes efficiently based on their frequency. Nodes with lower
 *   frequencies have higher priority.
 * - The tree is constructed by repeatedly extracting the two smallest nodes from the heap, combining them
 *   into a new node, and reinserting the new node into the heap.
 * - The final node in the heap represents the root of the Huffman tree.
 * - The `generateHuffmanCodes` function recursively traverses the tree to create unique binary codes
 *   for each character.
 *
 * ## Usage Notes:
 * - Ensure the input string is null-terminated to avoid buffer overflows during frequency calculation.
 * - The function assumes that `AttendeE` is properly initialized and capable of storing the resulting
 *   Huffman codes.
 * - Memory management for the min-heap and tree nodes should be handled appropriately to prevent leaks.
 *
 * ## Advantages of Huffman Encoding:
 * - Reduces the average length of encoded strings, especially for strings with skewed character frequencies.
 * - Commonly used in data compression algorithms, such as JPEG and MP3.
 *
 * @warning If the input string contains characters with very high frequencies, the min-heap may grow
 *          large, increasing memory usage and computation time.
 *
 * @see generateHuffmanCodes(), createMinHeap(), insertMinHeap(), extractMin()
 */
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

/**
 * @brief Generates Huffman codes for characters in a Huffman tree.
 *
 * This function traverses the Huffman tree recursively and generates
 * the corresponding Huffman codes for each character. The generated codes
 * are stored in the provided huffmanCode buffer.
 *
 * @param root A pointer to the root of the Huffman tree.
 * @param code A buffer to store the current Huffman code during recursion.
 * @param top The current position in the code buffer.
 * @param huffmanCode A buffer to store the final Huffman codes for each character.
 */
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

/**
 * @brief Validates user login credentials.
 *
 * This function checks if the provided phone number and password
 * match any entry in the hash table. If a match is found, it indicates
 * a successful login; otherwise, it indicates a failure.
 *
 * @param phone The phone number of the user attempting to log in.
 * @param password The password of the user attempting to log in.
 * @return true if the login credentials are valid; false otherwise.
 */
bool validateLogin(const char* phone, const char* password) {
    if (!phone || !password) {
        return false; // Invalid login parameters
    }

    unsigned int index = hash(phone);
    User* current = hashTable[index];
    while (current) {
        if (strcmp(current->phone, phone) == 0 && strcmp(current->password, password) == 0) {
            return true; // Successful login
        }
        current = current->next;
    }
    return false; // Unsuccessful login
}

/**
 * @brief Demonstrates a progressive overflow algorithm.
 *
 * This function iterates through an array of integers, calculating a running sum.
 * If the current sum exceeds a predefined overflow threshold, it prints a message
 * indicating the overflow and resets the current sum while retaining the last
 * processed element in the sum.
 *
 * The algorithm helps in understanding how to handle overflows
 * in data processing where cumulative values are significant.
 */
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
/**
 * @brief Displays the main menu and manages user interactions by executing corresponding features.
 *
 * This function serves as the central hub for the program, presenting users with a menu
 * of available features and allowing them to interact with the system. It handles user
 * input, validates it, and invokes the appropriate functions based on the user's selection.
 *
 * ## Menu Options:
 * - **User Authentication**: Allows users to log in or register.
 * - **Event Details**: Provides options to manage event-related information, such as creating,
 *   editing, or viewing event details.
 * - **Attendee Management**: Enables the management of attendee records, including adding,
 *   updating, or removing attendees.
 * - **Schedule Organizer**: Helps users organize and manage event schedules efficiently.
 * - **Feedback Collection**: Facilitates the collection and management of user feedback.
 * - **Exit**: Terminates the program gracefully when the user chooses to exit.
 *
 * ## Function Flow:
 * - The menu is displayed in a loop to allow continuous interaction until the user selects
 *   the exit option.
 * - Input is taken from the user and validated to ensure that it corresponds to one of the
 *   menu options.
 * - Based on the user's input, the corresponding function is invoked to handle the selected task.
 * - Invalid input is handled gracefully with a prompt to retry.
 *
 * ## Implementation Details:
 * - The menu uses `printf` for displaying options and `scanf` for input, ensuring compatibility
 *   with console-based systems.
 * - A `switch` statement is used for efficient selection of the appropriate functionality.
 * - The function returns `false` if the user chooses to exit the menu; otherwise, it loops back
 *   to display the menu again.
 *
 * ## Usage Notes:
 * - Ensure all sub-functions (e.g., `authentication`, `eventDetails`, etc.) are implemented and
 *   properly integrated with the main menu.
 * - This function is designed for console-based applications; GUI implementations would require
 *   a different approach.
 * - Input validation prevents crashes due to invalid data, but further improvements (e.g.,
 *   sanitizing inputs) may enhance robustness.
 *
 * ## Limitations:
 * - The current implementation uses numeric options; extending it to support descriptive input
 *   (e.g., keywords) may improve user experience.
 * - It relies on a linear flow, which may not scale well for applications with highly complex menus.
 *
 * @return Returns `false` if the user selects the exit option (6); otherwise, it returns `true`
 *         to indicate that the menu should continue displaying.
 *
 * @see authentication(), eventDetails(), attendee(), schedule(), feedback()
 */

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
            authentication(); // you are in authentication menu
            break;
        case 2:
            eventDetails(); // you are in event details menu 
            break;
        case 3:
            attendee(); // you are in attendee menu
            break;
        case 4:
            schedule(); // you are in schedule organizer menu
            break;
        case 5:
            feedback(); // you are in feedback collection menu
            break;
        case 6:
            return false; // return main menu
        default:
            printf("Invalid choice. Please try again.\n"); // if you made a invalid choice
            return false;
        }
    } while (choice != 6);
    return true;
}

/**
 * @brief Executes the Progressive Overflow algorithm.
 *
 * This function demonstrates the Progressive Overflow algorithm by
 * iterating through a predefined array of integers. It calculates
 * the cumulative sum of the elements and checks if the sum exceeds
 * a specified overflow threshold. If an overflow is detected, it
 * prints a message indicating the index at which the overflow
 * occurred and resets the current sum to zero for simplicity.
 *
 * This function serves as a demonstration of how to handle
 * overflow conditions in a sequence of operations.
 */
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


/**
 * @brief Executes the Linear Probing algorithm for hash table insertion.
 *
 * This function demonstrates the Linear Probing collision resolution
 * technique in a hash table. It initializes a hash table of size 10
 * and attempts to insert a predefined set of keys into the table.
 * When a collision occurs, the function searches for the next available
 * index in a linear manner, wrapping around to the start of the table
 * if necessary. If the table is full, it notifies the user that the
 * key cannot be placed.
 *
 * After attempting to place all keys, the function prints the contents
 * of the hash table, showing the index and the corresponding key, or
 * indicating if the index is empty.
 */
void linearProbing() {
    printf("Executing Linear Probing algorithm...\n");
    int hashTable[10];
    for (int i = 0; i < 10; i++) {
        hashTable[i] = -1; 
    }

    int keys[] = { 23, 45, 12, 37, 29 };
    int size = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;
        int startIndex = index;
        bool placed = false; 

        do {
            if (hashTable[index] == -1) {
                hashTable[index] = keys[i];
                placed = true;
                break;
            }
            index = (index + 1) % 10;
        } while (index != startIndex);

        if (!placed) {
            printf("Hash table is full, cannot place key %d\n", keys[i]);
        }
    }

    for (int i = 0; i < 10; i++) {
        if (hashTable[i] != -1) {
            printf("Index %d: %d\n", i, hashTable[i]);
        }
        else {
            printf("Index %d: Empty\n", i);
        }
    }
}

/**
 * @brief Executes the Quadratic Probing algorithm for hash table insertion.
 *
 * This function demonstrates the Quadratic Probing collision resolution
 * technique in a hash table. It initializes a hash table of size 10
 * and attempts to insert a predefined set of keys into the table.
 * When a collision occurs, the function uses a quadratic probing method
 * to find the next available index by incrementing the offset
 * quadratically. The process continues until an empty slot is found
 * for each key.
 *
 * After attempting to place all keys, the function prints the contents
 * of the hash table, showing the index and the corresponding key.
 */
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

/**
 * @brief Executes the Double Hashing algorithm for hash table insertion.
 *
 * This function implements the Double Hashing collision resolution technique in a hash table.
 * It initializes a hash table of size 10 and attempts to insert a predefined set of keys.
 * When a collision occurs, the function computes a second hash value to determine the step size
 * for probing, thus minimizing clustering in the table. The process continues until an empty
 * slot is found for each key.
 *
 * After attempting to place all keys, the function prints the contents of the hash table,
 * showing the index and the corresponding key.
 */
void doubleHashing() {
    printf("Executing Double Hashing algorithm...\n");
    int hashTable[10] = { 0 };  // Hash table initialized to 0
    int keys[] = { 23, 45, 12, 37, 29 };  // Keys to be added
    int size = sizeof(keys) / sizeof(keys[0]);  // Number of keys

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;  // First hash computation for the key
        int step = 7 - (keys[i] % 7);  // Second hash function for double hashing
        while (hashTable[index] != 0) {  // If the slot is occupied
            index = (index + step) % 10;  // Add step to resolve collision
        }
        hashTable[index] = keys[i];  // Place the key in the correct index
    }

    // Print the contents of the hash table
    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d\n", i, hashTable[i]);
    }
}

/**
 * @brief Executes the Use of Buckets algorithm for storing keys.
 *
 * This function implements a simple bucket-based hashing method for storing
 * keys in a multi-dimensional array (buckets). It initializes an array of
 * buckets and inserts a predefined set of keys based on their modulo with
 * respect to the number of buckets.
 *
 * Each key is assigned to a bucket determined by the expression `key % 3`.
 * The function iterates through the selected bucket to find an empty slot
 * (indicated by a value of 0) and places the key there. The process continues
 * until all keys are inserted into their respective buckets.
 *
 * After insertion, the contents of each bucket are printed, showing the keys
 * stored within each one.
 */
void useBuckets() {
    printf("Executing Use of Buckets algorithm...\n");
    int buckets[3][10] = { 0 };  // Initialize buckets (3 buckets, each can hold 10 keys)
    int keys[] = { 23, 45, 12, 37, 29 };  // Keys to be added
    int size = sizeof(keys) / sizeof(keys[0]);  // Number of keys

    for (int i = 0; i < size; i++) {
        int bucketIndex = keys[i] % 3;  // Determine the bucket index
        for (int j = 0; j < 10; j++) {
            if (buckets[bucketIndex][j] == 0) {  // Find an empty slot
                buckets[bucketIndex][j] = keys[i];  // Place the key
                break;  // Exit loop after placing the key
            }
        }
    }

    // Print the contents of each bucket
    for (int i = 0; i < 3; i++) {
        printf("Bucket %d: ", i);
        for (int j = 0; j < 10; j++) {
            if (buckets[i][j] != 0) {
                printf("%d ", buckets[i][j]);  // Print non-empty keys
            }
        }
        printf("\n");
    }
}
/**
 * @brief Executes the Linear Quotient algorithm for storing keys in a hash table.
 *
 * This function implements a linear probing hashing method to handle collisions
 * when inserting keys into a hash table. It initializes a hash table and uses
 * a predefined set of keys for insertion.
 *
 * The algorithm computes the initial index for each key using the modulo operation
 * `key % 10`. If the computed index is already occupied (indicated by a non-zero
 * value in the hash table), the function increments the index linearly (by one)
 * until it finds an empty slot. Once an empty slot is located, the key is
 * inserted into that position.
 *
 * After processing all keys, the contents of the hash table are printed,
 * showing the keys stored at each index.
 */
void linearQuotient() {
    printf("Executing Linear Quotient algorithm...\n");
    int hashTable[10] = { 0 };  // Initialize the hash table with 0
    int keys[] = { 23, 45, 12, 37, 29 };  // Keys to be added
    int size = sizeof(keys) / sizeof(keys[0]);  // Number of keys

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;  // Compute the initial index
        int increment = 1;  // Linear increment for probing
        while (hashTable[index] != 0) {  // Check for collisions
            index = (index + increment) % 10;  // Increment index linearly
        }
        hashTable[index] = keys[i];  // Insert the key into the hash table
    }

    // Print the contents of the hash table
    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d\n", i, hashTable[i]);  // Display each index and its key
    }
}
/**
 * @brief Executes Brent's Method for storing keys in a hash table.
 *
 * This function demonstrates the use of Brent's Method for collision resolution
 * in a hash table. The method is designed to efficiently handle collisions
 * by incrementally exploring potential slots for inserting keys.
 *
 * The function initializes a hash table and a predefined set of keys for insertion.
 * The initial index for each key is calculated using the modulo operation
 * `key % 10`. If the calculated index is already occupied, the method
 * attempts to find the next available slot using Brent's Method, which involves
 * incrementing the step size and exploring further indices in the hash table.
 *
 * However, the core logic for resolving collisions is currently commented out,
 * meaning that if a collision occurs, the keys will not be inserted correctly.
 * Therefore, only the initial indices for keys will be displayed in the output.
 *
 * After processing the keys, the function prints the contents of the hash table,
 * showing the keys stored at each index.
 */

void brentsMethod() {
    printf("Executing Brent's Method algorithm...\n");
    int hashTable[10] = { 0 };  // Initialize the hash table with 0
    int keys[] = { 23, 45, 12, 37, 29 };  // Keys to be added
    int size = sizeof(keys) / sizeof(keys[0]);  // Number of keys

    for (int i = 0; i < size; i++) {
        int index = keys[i] % 10;  // Compute the initial index
        int step = 1;  // Initial step size
        /* while (hashTable[index] != 0) {  // Check for collisions
            int newIndex = (index + step) % 10;  // Compute new index
            if (hashTable[newIndex] == 0) {  // Check if new index is free
                index = newIndex;  // Update index to the new slot
                break;  // Exit the loop if a slot is found
            }
            step++;  // Increment step size for further probing
        } */
        hashTable[index] = keys[i];  // Insert the key into the hash table
    }

    // Print the contents of the hash table
    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d\n", i, hashTable[i]);  // Display each index and its key
    }
}

/**
 * @brief Handles various file operations based on user choice.
 *
 * This function takes a user's choice as input and executes the corresponding
 * hashing algorithm or operation. Each case in the switch statement calls
 * a specific function that implements a particular algorithm or data structure
 * operation related to hash tables or data management.
 *
 * @param choice An integer representing the user's selection from the menu.
 *               The valid choices are as follows:
 *               - 1: Execute Progressive Overflow algorithm
 *               - 2: Execute Linear Probing algorithm
 *               - 3: Execute Quadratic Probing algorithm
 *               - 4: Execute Double Hashing algorithm
 *               - 5: Execute Bucket Usage algorithm
 *               - 6: Execute Linear Quotient algorithm
 *               - 7: Execute Brent's Method algorithm
 *               - 8: Return to the Authentication Menu
 *
 * If an invalid choice is provided, the function will notify the user to
 * try again.
 */
void handleFileOperation(int choice) {
    switch (choice) {
    case 1:
        progressiveOverflow();  // Execute the Progressive Overflow algorithm
        break;
    case 2:
        linearProbing();  // Execute the Linear Probing algorithm
        break;
    case 3:
        quadraticProbing();  // Execute the Quadratic Probing algorithm
        break;
    case 4:
        doubleHashing();  // Execute the Double Hashing algorithm
        break;
    case 5:
        useBuckets();  // Execute the Use of Buckets algorithm
        break;
    case 6:
        linearQuotient();  // Execute the Linear Quotient algorithm
        break;
    case 7:
        brentsMethod();  // Execute Brent's Method algorithm
        break;
    case 8:
        printf("Returning to Authentication Menu.\n");  // Inform the user of return
        break;
    default:
        printf("Invalid choice. Please try again.\n");  // Handle invalid choices
    }
}

/**
 * @brief Displays the File Operations Menu and handles user choices.
 *
 * This function presents a menu of file operations related to various hashing
 * algorithms and data structures. It prompts the user to enter a choice,
 * validates the input, and calls the appropriate handler function for the
 * selected operation. The menu will continue to be displayed until the user
 * chooses to return to the authentication menu.
 *
 * The valid choices are as follows:
 * - 1: Progressive Overflow
 * - 2: Linear Probing
 * - 3: Quadratic Probing
 * - 4: Double Hashing
 * - 5: Use of Buckets
 * - 6: Linear Quotient
 * - 7: Brent's Method
 * - 8: Back to Authentication Menu
 *
 * Upon receiving the choice, the function invokes the `handleFileOperation`
 * function to execute the corresponding operation.
 */
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
            handleFileOperation(choice);  // Handle back to authentication menu
            break;
        }
        handleFileOperation(choice);  // Execute the chosen file operation
    } while (choice != 8);
}

/**
 * @brief Displays the Authentication Menu and handles user choices.
 *
 * This function presents a menu for user authentication, allowing users to
 * register, log in, log in as a guest, or access file operations. It prompts
 * the user to enter a choice and executes the corresponding action based on
 * the input.
 *
 * The valid choices are as follows:
 * - 1: Register - Initiates the user registration process.
 * - 2: Login - Prompts for login credentials and attempts to log the user in.
 * - 3: Guest Login - Provides access to the system as a guest user.
 * - 4: File Operations for Fast Search Operations - Directs the user to the
 *    file operations menu.
 * - 5: Return to Main Menu - Exits the authentication menu and returns to the
 *    main menu.
 *
 * After executing the selected action, the function will return to the
 * authentication menu unless the user chooses to return to the main menu.
 *
 * @return true if the user remains in the authentication menu; false if the
 *         user chooses to return to the main menu.
 */
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
        Register(); // You should register for app
        break;
    case 2:
        if (logIn()) {
            clear_screen();
            printf("Login successful!\n"); // If password is true
        }
        else {
            clear_screen();
            printf("Invalid login. Returning to main menu.\n"); // If password is false 
        }
        break;
    case 3:
        guest(); // You are in guest mode 
        break;
    case 4:
        fileOperationsMenu(); // You are in file operations algorithms
        break;
    case 5:
        return false; // Return main menu
    default:
        clear_screen();
        printf("Invalid choice. Please try again.\n"); // If you are made an invalid choice
        break;
    }
    return true;
}

/**
 * @brief Handles user registration by collecting user information.
 *
 * This function prompts the user to enter their name, surname, phone number,
 * and password to create a new user account. Upon receiving the input, it
 * stores the new user data using the `saveUserData` function and displays a
 * confirmation message.
 *
 * After successfully registering the user, it prints the current state of the
 * hash table (presumably containing user records) and then redirects the user
 * to the main menu.
 *
 * The function does not accept any parameters and returns true to indicate
 * successful registration.
 *
 * @return true if registration is successful and the user is redirected to
 *         the main menu.
 */
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

    printHashTable(); // Print table after each record
    mainMenu();
    return true;
}

/**
 * @brief Handles user login by validating phone number and password.
 *
 * This function prompts the user to enter their phone number and password.
 * It then calls the `validateLogin` function to check if the provided
 * credentials are valid.
 *
 * The function does not accept any parameters and returns a boolean value
 * indicating the success or failure of the login attempt.
 *
 * @return true if the login is successful; false otherwise.
 */
bool logIn() {
    char phone[20];
    char password[20];
    printf("Enter your phone number: ");
    scanf("%s", phone);
    printf("Enter your password: ");
    scanf("%s", password);

    return validateLogin(phone, password); // Check login validity
}

/**
 * @brief Handles guest login mode.
 *
 * This function simulates a guest login by displaying a success message.
 * It does not require any user credentials and grants access to the
 * system in a limited capacity (as defined by the application's logic).
 *
 * The function does not accept any parameters and returns true to indicate
 * the successful entry into guest mode.
 *
 * @return true to signify that guest login was successful.
 */
bool guest() {
    clear_screen();
    printf("Guest login successful!\n");
    return true;
}

/**
 * @brief Creates a new event and saves it to a linked list and a binary file.
 *
 * This function prompts the user to enter details for a new event, including
 * the event type, date, color option, and concept. It then allocates memory
 * for the new event and appends it to a linked list of events. Finally, it
 * attempts to write the event data to a binary file named "event.bin".
 *
 * The function does not take any parameters and returns a boolean value
 * indicating the success or failure of the event creation process.
 *
 * @return true if the event is created and saved successfully; false otherwise.
 */
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

    // Write event data to the binary file "event.bin"
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

/**
 * @brief Manages the display and modification of events in a linked list.
 *
 * This function allows the user to navigate through a list of events,
 * displaying information for the current event and providing options
 * to move to the next or previous event, update event details, or
 * return to the main menu.
 *
 * The function utilizes a linked list of events and interacts with the
 * user through the console to facilitate event management.
 *
 * @return true if the user chooses to return to the main menu; false if
 * an event was updated or if no events are available.
 */
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
            // if you want to go to next event
            if (current->next != NULL) {
                current = current->next;
            }
            else {
                printf("No next event.\n");
            }
            clear_screen();
            break;
        case 2:
            // if you want to go to previous event
            if (current->prev != NULL) {
                current = current->prev;
            }
            else {
                printf("No previous event.\n");
            }
            clear_screen();
            break;
        case 3:
            // if you want to update event information
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
            printf("Invalid choice. Please try again.\n");// if you made an invalid choice
            break;
        }
    }

    return true; // Added return statement here
}

/**
 * @brief Displays the event management menu and handles user choices.
 *
 * This function presents a menu for event management, allowing the user
 * to create new events, manage existing events, or return to the main menu.
 * Depending on the user's choice, it invokes the appropriate functions for
 * event creation or management.
 *
 * @return true if the user wants to continue using the event menu;
 * false if the user chooses to return to the main menu.
 */
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
        createEvent(); // create event function called
        break;
    case 2:
        manageEvent(); // manage event function called
        break;
    case 3:
        return false; // return main menu
    default:
        clear_screen();
        printf("Invalid choice. Please try again.\n");
        break;
    }
    return true;
}

/**
 * @brief Maximum number of attendees allowed for an event.
 *
 * This constant defines the maximum number of attendees that can be
 * registered for any event in the event management system.
 * If the number of attendees exceeds this limit, additional
 * attendees cannot be added.
 */
#define MAX_ATTENDEES 100

 /**
  * @brief Maximum length for names in the system.
  *
  * This constant defines the maximum character length allowed
  * for attendee names (both first name and surname) in the event
  * management system. Names longer than this limit will be truncated
  * or rejected during input.
  */
#define MAX_NAME_LENGTH 50

  /**
   * @brief Struct to hold attendee details.
   *
   * This structure stores information about an attendee, including their
   * name, surname, and a Huffman code for encoding or identification.
   */
typedef struct {
    char nameAttendee[MAX_NAME_LENGTH];    ///< Attendee's first name.
    char surnameAttendee[MAX_NAME_LENGTH]; ///< Attendee's surname.
    char huffmanCode[MAX_NAME_LENGTH];     ///< Huffman code representing the attendee.
} Attendee;

// Array to hold attendees
Attendee attendees[MAX_ATTENDEES]; ///< Array to store up to MAX_ATTENDEES attendees.

/**
 * @brief Counter for the number of attendees registered.
 *
 * This variable keeps track of the total number of attendees currently
 * registered in the event management system.
 */
int attendeeCount = 0; ///< Number of registered attendees.

/**
 * @brief Performs the KMP (Knuth-Morris-Pratt) search algorithm.
 *
 * This function searches for a specified pattern within a string using the
 * KMP algorithm, which is efficient for substring searching. The results
 * will indicate the starting indices of the pattern within the string.
 *
 * @param pattern Pointer to the character array representing the pattern to search for.
 */
void kmpSearch(char* pattern);

/**
 * @brief Computes the Longest Prefix Suffix (LPS) array for the KMP algorithm.
 *
 * The LPS array is used to optimize the KMP search by skipping unnecessary comparisons.
 * It indicates the longest proper prefix of the pattern that is also a suffix for each prefix.
 *
 * @param pattern Pointer to the character array representing the pattern.
 * @param M Length of the pattern (number of characters in the pattern).
 * @param lps Pointer to an integer array that will be filled with the LPS values.
 */
void computeLPSArray(char* pattern, int M, int* lps);

/**
 * @brief Compresses the name of an attendee.
 *
 * This function modifies the attendee's name, potentially to fit within storage limits or to
 * optimize representation. The exact compression algorithm is implementation-dependent.
 *
 * @param attendee Pointer to the Attendee structure whose name is to be compressed.
 */
void compressAttendeeName(Attendee* attendee);

/**
 * @brief Registers attendees for the event.
 *
 * This function handles the registration process, including collecting attendee details
 * and storing them in the system. It may involve input validation and duplication checks.
 *
 * @return Returns true if registration was successful, false otherwise.
 */
bool registerAttendees();

/**
 * @brief Prints the details of all registered attendees.
 *
 * This function iterates through the list of registered attendees and displays their
 * information in a readable format.
 */
void printAttendees();

/**
 * @brief Manages attendee-related functionalities.
 *
 * This function provides a menu or interface for operations related to attendees, such as
 * registration, viewing attendee details, or other related actions.
 *
 * @return Returns true to indicate the menu is active; returns false when returning to the main menu.
 */
bool attendee();

/**
 * @brief Performs the Knuth-Morris-Pratt (KMP) search for a pattern in attendee Huffman codes.
 *
 * This function searches for a specified pattern within the Huffman codes of registered
 * attendees. It converts both the pattern and the Huffman codes to lowercase for case-insensitive
 * comparison. If a match is found, it prints the names of the attendees whose Huffman codes contain
 * the pattern.
 *
 * @param pattern Pointer to the character array representing the pattern to search for.
 *
 * This function dynamically allocates memory for the Longest Prefix Suffix (LPS) array used in
 * the KMP algorithm and frees it after use. If no matches are found, it can be configured to print
 * a message indicating that no match was found.
 */
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
/**
 * @brief Computes the Longest Prefix Suffix (LPS) array for a given pattern.
 *
 * The LPS array is used in the Knuth-Morris-Pratt (KMP) string matching algorithm
 * to determine the longest proper prefix of the pattern that is also a suffix. This
 * information allows the KMP algorithm to skip unnecessary comparisons in the search
 * process.
 *
 * @param pattern Pointer to the character array representing the pattern for which the
 *                LPS array is to be computed.
 * @param M The length of the pattern.
 * @param lps Pointer to an array where the computed LPS values will be stored.
 *
 * The LPS array is initialized with the first element set to 0, and the function processes
 * the pattern to fill the LPS array based on matching characters. This helps optimize the
 * KMP search algorithm by allowing it to skip certain comparisons when a mismatch occurs.
 */
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

/**
 * @brief Compresses and stores the Huffman code for an attendee's name.
 *
 * This function takes an attendee's name and simulates the process of Huffman
 * coding by storing the name directly in the `huffmanCode` field of the
 * `Attendee` structure. In a complete implementation, this function would
 * contain the logic to generate an actual Huffman code for compression.
 *
 * @param attendee Pointer to an `Attendee` structure whose name will be compressed
 *                 and stored as a Huffman code.
 *
 * The function copies each character from the attendee's name to the `huffmanCode`
 * array. For simplicity, it directly stores the name characters instead of generating
 * a true Huffman code. The resulting `huffmanCode` string is null-terminated to ensure
 * proper string handling in C.
 */
void compressAttendeeName(Attendee* attendee) {
    int len = strlen(attendee->nameAttendee);
    // Directly store the name for simplicity (as a placeholder for actual Huffman coding)
    for (int i = 0; i < len; i++) {
        attendee->huffmanCode[i] = attendee->nameAttendee[i]; // Directly storing name characters
    }
    attendee->huffmanCode[len] = '\0'; // Null terminate the string
}

/**
 * @brief Registers attendees and saves their information in a binary file.
 *
 * This function prompts the user for the number of attendees and collects
 * their names and surnames. It then compresses the attendees' names into
 * a simulated Huffman code and saves all the attendee information in a
 * binary file named "attendee.bin".
 *
 * The user is asked how many attendees will register. If the number
 * exceeds the defined limit or is invalid, an error message is displayed.
 * For each attendee, their name and surname are collected, compressed,
 * and saved to the binary file.
 *
 * @return true if the registration is successful; false if there is an
 *         error opening the file or writing data.
 */
bool registerAttendees() {
    int count;
    FILE* file = fopen("attendee.bin", "ab");
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
        compressAttendeeName(&attendees[attendeeCount]); 

        if (fwrite(&attendees[attendeeCount], sizeof(Attendee), 1, file) != 1) {
            /*   perror("Error writing to file");
               fclose(file);*/
            return false;
        }

        attendeeCount++;
    }

    printf("%d attendees registered and saved in binary format.\n", count);
    fclose(file); 
    return true;
}

/**
 * @brief Prints the details of all registered attendees.
 *
 * This function iterates through the array of registered attendees and
 * displays each attendee's name, surname, and their corresponding
 * Huffman code. The information is printed to the standard output.
 *
 * It is assumed that the attendeeCount variable accurately reflects the
 * number of attendees currently registered.
 */
void printAttendees() {
    printf("\nRegistered Attendees:\n");
    for (int i = 0; i < attendeeCount; i++) {
        printf("Name: %s, Surname: %s, Huffman Code: %s\n", attendees[i].nameAttendee, attendees[i].surnameAttendee, attendees[i].huffmanCode);
    }
}
/**
 * @brief Displays the attendee menu and handles user interactions.
 *
 * This function presents a menu for managing attendees, including
 * options to register attendees, search for attendees, print a list of
 * registered attendees, and manage the attendees list (add/remove).
 * It utilizes the Knuth-Morris-Pratt (KMP) algorithm to search through
 * the Huffman codes of attendees.
 *
 * @return Returns true if the user selects an option that leads to
 *         a return to the main menu; otherwise, returns false.
 *
 * The following options are available in the attendee menu:
 * 1. Register Attendees
 * 2. Track Attendees (search by name in Huffman code)
 * 3. Print Attendees
 * 4. Manage Attendees List (add, remove, or display activity history)
 * 5. Return to main menu
 */
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

/**
 * @brief Maximum number of elements allowed in various data structures.
 *
 * These constants define the limits for different data structures
 * used in the event management system.
 *
 * - MAX_SIZE: Maximum size for arrays or collections that may need to
 *   hold multiple elements.
 * - STACK_SIZE: Maximum number of elements that can be stored in a
 *   stack data structure.
 * - QUEUE_SIZE: Maximum number of elements that can be stored in a
 *   queue data structure.
 */
#define MAX_SIZE 100  /**< Maximum size for arrays or collections. */
#define STACK_SIZE 100 /**< Maximum size for the stack data structure. */
#define QUEUE_SIZE 100 /**< Maximum size for the queue data structure. */

 /**
  * @brief Structure to represent a sparse matrix for storing activity details.
  *
  * A sparse matrix is used to efficiently store activities where most of the
  * elements are zero. This structure maintains the non-zero entries in
  * separate arrays for rows, columns, and their corresponding values.
  */
typedef struct {
    int row[MAX_SIZE];         /**< Array to store the row indices of non-zero entries. */
    int col[MAX_SIZE];         /**< Array to store the column indices of non-zero entries. */
    char value[MAX_SIZE][100]; /**< Array to store the activity details for non-zero entries. */
    int size;                  /**< Number of non-zero entries in the sparse matrix. */
} SparseMatrix;

/** Global sparse matrix for activities. */
SparseMatrix activityMatrix; /**< Sparse matrix instance to store activity details. */


/**
 * @brief Structure to represent a stack for storing activities.
 *
 * This stack is used to keep track of activities in a last-in-first-out (LIFO)
 * manner, allowing for easy retrieval of the most recently added activities.
 */
typedef struct {
    char items[STACK_SIZE][100]; /**< Array to store the items in the stack. */
    int top;                     /**< Index of the top element in the stack. */
} Stack;

/** Global stack for storing activities. */
Stack activityStack; /**< Stack instance for managing activities. */

/**
 * @brief Structure to represent a queue for storing activities.
 *
 * This queue follows the first-in-first-out (FIFO) principle, allowing
 * activities to be added and retrieved in the order they were added.
 */
typedef struct {
    char items[QUEUE_SIZE][100]; /**< Array to store the items in the queue. */
    int front;                   /**< Index of the front element in the queue. */
    int rear;                    /**< Index of the rear element in the queue. */
} Queue;

/** Global queue for storing activities. */
Queue activityQueue; /**< Queue instance for managing activities. */


/**
 * @brief Structure to represent a node in an XOR linked list.
 *
 * Each node in this structure contains a value and a pointer that holds
 * the XOR of the previous and next nodes. This allows for efficient memory
 * usage by utilizing the same pointer for both previous and next references.
 */
typedef struct XORNode {
    char value[100];           /**< Value stored in the node. */
    struct XORNode* both;     /**< XOR of the previous and next node pointers. */
} XORNode;

/** Head of the XOR linked list. */
XORNode* xorHead = NULL; /**< Pointer to the first node in the XOR linked list. */


/**
 * @brief Function to compute the XOR of two pointers.
 *
 * This function takes two pointers and returns their XOR value, which can
 * be used to navigate an XOR linked list.
 *
 * @param a Pointer to the first node.
 * @param b Pointer to the second node.
 * @return XOR of the two pointers.
 */
XORNode* XOR(XORNode* a, XORNode* b) {
    return (XORNode*)((uintptr_t)(a) ^ (uintptr_t)(b));
}

/**
 * @brief Adds a new node with the specified value to the XOR linked list.
 *
 * This function creates a new node, initializes its value, and updates
 * the XOR pointers accordingly to maintain the structure of the linked list.
 *
 * @param value The value to be stored in the new node.
 */
void addToXORList(const char* value) {
    XORNode* newNode = (XORNode*)malloc(sizeof(XORNode)); /**< Allocate memory for the new node. */
    strcpy(newNode->value, value);                        /**< Copy the value into the new node. */
    newNode->both = XOR(xorHead, NULL);                   /**< Set both pointer to XOR of head and NULL. */

    if (xorHead != NULL) {
        // Update the both pointer of the current head
        XORNode* next = XOR(xorHead->both, NULL);         /**< Get the next node by XORing head's both pointer. */
        xorHead->both = XOR(newNode, next);                /**< Update head's both pointer. */
    }

    xorHead = newNode;                                    /**< Move head to the new node. */
}

/**
 * @brief Removes a node with the specified value from the XOR linked list.
 *
 * This function traverses the XOR linked list to find a node containing
 * the specified value. If found, it updates the pointers of the previous
 * and next nodes to maintain the integrity of the list and frees the
 * memory allocated for the removed node.
 *
 * @param value The value of the node to be removed.
 */
void removeFromXORList(const char* value) {
    XORNode* current = xorHead;  /**< Pointer to the current node being examined. */
    XORNode* prev = NULL;         /**< Pointer to the previous node. */
    XORNode* next;                /**< Pointer to the next node. */

    while (current != NULL) {
        if (strcmp(current->value, value) == 0) {
            // Node to be removed is found
            if (prev != NULL) {
                prev->both = XOR(prev->both, current);  /**< Update previous node's both pointer. */
            }
            else {
                xorHead = XOR(xorHead->both, NULL);    /**< Adjust head if the current node is the head. */
            }

            next = XOR(prev, current->both);           /**< Get the next node using XOR. */
            free(current);                             /**< Free memory for the current node. */
            current = next;                            /**< Move to the next node. */
        }
        else {
            next = XOR(prev, current->both);           /**< Get the next node using XOR. */
            prev = current;                             /**< Move previous to the current node. */
            current = next;                            /**< Move to the next node. */
        }
    }
}

/**
 * @brief Displays the values stored in the XOR linked list.
 *
 * This function traverses the XOR linked list starting from the head
 * and prints each node's value to the standard output. The values are
 * displayed in the order they appear in the list, followed by "NULL"
 * to indicate the end of the list.
 */
void displayXORList() {
    XORNode* current = xorHead;  /**< Pointer to the current node being examined. */
    XORNode* prev = NULL;         /**< Pointer to the previous node. */
    XORNode* next;                /**< Pointer to the next node. */

    printf("Activity History: \n");
    while (current != NULL) {
        printf("%s -> ", current->value);            /**< Print the current node's value. */
        next = XOR(prev, current->both);             /**< Get the next node using XOR. */
        prev = current;                               /**< Move previous to the current node. */
        current = next;                              /**< Move to the next node. */
    }
    printf("NULL\n");                                 /**< Indicate the end of the list. */
}


/**
 * @brief Initializes the XOR linked list.
 *
 * This function sets the head of the XOR linked list to NULL,
 * indicating that the list is empty.
 */
void initializeXORList() {
    xorHead = NULL;  // Initialize head as NULL
}

/**
 * @brief Initializes the sparse matrix.
 *
 * This function sets the size of the sparse matrix to zero,
 * indicating that there are no non-zero entries in the matrix.
 */
void initializeSparseMatrix() {
    activityMatrix.size = 0;  // Initialize size to zero
}

/**
 * @brief Initializes the stack for activities.
 *
 * This function sets the top of the activity stack to -1,
 * indicating that the stack is currently empty.
 */
void initializeStack() {
    activityStack.top = -1;  // Stack is empty
}

/**
 * @brief Initializes the queue for activities.
 *
 * This function sets the front and rear of the activity queue to 0,
 * indicating that the queue is currently empty.
 */
void initializeQueue() {
    activityQueue.front = 0;
    activityQueue.rear = 0;  // Queue is empty
}

/**
 * @brief Checks if the stack is full.
 *
 * @return true if the stack is full, false otherwise.
 *
 * This function compares the top of the stack with the maximum size
 * to determine if the stack has reached its capacity.
 */
bool isStackFull() {
    return activityStack.top == STACK_SIZE - 1;
}

/**
 * @brief Checks if the stack is empty.
 *
 * @return true if the stack is empty, false otherwise.
 *
 * This function checks if the top of the stack is -1 to determine
 * if there are no elements in the stack.
 */
bool isStackEmpty() {
    return activityStack.top == -1;
}

/**
 * @brief Pushes an activity onto the stack.
 *
 * This function adds a new activity to the top of the stack.
 * If the stack is full, an error message is printed.
 *
 * @param activity A pointer to a string containing the activity to be pushed onto the stack.
 */
void pushStack(const char* activity) {
    if (!isStackFull()) {
        strcpy(activityStack.items[++activityStack.top], activity);
    }
    else {
        printf("Error: Stack is full!\n");
    }
}

/**
 * @brief Pops an activity from the stack.
 *
 * This function removes the activity at the top of the stack
 * and prints it. If the stack is empty, an error message is printed.
 */
void popStack() {
    if (!isStackEmpty()) {
        printf("Popped activity: %s\n", activityStack.items[activityStack.top--]);
    }
    else {
        printf("Error: Stack is empty!\n");
    }
}


/**
 * @brief Checks if the queue is full.
 *
 * This function determines if the queue has reached its maximum capacity.
 * It checks if the rear index has reached the size limit of the queue.
 *
 * @return true if the queue is full; false otherwise.
 */
bool isQueueFull() {
    return activityQueue.rear == QUEUE_SIZE;
}

/**
 * @brief Checks if the queue is empty.
 *
 * This function determines if there are any items in the queue.
 * It checks if the front index is equal to the rear index.
 *
 * @return true if the queue is empty; false otherwise.
 */
bool isQueueEmpty() {
    return activityQueue.front == activityQueue.rear;
}

/**
 * @brief Enqueues an activity into the queue.
 *
 * This function adds a new activity to the end of the queue.
 * If the queue is not full, the activity is copied to the queue at the rear position,
 * and the rear index is incremented.
 *
 * @param activity The activity to be added to the queue.
 */
void enqueue(const char* activity) {
    if (!isQueueFull()) {
        strcpy(activityQueue.items[activityQueue.rear++], activity);
    }
    // else {
    //     printf("Error: Queue is full!\n");
    // }
}

/**
 * @brief Dequeues an activity from the queue.
 *
 * This function removes an activity from the front of the queue.
 * If the queue is not empty, it prints the dequeued activity and increments the front index.
 *
 * @note If the queue is empty, an error message is displayed.
 */
void dequeue() {
    if (!isQueueEmpty()) {
        printf("Dequeued activity: %s\n", activityQueue.items[activityQueue.front++]);
    }
    else {
        printf("Error: Queue is empty!\n");
    }
}

/**
 * @brief Adds an activity to the sparse matrix.
 *
 * This function adds a new activity to the sparse matrix at the specified row and column.
 * If the matrix is not full, it stores the activity along with its position in the matrix,
 * and increments the size of the matrix. The function also pushes the activity onto a stack,
 * enqueues it in a queue, and adds it to an XOR linked list for tracking purposes.
 *
 * @param row The row index where the activity will be stored in the sparse matrix.
 * @param col The column index where the activity will be stored in the sparse matrix.
 * @param activity The activity details to be added to the sparse matrix.
 *
 * @note The function removes any newline characters from the activity string before storing it.
 *       If the sparse matrix is full, an error message is displayed.
 */
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

/**
 * @brief Displays the activities stored in the sparse matrix.
 *
 * This function iterates through the sparse matrix and prints out the row,
 * column, and details of each activity stored. After displaying the activities,
 * it waits for the user to press Enter to continue.
 */
void displayActivities() {
    printf("Activities in Sparse Matrix:\n");
    for (int i = 0; i < activityMatrix.size; i++) {
        printf("Row: %d, Column: %d, Activity: %s\n",
            activityMatrix.row[i], activityMatrix.col[i], activityMatrix.value[i]);
    }
    printf("Press Enter to continue...\n");
    getchar();  // Wait for user to press Enter
}

/**
 * @brief Plans timelines for activities.
 *
 * This function prompts the user to enter timeline details, such as start and end dates.
 * It reads the input and displays the planned timeline to the user.
 * After displaying the timeline, it waits for the user to press Enter to continue.
 */
void planTimelines() {
    char timeline[100];  // Buffer for input
    printf("Enter the timeline details (e.g., start and end dates): ");
    fgets(timeline, sizeof(timeline), stdin);  // Get input from user
    printf("Timeline planned: %s\n", timeline);  // Show entered timeline
    printf("Press Enter to continue...\n");
    getchar();  // Wait for user to press Enter
}

/**
 * @brief Organizes activities by adding them to a sparse matrix.
 *
 * This function prompts the user to enter the row and column indices for an
 * activity, followed by the activity details. It reads the user input and
 * adds the activity to the sparse matrix using the `addActivityToMatrix` function.
 * After successfully adding the activity, it displays a confirmation message
 * and waits for the user to press Enter to continue.
 */
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

/**
 * @brief Displays the schedule submenu and handles user choices.
 *
 * This function presents a menu of options related to scheduling activities.
 * It allows users to plan timelines, organize activities, display activities,
 * show activity history, pop an activity from the stack, dequeue an activity,
 * or return to the main menu. The function runs in a loop until the user decides
 * to return to the main menu, processing user input and invoking the appropriate
 * functions based on the user's choice.
 *
 * @return Returns false if the user chooses to return to the main menu,
 *         otherwise true.
 */
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

/**
 * @brief Heapifies a subtree rooted at the given index.
 *
 * This function ensures that the subtree rooted at index `i` in the array
 * satisfies the max-heap property. It assumes that the left and right
 * subtrees are already heapified. If the subtree is not a max-heap, it
 * will rearrange the elements to maintain the max-heap property.
 *
 * @param arr The array representing the heap.
 * @param n The total number of elements in the array.
 * @param i The index of the root of the subtree to be heapified.
 */
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

/**
 * @brief Performs heap sort on the given array.
 *
 * This function sorts an array of integers using the heap sort algorithm.
 * It first builds a max-heap from the input array and then repeatedly
 * extracts the maximum element from the heap, placing it at the end of
 * the sorted portion of the array.
 *
 * @param arr The array of integers to be sorted.
 * @param n The total number of elements in the array.
 */
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

/**
 * @brief Maximum number of feedback ratings allowed.
 *
 * This constant defines the maximum number of feedback ratings that can
 * be stored in the feedbackRatings array.
 */
#define MAX_FEEDBACKS 4

 /**
  * @brief Maximum number of keys for the B+ tree.
  *
  * This constant defines the maximum number of keys that can be stored in
  * each node of the B+ tree.
  */
#define MAX_KEYS 3  // Maximum number of keys for B+ tree

  /**
   * @brief Array to store feedback ratings.
   *
   * This array holds the feedback ratings given by users. It has a fixed
   * size defined by MAX_FEEDBACKS.
   */
int feedbackRatings[MAX_FEEDBACKS];

/**
 * @brief Current count of feedback ratings.
 *
 * This variable tracks the number of feedback ratings currently stored in
 * the feedbackRatings array.
 */
int feedbackCount = 0;

// Global variables required for Strongly Connected Components (SCC)

/**
 * @brief Discovery time of each node.
 *
 * This array keeps track of the discovery times of nodes during the
 * depth-first search (DFS) for finding strongly connected components.
 */
int discoveryTime[MAX_FEEDBACKS];

/**
 * @brief Lowest link value of each node.
 *
 * This array stores the lowest discovery time reachable from each node,
 * which is used in the SCC algorithm to determine strongly connected
 * components.
 */
int lowLink[MAX_FEEDBACKS];

/**
 * @brief Stack to keep track of nodes in the current path of DFS.
 *
 * This boolean array indicates whether a node is currently in the stack
 * used for tracking nodes during the DFS.
 */
bool inStack[MAX_FEEDBACKS];

/**
 * @brief Stack for storing the nodes during the DFS process.
 *
 * This array is used as a stack to store the nodes visited during the
 * depth-first search for SCC. The top of the stack is tracked using the
 * stackTop variable.
 */
int stack[MAX_FEEDBACKS];

/**
 * @brief Top index of the stack.
 *
 * This variable tracks the top index of the stack used during the DFS for
 * strongly connected components.
 */
int stackTop = -1;

/**
 * @brief Counter for the discovery time.
 *
 * This variable serves as a counter for assigning discovery times to nodes
 * during the DFS traversal for finding strongly connected components.
 */
int timeCounter = 0;


/**
 * @brief Structure representing a leaf node in a B+ tree.
 *
 * A B+ tree leaf node contains keys and a pointer to the next leaf node,
 * allowing for linked traversal of leaf nodes. It also maintains a count
 * of the number of keys currently stored in the node.
 */
typedef struct BPlusLeafNode {
    int keys[MAX_KEYS];               /**< Array to store keys in the leaf node. */
    struct BPlusLeafNode* next;       /**< Pointer to the next leaf node for linked traversal. */
    int numKeys;                      /**< Number of keys currently stored in the leaf node. */
} BPlusLeafNode;

/**
 * @brief Structure representing an internal node in a B+ tree.
 *
 * A B+ tree internal node contains keys and pointers to its children nodes.
 * It allows for efficient searching and maintaining the B+ tree structure.
 */
typedef struct BPlusInternalNode {
    int keys[MAX_KEYS];               /**< Array to store keys in the internal node. */
    void* children[MAX_KEYS + 1];     /**< Array of pointers to child nodes (internal or leaf). */
    int numKeys;                      /**< Number of keys currently stored in the internal node. */
} BPlusInternalNode;

/**
 * @brief Structure representing a B+ tree.
 *
 * A B+ tree consists of a root node, which can be an internal or leaf node.
 * This structure serves as the entry point to the B+ tree and facilitates
 * operations such as insertion, deletion, and searching.
 */
typedef struct BPlusTree {
    void* root;                       /**< Pointer to the root node of the B+ tree (internal or leaf). */
} BPlusTree;


/**
 * @brief Creates a new B+ tree.
 *
 * Allocates memory for a new B+ tree and initializes it with a root leaf node.
 *
 * @return A pointer to the newly created BPlusTree structure.
 *         The root of the tree is initialized as an empty leaf node.
 */
BPlusTree* createBPlusTree() {
    BPlusTree* tree = (BPlusTree*)malloc(sizeof(BPlusTree));
    // Uncomment the following lines to handle memory allocation failure
    // if (tree == NULL) {
    //     perror("Failed to allocate memory for BPlusTree");
    //     exit(EXIT_FAILURE); // Stop the program if memory allocation fails
    // }

    BPlusLeafNode* rootLeaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
    // Uncomment the following lines to handle memory allocation failure
    // if (rootLeaf == NULL) {
    //     perror("Failed to allocate memory for BPlusLeafNode");
    //     exit(EXIT_FAILURE); // Stop the program if memory allocation fails
    // }

    rootLeaf->numKeys = 0;  // Initialize the number of keys to 0
    rootLeaf->next = NULL;   // Set the next pointer to NULL
    tree->root = rootLeaf;   // Set the root of the tree to the new leaf node
    return tree;             // Return the newly created BPlusTree
}

/**
 * @brief Inserts a key into a leaf node of the B+ tree.
 *
 * If the leaf node has space for the new key, it inserts it in sorted order.
 * If the leaf node is full, it splits the node, creating a new leaf node
 * and redistributing the keys accordingly.
 *
 * @param leaf A pointer to the leaf node where the key will be inserted.
 * @param key The key to be inserted into the leaf node.
 */
void insertIntoLeaf(BPlusLeafNode* leaf, int key) {
    if (leaf->numKeys < MAX_KEYS) {
        // Insert key in sorted order
        int i = leaf->numKeys - 1;
        while (i >= 0 && leaf->keys[i] > key) {
            leaf->keys[i + 1] = leaf->keys[i]; // Shift keys to the right
            i--;
        }
        leaf->keys[i + 1] = key; // Insert the new key
        leaf->numKeys++;         // Increment the number of keys
    }
    else {
        // Leaf node is full, split the node
        BPlusLeafNode* newLeaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
        newLeaf->numKeys = 0;        // Initialize new leaf node
        newLeaf->next = leaf->next;  // Link new leaf to the next node
        leaf->next = newLeaf;        // Update current leaf's next pointer

        int mid = MAX_KEYS / 2;      // Calculate midpoint for splitting
        newLeaf->numKeys = MAX_KEYS - mid; // Assign keys to the new leaf
        for (int i = 0; i < newLeaf->numKeys; i++) {
            newLeaf->keys[i] = leaf->keys[mid + i];
        }
        leaf->numKeys = mid;         // Update the current leaf's key count

        // Add the new key to the appropriate leaf
        if (key > newLeaf->keys[0]) {
            insertIntoLeaf(newLeaf, key); // Insert in the new leaf
        }
        else {
            insertIntoLeaf(leaf, key);     // Insert in the current leaf
        }
    }
}

/**
 * @brief Inserts a key into the B+ tree.
 *
 * If the root leaf node has space for the new key, it inserts it into the leaf node.
 * If the root is full, it creates a new root node, splits the existing root,
 * and promotes the median key to the new root.
 *
 * @param tree A pointer to the BPlusTree structure where the key will be inserted.
 * @param key The key to be inserted into the B+ tree.
 */
void insert(BPlusTree* tree, int key) {
    BPlusLeafNode* root = (BPlusLeafNode*)tree->root;
    if (root->numKeys < MAX_KEYS) {
        // Insert key directly into the root leaf node
        insertIntoLeaf(root, key);
    }
    else {
        // Root is full, create a new root and split the existing root
        BPlusInternalNode* newRoot = (BPlusInternalNode*)malloc(sizeof(BPlusInternalNode));
        newRoot->numKeys = 1;                             // Set number of keys in new root
        newRoot->keys[0] = root->keys[MAX_KEYS / 2];     // Promote median key to new root
        newRoot->children[0] = root;                      // Link old root to new root
        BPlusLeafNode* newLeaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
        newLeaf->numKeys = 0;                             // Initialize new leaf node
        newLeaf->next = NULL;                             // Set next pointer to NULL
        newRoot->children[1] = newLeaf;                   // Link new leaf to new root

        tree->root = newRoot;                             // Update the tree's root
        insertIntoLeaf(newLeaf, key);                     // Insert key into the new leaf
    }
}

/**
 * @brief Pushes a node onto the SCC stack.
 *
 * Marks the node as being in the stack and pushes it onto the stack.
 *
 * @param node The node to be pushed onto the stack.
 */
void pushStackSCC(int node) {
    stack[++stackTop] = node;   // Push the node onto the stack
    inStack[node] = true;       // Mark the node as in the stack
}

/**
 * @brief Pops a node from the SCC stack.
 *
 * Removes the top node from the stack and marks it as not in the stack.
 *
 * @return The node that was popped from the stack.
 */
int popStackSCC() {
    int node = stack[stackTop--]; // Pop the top node from the stack
    inStack[node] = false;        // Mark the node as not in the stack
    return node;                  // Return the popped node
}

/**
 * @brief Finds strongly connected components (SCC) in a directed graph.
 *
 * This function implements Tarjan's algorithm to find all strongly connected components
 * in a directed graph represented by an adjacency matrix. It uses depth-first search (DFS)
 * to explore the graph and maintains discovery and low-link values for each node.
 *
 * @param node The current node being visited in the graph.
 * @param adjMatrix The adjacency matrix representing the graph.
 * @param n The number of nodes in the graph.
 */
void SCC(int node, int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS], int n) {
    discoveryTime[node] = lowLink[node] = ++timeCounter;  // Initialize discovery and low-link values
    pushStackSCC(node);                                   // Push the node onto the stack

    for (int i = 0; i < n; i++) {                        // Explore all adjacent nodes
        if (adjMatrix[node][i]) {                        // Check if there is an edge from node to i
            if (discoveryTime[i] == -1) {               // If the node has not been visited
                SCC(i, adjMatrix, n);                   // Recursively visit the node
                lowLink[node] = (lowLink[node] < lowLink[i]) ? lowLink[node] : lowLink[i]; // Update low-link value
            }
            else if (inStack[i]) {                       // If the node is in the stack
                lowLink[node] = (lowLink[node] < discoveryTime[i]) ? lowLink[node] : discoveryTime[i]; // Update low-link value
            }
        }
    }

    if (lowLink[node] == discoveryTime[node]) {          // If the node is a root of an SCC
        printf("SCC Found: ");
        int w;
        do {
            w = popStackSCC();                           // Pop nodes from the stack until reaching the root
            printf("Feedback %d (Rating: %d) ", w + 1, feedbackRatings[w]);
        } while (w != node);                             // Continue until the root node is reached
        printf("\n");
    }
}

/**
 * @brief Gathers feedback and ratings from the user.
 *
 * This function prompts the user to enter their feedback for the application
 * and a rating between 1 and 5. It stores the feedback rating in an array and
 * inserts the rating into a B+ tree for further processing.
 *
 * @param tree A pointer to the BPlusTree where the feedback rating will be inserted.
 */
void gatherFeedbacks(BPlusTree* tree) {
    char feedback[256];
    int rating;

    printf("Enter your feedback (max 255 characters): ");
    fgets(feedback, sizeof(feedback), stdin);             // Get feedback from user
    feedback[strcspn(feedback, "\n")] = 0;                // Remove trailing newline

    printf("Rate the application (1 to 5): ");
    scanf("%d", &rating);                                  // Get rating from user
    getchar();                                             // Clear the input buffer

    if (rating >= 1 && rating <= 5) {                     // Validate rating
        feedbackRatings[feedbackCount++] = rating;        // Store the rating
        insert(tree, rating);                              // Insert rating into B+ tree
        printf("Feedback received: %s\n", feedback);
        printf("Rating received: %d\n", rating);
    }
    else {
        printf("Invalid rating. Please enter a value between 1 and 5.\n");
    }

    printf("Press Enter to continue...\n");
    getchar();                                             // Wait for user input
}


/**
 * @brief Displays the feedback ratings in sorted order.
 *
 * This function checks if there are any feedback ratings available. If ratings exist,
 * it creates a copy of the ratings, sorts them using bubble sort, and displays the
 * sorted ratings to the user.
 */
void displaySortedRatings() {
    if (feedbackCount == 0) {                              // Check if there are no ratings
        printf("No ratings available.\n");
        return;
    }

    int sortedRatings[MAX_FEEDBACKS];
    memcpy(sortedRatings, feedbackRatings, feedbackCount * sizeof(int)); // Copy ratings

    // Bubble sort for sorting ratings
    for (int i = 0; i < feedbackCount - 1; i++) {
        for (int j = 0; j < feedbackCount - i - 1; j++) {
            if (sortedRatings[j] > sortedRatings[j + 1]) { // Compare and swap
                int temp = sortedRatings[j];
                sortedRatings[j] = sortedRatings[j + 1];
                sortedRatings[j + 1] = temp;
            }
        }
    }

    printf("Sorted Ratings:\n");
    for (int i = 0; i < feedbackCount; i++) {
        printf("%d ", sortedRatings[i]);                    // Display sorted ratings
    }
    printf("\nPress Enter to return to Feedback Menu...\n");
    getchar();                                             // Wait for user input
}

/**
 * @brief Performs breadth-first search (BFS) traversal from a given starting node.
 *
 * This function implements the BFS algorithm on a graph represented by an
 * adjacency matrix. It starts the traversal from the specified starting node,
 * visits all reachable nodes, and displays the ratings of the visited feedbacks.
 *
 * @param startNode The starting node for the BFS traversal.
 * @param adjMatrix The adjacency matrix representing the graph.
 * @param n The number of nodes in the graph.
 */
void BFS(int startNode, int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS], int n) {
    int queue[MAX_FEEDBACKS], front = 0, rear = 0;          // Queue for BFS
    int visited[MAX_FEEDBACKS] = { 0 };                     // Array to track visited nodes

    queue[rear++] = startNode;                              // Enqueue the starting node
    visited[startNode] = 1;

    printf("BFS Traversal starting from Feedback %d:\n", startNode + 1);
    while (front < rear) {                                  // Process the queue
        int node = queue[front++];
        printf("Visited Feedback %d with Rating %d\n", node + 1, feedbackRatings[node]);

        for (int i = 0; i < n; i++) {                       // Explore adjacent nodes
            if (adjMatrix[node][i] && !visited[i]) {      // Check for edges and visited status
                queue[rear++] = i;                          // Enqueue unvisited adjacent node
                visited[i] = 1;
            }
        }
    }
}
/**
 * @brief Performs depth-first search (DFS) traversal from a given node.
 *
 * This function implements the DFS algorithm on a graph represented by an
 * adjacency matrix. It starts the traversal from the specified node, marks
 * it as visited, and recursively visits all reachable nodes, displaying the
 * ratings of the visited feedbacks.
 *
 * @param node The current node to visit in the DFS traversal.
 * @param visited An array that keeps track of visited nodes.
 * @param adjMatrix The adjacency matrix representing the graph.
 * @param n The number of nodes in the graph.
 */
void DFS(int node, int visited[MAX_FEEDBACKS], int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS], int n) {
    visited[node] = 1;                                      // Mark the current node as visited
    printf("Visited Feedback %d with Rating %d\n", node + 1, feedbackRatings[node]);

    for (int i = 0; i < n; i++) {                           // Explore adjacent nodes
        if (adjMatrix[node][i] && !visited[i]) {          // Check for edges and visited status
            DFS(i, visited, adjMatrix, n);                 // Recursive DFS call
        }
    }
}

/**
 * @brief Prints the contents of the leaf nodes in a B+ tree.
 *
 * This function traverses the leaf nodes of the B+ tree starting from the root
 * and prints the keys stored in each leaf node. It continues until it reaches
 * the end of the leaf nodes, displaying all keys in the tree.
 *
 * @param tree A pointer to the B+ tree from which to print the leaf nodes.
 */
void printLeafNodes(BPlusTree* tree) {
    BPlusLeafNode* current = (BPlusLeafNode*)tree->root;  // Start from the root leaf node
    while (current != NULL) {                               // Traverse through all leaf nodes
        printf("Leaf Node: ");
        for (int i = 0; i < current->numKeys; i++) {      // Print keys in the current leaf node
            printf("%d ", current->keys[i]);
        }
        printf("\n");
        current = current->next;                            // Move to the next leaf node
    }
}

/**
 * @brief Displays the feedback menu and handles user interactions.
 *
 * This function manages the feedback process, allowing users to gather feedback,
 * view sorted ratings, and perform operations such as BFS, DFS, and finding
 * strongly connected components (SCC) using Tarjan's algorithm. It repeatedly
 * presents a menu to the user until they choose to return to the main menu.
 *
 * @return Returns false if the user opts to return to the main menu, true otherwise.
 */
bool feedback() {
    BPlusTree* tree = createBPlusTree();  // Create a new B+ tree for storing feedback ratings
    int choice;
    while (1) {
        // Display the feedback menu
        printf("\n----------- Feedback Menu -----------\n");
        printf("1. Gather Feedback\n");
        printf("2. View Sorted Ratings\n");
        printf("3. Print B+ Tree\n");
        printf("4. Perform BFS\n");
        printf("5. Perform DFS\n");
        printf("6. Find SCC (Tarjan Algorithm)\n");
        printf("7. Return to Main Menu\n");
        printf("Please enter your choice: ");

        scanf("%d", &choice);                // Get user's choice
        getchar();                           // Clear the newline character

        switch (choice) {
        case 1:
            gatherFeedbacks(tree);           // Call function to gather feedback
            break;
        case 2:
            displaySortedRatings();          // Call function to display sorted ratings
            break;
        case 3:
            printf("Feedbacks stored in B+ Tree:\n");
            printLeafNodes(tree);            // Call function to print leaf nodes of the B+ tree
            break;
        case 4: {                            // Perform BFS
            int startNode;
            printf("Enter starting feedback number for BFS (1 to %d): ", feedbackCount);
            scanf("%d", &startNode);         // Get starting node for BFS
            getchar();

            if (startNode < 1 || startNode > feedbackCount) {
                printf("Invalid node.\n");
                break;
            }
            startNode--;

            int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 }; // Adjacency matrix initialization
            adjMatrix[0][1] = adjMatrix[1][0] = 1;  // Sample edges for testing
            adjMatrix[1][2] = adjMatrix[2][1] = 1;

            BFS(startNode, adjMatrix, feedbackCount); // Call BFS function
        } break;
        case 5: {                            // Perform DFS
            int startNode;
            printf("Enter starting feedback number for DFS (1 to %d): ", feedbackCount);
            scanf("%d", &startNode);         // Get starting node for DFS
            getchar();

            if (startNode < 1 || startNode > feedbackCount) {
                printf("Invalid node.\n");
                break;
            }
            startNode--;

            int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 }; // Adjacency matrix initialization
            adjMatrix[0][1] = adjMatrix[1][0] = 1;  // Sample edges for testing
            adjMatrix[1][2] = adjMatrix[2][1] = 1;

            int visited[MAX_FEEDBACKS] = { 0 };   // Visited array for DFS
            DFS(startNode, visited, adjMatrix, feedbackCount); // Call DFS function
        } break;
        case 6: {                            // Find SCC
            int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 }; // Adjacency matrix initialization
            adjMatrix[0][1] = adjMatrix[1][0] = 1;  // Sample edges for testing
            adjMatrix[1][2] = adjMatrix[2][1] = 1;

            memset(discoveryTime, -1, sizeof(discoveryTime)); // Reset discovery times
            memset(lowLink, -1, sizeof(lowLink));             // Reset low link values
            memset(inStack, false, sizeof(inStack));         // Reset stack status
            stackTop = -1;                                    // Initialize stack top
            timeCounter = 0;                                  // Initialize time counter

            printf("Finding SCCs:\n");
            for (int i = 0; i < feedbackCount; i++) {
                if (discoveryTime[i] == -1) {                // If node is unvisited
                    SCC(i, adjMatrix, feedbackCount);        // Call SCC function
                }
            }
        } break;
        case 7:
            return false; // Return to main menu
        default:
            printf("Invalid choice. Try again.\n"); // Handle invalid choice
        }
        return 0; // This should be outside the while loop for continuous menu display
    }
    return true; // Default return value
}
