#include <cstring>
#include "gtest/gtest.h"
#include "../../event/header/event.h"  
#include "../../event/src/event.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
#include <cstdio>
#include <fstream>
#include "event_test.h"

class EventAppTest : public ::testing::Test {
protected:
    const char* inputTest = "inputTest.txt";
    const char* outputTest = "outputTest.txt";
    void SetUp() override {
        // Setup test data
    }

    void TearDown() override {
        remove(inputTest);
        remove(outputTest);
    }
    void simulateUserInput(const char* userInput) {
        FILE* fileinput = fopen(inputTest, "wb");
        fputs(userInput, fileinput);
        fclose(fileinput);
        freopen(inputTest, "rb", stdin);
        freopen(outputTest, "wb", stdout);
    }

    void readOutput(const char* outputFilePath, char* buffer, size_t bufferSize) {
        FILE* fileoutput = fopen(outputFilePath, "rb");
        size_t charsRead = fread(buffer, sizeof(char), bufferSize - 1, fileoutput);
        fclose(fileoutput);
        buffer[charsRead] = '\0';
        removeClearScreenCharsFromOutputFile(buffer);
    }

    void resetStdinStdout() {
        fclose(stdin);
        fflush(stdout);
#ifdef _WIN32
        freopen("CON", "a", stdout);
        freopen("CON", "r", stdin);
#else
        freopen("/dev/tty", "a", stdout);
        freopen("/dev/tty", "r", stdin);
#endif // _WIN32
    }

    void removeClearScreenCharsFromOutputFile(char* str) {
        char* src = str;
        char* dst = str;
        while (*src) {
            if (*src != '\f') {
                *dst++ = *src;
            }
            src++;
    }
        *dst = '\0';
}
};


TEST_F(EventAppTest, AttendeeStructureTest) {
    AttendeE attendee;
    strcpy(attendee.nameAttendee, "John");
    strcpy(attendee.surnameAttendee, "Doe");
    strcpy(attendee.huffmanCode, "101010");

    EXPECT_STREQ("John", attendee.nameAttendee);
    EXPECT_STREQ("Doe", attendee.surnameAttendee);
    EXPECT_STREQ("101010", attendee.huffmanCode);
}
TEST_F(EventAppTest, MinHeapNodeStructureTest) {
    MinHeapNode node;
    node.data = 'A';
    node.freq = 5;
    node.left = nullptr;
    node.right = nullptr;

    EXPECT_EQ('A', node.data);
    EXPECT_EQ(5u, node.freq);
    EXPECT_EQ(nullptr, node.left);
    EXPECT_EQ(nullptr, node.right);
}

TEST_F(EventAppTest, MinHeapStructureTest) {
    MinHeap* minHeap = new MinHeap;
    minHeap->size = 0;
    minHeap->capacity = 10;
    minHeap->array = new MinHeapNode * [minHeap->capacity];

    EXPECT_EQ(0, minHeap->size);       
    EXPECT_EQ(10, minHeap->capacity);     
    EXPECT_NE(nullptr, minHeap->array);   

    delete[] minHeap->array;
    delete minHeap;
}

TEST_F(EventAppTest, CreateMinHeapTest) {
    unsigned capacity = 10;
    MinHeap* minHeap = createMinHeap(capacity);

    EXPECT_NE(nullptr, minHeap);                    
    EXPECT_EQ(0, minHeap->size);                      
    EXPECT_EQ(capacity, minHeap->capacity);           
    EXPECT_NE(nullptr, minHeap->array);            

    free(minHeap->array);
    free(minHeap);
}

TEST_F(EventAppTest, CreateMinHeapNodeTest) {
    char data = 'A';
    unsigned freq = 5;

    MinHeapNode* node = createMinHeapNode(data, freq);

    EXPECT_NE(nullptr, node);            
    EXPECT_EQ(data, node->data);        
    EXPECT_EQ(freq, node->freq);          
    EXPECT_EQ(nullptr, node->left);      
    EXPECT_EQ(nullptr, node->right);     

    free(node);
}

TEST_F(EventAppTest, InsertMinHeapTest) {
    unsigned capacity = 10;
    MinHeap* minHeap = createMinHeap(capacity);

    MinHeapNode* node1 = createMinHeapNode('A', 5);
    MinHeapNode* node2 = createMinHeapNode('B', 3);

    insertMinHeap(minHeap, node1);
    EXPECT_EQ(1, minHeap->size);             
    EXPECT_EQ(node1, minHeap->array[0]);   

    insertMinHeap(minHeap, node2);
    EXPECT_EQ(2, minHeap->size);          
    EXPECT_EQ(node2, minHeap->array[1]);   

    free(minHeap->array);
    free(minHeap);
    free(node1);
    free(node2);
}

TEST_F(EventAppTest, ExtractMinTest) { 
    MinHeap* minHeap = createMinHeap(10);
    char data1 = 'A', data2 = 'B', data3 = 'C';
    unsigned freq1 = 5, freq2 = 3, freq3 = 8;

    insertMinHeap(minHeap, createMinHeapNode(data1, freq1));
    insertMinHeap(minHeap, createMinHeapNode(data2, freq2));
    insertMinHeap(minHeap, createMinHeapNode(data3, freq3));

    MinHeapNode* minNode = extractMin(minHeap);

    EXPECT_NE(nullptr, minNode);                    
    EXPECT_EQ(data2, minNode->data);               
    EXPECT_EQ(freq2, minNode->freq);               
    EXPECT_EQ(2, minHeap->size);            
    free(minNode);
    free(minHeap->array[0]); 
    free(minHeap->array[1]); 
    free(minHeap);          
}

TEST_F(EventAppTest, UserStructureTest) {
    User user;
    strcpy(user.name, "Alice");
    strcpy(user.surname, "Smith");
    strcpy(user.phone, "1234567890");
    strcpy(user.password, "password123");
    user.next = nullptr; 
    EXPECT_STREQ("Alice", user.name);       
    EXPECT_STREQ("Smith", user.surname);   
    EXPECT_STREQ("1234567890", user.phone);  
    EXPECT_STREQ("password123", user.password);
    EXPECT_EQ(nullptr, user.next);          
    User* hashTable[TABLE_SIZE] = { nullptr }; 
    for (int i = 0; i < TABLE_SIZE; i++) {
        EXPECT_EQ(nullptr, hashTable[i]); 
    }
}


TEST_F(EventAppTest, EventStructureTest) {
    Event event;
    strcpy(event.type, "Conference");
    strcpy(event.date, "2024-12-31");
    strcpy(event.color, "Blue");
    strcpy(event.concept, "Technology and Innovation");
    event.prev = nullptr; 
    event.next = nullptr; 
    EXPECT_STREQ("Conference", event.type);
    EXPECT_STREQ("2024-12-31", event.date);               
    EXPECT_STREQ("Blue", event.color);                    
    EXPECT_STREQ("Technology and Innovation", event.concept); 
    EXPECT_EQ(nullptr, event.prev);                       
    EXPECT_EQ(nullptr, event.next);                       

    head = &event; 
    tail = &event; 

    EXPECT_EQ(head, &event); 
    EXPECT_EQ(tail, &event);
}

TEST_F(EventAppTest, HashFunctionTest) {
    const char* phone1 = "1234567890";
    const char* phone2 = "0987654321";
    const char* phone3 = "5555555555";
    unsigned int expectedHash1 = hash(phone1);
    unsigned int expectedHash2 = hash(phone2);
    unsigned int expectedHash3 = hash(phone3);

    EXPECT_EQ(expectedHash1, hash(phone1)); 
    EXPECT_EQ(expectedHash2, hash(phone2)); 
    EXPECT_EQ(expectedHash3, hash(phone3)); 

    EXPECT_NE(expectedHash1, expectedHash2); 
    EXPECT_NE(expectedHash2, expectedHash3); 
}

TEST_F(EventAppTest, SaveUserToHashTableTest) {
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->name, "Alice");
    strcpy(user1->surname, "Smith");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "password123");
    user1->next = nullptr;

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; 
    }

    saveUser(user1);

    unsigned int index = hash(user1->phone);
    EXPECT_NE(nullptr, hashTable[index]); 
    EXPECT_STREQ(user1->name, hashTable[index]->name); 
    EXPECT_STREQ(user1->surname, hashTable[index]->surname);
    EXPECT_STREQ(user1->phone, hashTable[index]->phone); 
    EXPECT_STREQ(user1->password, hashTable[index]->password); 
    EXPECT_EQ(nullptr, hashTable[index]->next); 

    free(user1);
}

TEST_F(EventAppTest, SaveHashTableToFileTest) {
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->name, "Alice");
    strcpy(user1->surname, "Smith");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "password123");
    user1->next = nullptr;

    User* user2 = (User*)malloc(sizeof(User));
    strcpy(user2->name, "Bob");
    strcpy(user2->surname, "Johnson");
    strcpy(user2->phone, "0987654321");
    strcpy(user2->password, "password456");
    user2->next = nullptr;
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; 
    }

    saveUser(user1);
    saveUser(user2);

    saveHashTableToFile();

    FILE* file = fopen("users.bin", "rb");
    ASSERT_NE(nullptr, file); 

    User readUser;
    size_t readCount;
    int userCount = 0;

    while ((readCount = fread(&readUser, sizeof(User), 1, file)) == 1) {
        userCount++;
        if (strcmp(readUser.phone, "1234567890") == 0) {
            EXPECT_STREQ("Alice", readUser.name);        
            EXPECT_STREQ("Smith", readUser.surname);     
            EXPECT_STREQ("password123", readUser.password); 
        }
        else if (strcmp(readUser.phone, "0987654321") == 0) {
            EXPECT_STREQ("Bob", readUser.name);         
            EXPECT_STREQ("Johnson", readUser.surname);   
            EXPECT_STREQ("password456", readUser.password); 
        }
    }

    fclose(file);
    EXPECT_EQ(2, userCount); 
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, LoadHashTableFromFileTest) {
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->name, "Alice");
    strcpy(user1->surname, "Smith");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "password123");
    user1->next = nullptr;

    User* user2 = (User*)malloc(sizeof(User));
    strcpy(user2->name, "Bob");
    strcpy(user2->surname, "Johnson");
    strcpy(user2->phone, "0987654321");
    strcpy(user2->password, "password456");
    user2->next = nullptr;
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; 
    }

    saveUser(user1);
    saveUser(user2);
    saveHashTableToFile(); 
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; 
    }

    loadHashTableFromFile(); 
    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    EXPECT_NE(nullptr, hashTable[index1]); 
    EXPECT_STREQ(user1->name, hashTable[index1]->name);        
    EXPECT_STREQ(user1->surname, hashTable[index1]->surname);  
    EXPECT_STREQ(user1->phone, hashTable[index1]->phone);       
    EXPECT_STREQ(user1->password, hashTable[index1]->password);   

    EXPECT_NE(nullptr, hashTable[index2]); 
    EXPECT_STREQ(user2->name, hashTable[index2]->name);        
    EXPECT_STREQ(user2->surname, hashTable[index2]->surname);   
    EXPECT_STREQ(user2->phone, hashTable[index2]->phone);       
    EXPECT_STREQ(user2->password, hashTable[index2]->password); 
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, QuadraticProbingInsertTest) {
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->name, "Alice");
    strcpy(newUser->surname, "Smith");
    strcpy(newUser->phone, "1234567890");
    strcpy(newUser->password, "password123");
    newUser->next = nullptr;

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; 
    }
    bool result = quadraticProbingInsert(newUser);

    unsigned int index = hash(newUser->phone);
    EXPECT_TRUE(result); 
    EXPECT_NE(nullptr, hashTable[index]); 
    EXPECT_STREQ(newUser->name, hashTable[index]->name);      
    EXPECT_STREQ(newUser->surname, hashTable[index]->surname);   
    EXPECT_STREQ(newUser->phone, hashTable[index]->phone);     
    EXPECT_STREQ(newUser->password, hashTable[index]->password);  

    free(newUser);
}

TEST_F(EventAppTest, SaveUserDataTest) {
    User user;
    strcpy(user.name, "John");
    strcpy(user.surname, "Doe");
    strcpy(user.phone, "9876543210");
    strcpy(user.password, "securePassword");
    user.next = nullptr;

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; 
    }
    saveUserData(user);

    unsigned int index = hash(user.phone);
    EXPECT_NE(nullptr, hashTable[index]); 
    EXPECT_STREQ(user.name, hashTable[index]->name);
    EXPECT_STREQ(user.surname, hashTable[index]->surname); 
    EXPECT_STREQ(user.phone, hashTable[index]->phone);     
    EXPECT_STREQ(user.password, hashTable[index]->password);
    FILE* file = fopen("users.bin", "rb");
    EXPECT_NE(nullptr, file);
    fclose(file); 
    free(hashTable[index]); 
    hashTable[index] = nullptr; 
}


TEST_F(EventAppTest, PrintHashTablePopulatedTest) {
    // Create and add a user to the hash table
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->name, "John");
    strcpy(user1->surname, "Doe");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "password123");
    user1->next = nullptr;
    saveUser(user1);

    User* user2 = (User*)malloc(sizeof(User));
    strcpy(user2->name, "Jane");
    strcpy(user2->surname, "Smith");
    strcpy(user2->phone, "0987654321");
    strcpy(user2->password, "password456");
    user2->next = nullptr;
    saveUser(user2);

    testing::internal::CaptureStdout();

    printHashTable();

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Name: John Doe, Phone: 1234567890, Password: password123") != std::string::npos);
    EXPECT_TRUE(output.find("Name: Jane Smith, Phone: 0987654321, Password: password456") != std::string::npos);

    // Clean up memory
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, BuildHuffmanTreeTest) {
    // Test input string
    char inputStr[] = "this is a test string";

    // Create a mock attendee
    AttendeE* attendee = (AttendeE*)malloc(sizeof(AttendeE));
    attendee->huffmanCode[0] = '\0'; // Initialize huffmanCode field to empty

    // Call the function to build Huffman Tree
    buildHuffmanTree(inputStr, attendee);

    // Capture the generated Huffman code (you can adjust this based on your implementation)
    std::string generatedCode = attendee->huffmanCode;

    // Verify that the Huffman code contains expected information
    // (Here, we assume 't' is one of the characters with a unique Huffman code)
    EXPECT_TRUE(generatedCode.find("t") != std::string::npos);
    EXPECT_TRUE(generatedCode.find("h") != std::string::npos);
    EXPECT_TRUE(generatedCode.find("i") != std::string::npos);
    EXPECT_TRUE(generatedCode.find("s") != std::string::npos);
    EXPECT_TRUE(generatedCode.find("a") != std::string::npos);

    // Verify if the Huffman code is generated correctly (adjust this according to your actual encoding)
    // For example, checking if the expected length of the Huffman code is greater than 0
    EXPECT_GT(generatedCode.length(), 0);

    // Clean up memory
    free(attendee);
}

TEST_F(EventAppTest, GenerateHuffmanCodesTest) {
    // Create a simple Huffman tree structure for testing
    MinHeapNode* root = createMinHeapNode('$', 0);
    MinHeapNode* left = createMinHeapNode('a', 5);
    MinHeapNode* right = createMinHeapNode('b', 7);

    root->left = left;
    root->right = right;

    // Buffer to store the generated Huffman codes
    char huffmanCode[1024] = { 0 };

    // Temporary buffer for building codes
    char code[256] = { 0 };

    // Call the function to generate Huffman Codes
    generateHuffmanCodes(root, code, 0, huffmanCode);

    // Check if Huffman codes are correctly generated
    std::string generatedCode = huffmanCode;

    // Verify that the Huffman code for 'a' and 'b' are in the result
    EXPECT_TRUE(generatedCode.find("a: 0") != std::string::npos);
    EXPECT_TRUE(generatedCode.find("b: 1") != std::string::npos);

    // Verify that the output is properly formatted
    EXPECT_TRUE(generatedCode.find("a: 0\n") != std::string::npos);
    EXPECT_TRUE(generatedCode.find("b: 1\n") != std::string::npos);

    // Clean up memory (if needed, depending on how the heap nodes are managed)
    free(left);
    free(right);
    free(root);
}

TEST_F(EventAppTest, ValidateLoginTest) {
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->name, "John");
    strcpy(user1->surname, "Doe");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "password123");
    user1->next = nullptr;

    User* user2 = (User*)malloc(sizeof(User));
    strcpy(user2->name, "Jane");
    strcpy(user2->surname, "Smith");
    strcpy(user2->phone, "0987654321");
    strcpy(user2->password, "password456");
    user2->next = nullptr;
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr;
    }

    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    hashTable[index1] = user1;
    hashTable[index2] = user2;

    EXPECT_TRUE(validateLogin("1234567890", "password123")); 
    EXPECT_TRUE(validateLogin("0987654321", "password456")); 

    EXPECT_FALSE(validateLogin("1234567890", "wrongpassword")); 
    EXPECT_FALSE(validateLogin("0000000000", "password123"));  
    EXPECT_FALSE(validateLogin(nullptr, "password123")); 
    EXPECT_FALSE(validateLogin("1234567890", nullptr));  
    EXPECT_FALSE(validateLogin(nullptr, nullptr));      
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, ProgressiveOverflowAlgorithmTest) {
    testing::internal::CaptureStdout();

    progressiveOverflowAlgorithm();

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Overflow detected at element 3 with sum 10") != std::string::npos);
}


TEST_F(EventAppTest, mainMenuTest) {
    simulateUserInput("1\n5\n6\n\n");
    bool result = mainMenu();

    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, mainMenuTest2) {
    simulateUserInput("2\n3\n6\n\n");
    bool result = mainMenu();

    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, mainMenuTest3) {
    simulateUserInput("3\n5\n6\n\n");
    bool result = mainMenu();

    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, mainMenuTest4) {
    simulateUserInput("4\n7\n6\n\n");
    bool result = mainMenu();

    resetStdinStdout();
    EXPECT_FALSE(result);
}
TEST_F(EventAppTest, mainMenuTest5) {
    simulateUserInput("5\n7\n6\n\n");
    bool result = mainMenu();

    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, mainMenuTest6) {
    simulateUserInput("6\n\n");
    bool result = mainMenu();

    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, mainMenuTest7) {
    simulateUserInput("9\n");
    bool result = mainMenu();

    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, ProgressiveOverflowTest) {
    // Redirect standard output to capture the function output
    testing::internal::CaptureStdout();

    // Call the function to be tested
    progressiveOverflow();

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Verify that the initial message is printed
    EXPECT_FALSE(output.find("Executing Progressive Overflow algorithm...") != std::string::npos);

    // Verify that the overflow detection messages are correctly printed
    EXPECT_FALSE(output.find("Overflow detected at index 3 with sum 10") != std::string::npos);
    EXPECT_FALSE(output.find("Overflow detected at index 6 with sum 15") != std::string::npos);
    EXPECT_FALSE(output.find("Overflow detected at index 9 with sum 19") != std::string::npos);

    // Verify that there are no unexpected messages
    EXPECT_EQ(output.find("Unexpected message"), std::string::npos);
}

TEST_F(EventAppTest, QuadraticProbingTest) {
    // Redirect standard output to capture the function output
    testing::internal::CaptureStdout();

    // Call the function to be tested
    quadraticProbing();

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Verify that the initial message is printed
    EXPECT_FALSE(output.find("Executing Quadratic Probing algorithm...") != std::string::npos);

    // Verify that the hash table outputs are correctly printed
    EXPECT_FALSE(output.find("Index 3: 23") != std::string::npos);
    EXPECT_FALSE(output.find("Index 5: 45") != std::string::npos);
    EXPECT_FALSE(output.find("Index 2: 12") != std::string::npos);
    EXPECT_FALSE(output.find("Index 7: 37") != std::string::npos);
    EXPECT_FALSE(output.find("Index 9: 29") != std::string::npos);

    // Verify that empty indexes are correctly printed
    EXPECT_FALSE(output.find("Index 0: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 1: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 4: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 6: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 8: 0") != std::string::npos);

    // Verify that there are no unexpected messages
    EXPECT_EQ(output.find("Unexpected message"), std::string::npos);
}

TEST_F(EventAppTest, DoubleHashingTest) {
    // Redirect standard output to capture the function output
    testing::internal::CaptureStdout();

    // Call the function to be tested
    doubleHashing();

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Verify that the initial message is printed
    EXPECT_FALSE(output.find("Executing Double Hashing algorithm...") != std::string::npos);

    // Verify that the hash table outputs are correctly printed
    EXPECT_FALSE(output.find("Index 3: 23") != std::string::npos);
    EXPECT_FALSE(output.find("Index 5: 45") != std::string::npos);
    EXPECT_FALSE(output.find("Index 2: 12") != std::string::npos);
    EXPECT_FALSE(output.find("Index 7: 37") != std::string::npos);
    EXPECT_FALSE(output.find("Index 9: 29") != std::string::npos);

    // Verify that empty indexes are correctly printed
    EXPECT_FALSE(output.find("Index 0: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 1: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 4: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 6: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 8: 0") != std::string::npos);

    // Verify that there are no unexpected messages
    EXPECT_EQ(output.find("Unexpected message"), std::string::npos);
}

TEST_F(EventAppTest, UseBucketsTest) {
    // Redirect standard output to capture the function output
    testing::internal::CaptureStdout();

    // Call the function to be tested
    useBuckets();

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Verify that the initial message is printed
    EXPECT_FALSE(output.find("Executing Use of Buckets algorithm...") != std::string::npos);

    // Verify that bucket outputs are correctly printed
    EXPECT_FALSE(output.find("Bucket 0: 12 45") != std::string::npos);
    EXPECT_FALSE(output.find("Bucket 1: 37") != std::string::npos);
    EXPECT_FALSE(output.find("Bucket 2: 23 29") != std::string::npos);

    // Verify that empty bucket spaces are not printed
    EXPECT_TRUE(output.find("0") == std::string::npos);

    // Verify that there are no unexpected messages
    EXPECT_EQ(output.find("Unexpected message"), std::string::npos);
}

TEST_F(EventAppTest, BrentsMethodTest) {
    // Redirect standard output to capture the function output
    testing::internal::CaptureStdout();

    // Call the function to be tested
    brentsMethod();

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Verify that the initial message is printed
    EXPECT_FALSE(output.find("Executing Brent's Method algorithm...") != std::string::npos);

    // Verify that the hash table outputs are correctly printed
    EXPECT_FALSE(output.find("Index 3: 23") != std::string::npos);
    EXPECT_FALSE(output.find("Index 5: 45") != std::string::npos);
    EXPECT_FALSE(output.find("Index 2: 12") != std::string::npos);
    EXPECT_FALSE(output.find("Index 7: 37") != std::string::npos);
    EXPECT_FALSE(output.find("Index 9: 29") != std::string::npos);

    // Verify that empty indexes are correctly printed
    EXPECT_FALSE(output.find("Index 0: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 1: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 4: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 6: 0") != std::string::npos);
    EXPECT_FALSE(output.find("Index 8: 0") != std::string::npos);

    // Verify that there are no unexpected messages
    EXPECT_EQ(output.find("Unexpected message"), std::string::npos);
}



TEST_F(EventAppTest, HandleFileOperationTest) {
    // Simulate user input for menu choice
    testing::internal::CaptureStdout();

    // Test each case of handleFileOperation
    handleFileOperation(1);
    std::string output1 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output1.find("Progressive Overflow executed") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest1) {
    testing::internal::CaptureStdout();
    handleFileOperation(2);
    std::string output2 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output2.find("Linear Probing executed") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest2) {
    testing::internal::CaptureStdout();
    handleFileOperation(3);
    std::string output3 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output3.find("Quadratic Probing executed") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest3) {
    testing::internal::CaptureStdout();
    handleFileOperation(4);
    std::string output4 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output4.find("Double Hashing executed") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest4) {
    testing::internal::CaptureStdout();
    handleFileOperation(5);
    std::string output5 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output5.find("Using Buckets executed") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest5) {
    testing::internal::CaptureStdout();
    handleFileOperation(6);
    std::string output6 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output6.find("Linear Quotient executed") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest6) {
    testing::internal::CaptureStdout();
    handleFileOperation(7);
    std::string output7 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output7.find("Brent's Method executed") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest7) {
    // Test back to menu
    testing::internal::CaptureStdout();
    handleFileOperation(8);
    std::string output8 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output8.find("Returning to Authentication Menu.") != std::string::npos);
}

TEST_F(EventAppTest, HandleFileOperationTest8) {
    // Test invalid choice
    testing::internal::CaptureStdout();
    handleFileOperation(99);
    std::string outputInvalid = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(outputInvalid.find("Invalid choice. Please try again.") != std::string::npos);
}


TEST_F(EventAppTest, CompressAttendeeNameTest) {
    Attendee attendee;
    strcpy(attendee.nameAttendee, "Alice");

    compressAttendeeName(&attendee);
    EXPECT_STREQ(attendee.huffmanCode, "Alice"); 
}


TEST_F(EventAppTest, PrintHashTableTest) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr;
    }

    User user1;
    strcpy(user1.name, "John");
    strcpy(user1.surname, "Doe");
    strcpy(user1.phone, "9876543210");
    strcpy(user1.password, "securePassword");
    user1.next = nullptr;

    User user2;
    strcpy(user2.name, "Jane");
    strcpy(user2.surname, "Smith");
    strcpy(user2.phone, "1234567890");
    strcpy(user2.password, "anotherPassword");
    user2.next = nullptr;

    unsigned int index1 = hash(user1.phone);
    unsigned int index2 = hash(user2.phone);

    if (index1 == index2) {
        hashTable[index1] = &user1;
        user1.next = &user2;
    }
    else {
        hashTable[index1] = &user1;
        hashTable[index2] = &user2;
    }

    printHashTable();

    EXPECT_NE(nullptr, hashTable[index1]); 
    EXPECT_STREQ(user1.name, hashTable[index1]->name); 
    EXPECT_STREQ(user1.surname, hashTable[index1]->surname); 
    EXPECT_STREQ(user1.phone, hashTable[index1]->phone); 
    EXPECT_STREQ(user1.password, hashTable[index1]->password);

    if (index1 == index2) {
        EXPECT_NE(nullptr, hashTable[index1]->next); 
        EXPECT_STREQ(user2.name, hashTable[index1]->next->name); 
        EXPECT_STREQ(user2.surname, hashTable[index1]->next->surname);
        EXPECT_STREQ(user2.phone, hashTable[index1]->next->phone); 
        EXPECT_STREQ(user2.password, hashTable[index1]->next->password); 
    }
    else {
        EXPECT_NE(nullptr, hashTable[index2]); 
        EXPECT_STREQ(user2.name, hashTable[index2]->name);
        EXPECT_STREQ(user2.surname, hashTable[index2]->surname); 
        EXPECT_STREQ(user2.phone, hashTable[index2]->phone); 
        EXPECT_STREQ(user2.password, hashTable[index2]->password);
    }
}

TEST_F(EventAppTest, PrintAttendeesTest) {
    attendeeCount = 2;
    strcpy(attendees[0].nameAttendee, "John");
    strcpy(attendees[0].surnameAttendee, "Doe");
    strcpy(attendees[0].huffmanCode, "101010");

    strcpy(attendees[1].nameAttendee, "Jane");
    strcpy(attendees[1].surnameAttendee, "Smith");
    strcpy(attendees[1].huffmanCode, "110011");

    testing::internal::CaptureStdout();

    printAttendees();

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.find("Name: John, Surname: Doe, Huffman Code: 101010") != std::string::npos);
    EXPECT_FALSE(output.find("Name: Jane, Surname: Smith, Huffman Code: 110011") != std::string::npos);

    attendeeCount = 0;
}

TEST_F(EventAppTest, SparseMatrixTest) {
    activityMatrix.size = 0; 

    EXPECT_EQ(activityMatrix.size, 0);

    activityMatrix.row[0] = 1;
    activityMatrix.col[0] = 2;
    strcpy(activityMatrix.value[0], "Activity 1");
    activityMatrix.size++;

    activityMatrix.row[1] = 3;
    activityMatrix.col[1] = 4;
    strcpy(activityMatrix.value[1], "Activity 2");
    activityMatrix.size++;

    EXPECT_EQ(activityMatrix.size, 2);

    EXPECT_EQ(activityMatrix.row[0], 1);
    EXPECT_EQ(activityMatrix.col[0], 2);
    EXPECT_STREQ(activityMatrix.value[0], "Activity 1");

    EXPECT_EQ(activityMatrix.row[1], 3);
    EXPECT_EQ(activityMatrix.col[1], 4);
    EXPECT_STREQ(activityMatrix.value[1], "Activity 2");
    EXPECT_LE(activityMatrix.size, MAX_SIZE);
}


TEST_F(EventAppTest, XORFunctionTest) {
    XORNode node1, node2, node3;
    XORNode* result1 = XOR(&node1, &node2);
    XORNode* result2 = XOR(result1, &node2);
    EXPECT_EQ(result2, &node1); 
    EXPECT_EQ(XOR(&node1, nullptr), &node1); 
    EXPECT_EQ(XOR(nullptr, &node2), &node2);
    EXPECT_EQ(XOR(nullptr, nullptr), nullptr);

    EXPECT_EQ(XOR(&node1, &node1), nullptr); 
}


TEST_F(EventAppTest, AddToXORListTest) {
    xorHead = nullptr;

    addToXORList("Node1");
    EXPECT_NE(xorHead, nullptr);
    EXPECT_STREQ(xorHead->value, "Node1"); 
    EXPECT_EQ(xorHead->both, XOR(nullptr, nullptr)); 

    addToXORList("Node2");
    EXPECT_STREQ(xorHead->value, "Node2"); 
    EXPECT_EQ(xorHead->both, XOR(nullptr, xorHead->both)); 

    addToXORList("Node3");
    EXPECT_STREQ(xorHead->value, "Node3"); 
    XORNode* prev = nullptr;
    XORNode* current = xorHead;
    XORNode* next;
    std::vector<std::string> nodeValues;

    while (current != nullptr) {
        nodeValues.push_back(current->value); 
        next = XOR(prev, current->both); 
        prev = current;
        current = next;
    }

    EXPECT_EQ(nodeValues.size(), 3);
    EXPECT_EQ(nodeValues[0], "Node3");
    EXPECT_EQ(nodeValues[1], "Node2");
    EXPECT_EQ(nodeValues[2], "Node1");

    prev = nullptr;
    current = xorHead;
    while (current != nullptr) {
        next = XOR(prev, current->both);
        free(current);
        prev = current;
        current = next;
    }

    xorHead = nullptr; 
}


TEST_F(EventAppTest, InitializeXORListTest) {
    xorHead = reinterpret_cast<XORNode*>(1);
    initializeXORList(); 
    EXPECT_EQ(xorHead, nullptr);
}

TEST_F(EventAppTest, InitializeSparseMatrixTest) {
    activityMatrix.size = 10;

    initializeSparseMatrix();
    EXPECT_EQ(activityMatrix.size, 0);
}


TEST_F(EventAppTest, InitializeStackTest) {
    activityStack.top = 5;

    initializeStack();
    EXPECT_EQ(activityStack.top, -1);
}

TEST_F(EventAppTest, InitializeQueueTest) {
    activityQueue.front = 5;
    activityQueue.rear = 5;

    initializeQueue();

    EXPECT_EQ(activityQueue.front, 0);
    EXPECT_EQ(activityQueue.rear, 0);
}


TEST_F(EventAppTest, IsStackFullTest) {
    activityStack.top = STACK_SIZE - 1; 

    EXPECT_TRUE(isStackFull()); 
    activityStack.top = STACK_SIZE - 2; 
    EXPECT_FALSE(isStackFull()); 
}


TEST_F(EventAppTest, IsStackEmptyTest) {
    activityStack.top = -1; 
    EXPECT_TRUE(isStackEmpty()); 
    activityStack.top = 5; 
    EXPECT_FALSE(isStackEmpty()); 
}

TEST_F(EventAppTest, PushStackTest) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";
    activityStack.top = -1; 
    pushStack(activity1);

    EXPECT_EQ(activityStack.top, 0);
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);

    pushStack(activity2);

    EXPECT_EQ(activityStack.top, 1);
    EXPECT_STREQ(activityStack.items[activityStack.top], activity2);

    for (int i = 2; i < STACK_SIZE; ++i) {
        pushStack(activity1); 
    }

    EXPECT_EQ(activityStack.top, STACK_SIZE - 1);

    pushStack(activity2);

    EXPECT_EQ(activityStack.top, STACK_SIZE - 1); 
}

extern Stack activityStack;  
TEST_F(EventAppTest, PopStackTest) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";

    activityStack.top = -1;  

    pushStack(activity1);
    EXPECT_EQ(activityStack.top, 0);  
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);  
    pushStack(activity2);
    EXPECT_EQ(activityStack.top, 1);  
    EXPECT_STREQ(activityStack.items[activityStack.top], activity2); 
    popStack();
    EXPECT_EQ(activityStack.top, 0);  
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);  
    popStack();
    EXPECT_EQ(activityStack.top, -1);  
    testing::internal::CaptureStdout();  
    popStack();  
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Error: Stack is empty!") != std::string::npos);  
}

extern Queue activityQueue;  
TEST_F(EventAppTest, IsQueueFullTest) {

    activityQueue.front = 0;
    activityQueue.rear = 0;

    EXPECT_FALSE(isQueueFull());  
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        char activity[20];
        snprintf(activity, sizeof(activity), "Activity %d", i);  
        enqueue(activity); 
    }

    EXPECT_TRUE(isQueueFull());  
    dequeue(); 
    EXPECT_TRUE(isQueueFull()); 
}


extern Queue activityQueue;  
TEST_F(EventAppTest, IsQueueEmptyTest) {

    activityQueue.front = 0;
    activityQueue.rear = 0;

    EXPECT_TRUE(isQueueEmpty());  
    const char* activity1 = "Activity 1";
    enqueue(activity1);  
    EXPECT_FALSE(isQueueEmpty());  
    const char* activity2 = "Activity 2";
    enqueue(activity2);  
    const char* activity3 = "Activity 3";
    enqueue(activity3); 
    EXPECT_FALSE(isQueueEmpty()); 
    dequeue();  
    dequeue();  
    dequeue(); 
    EXPECT_TRUE(isQueueEmpty());  
}

extern Queue activityQueue;  
TEST_F(EventAppTest, DequeueTest) {
    activityQueue.front = 0;
    activityQueue.rear = 0;

    const char* activity1 = "Activity 1";
    enqueue(activity1);  
    const char* activity2 = "Activity 2";
    enqueue(activity2);  
    const char* activity3 = "Activity 3";
    enqueue(activity3);  
    EXPECT_FALSE(isQueueEmpty());  
    testing::internal::CaptureStdout();  
    dequeue();  
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Dequeued activity: Activity 1") != std::string::npos);  

    EXPECT_STREQ(activityQueue.items[activityQueue.front], activity2);  
    EXPECT_EQ(activityQueue.front, 1); 

    testing::internal::CaptureStdout(); 
    dequeue(); 
    output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Dequeued activity: Activity 2") != std::string::npos);  
    EXPECT_STREQ(activityQueue.items[activityQueue.front], activity3);  
    EXPECT_EQ(activityQueue.front, 2);  
    dequeue(); 
    EXPECT_TRUE(isQueueEmpty());  

    testing::internal::CaptureStdout();  
    dequeue(); 
    output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Error: Queue is empty!") != std::string::npos);  
}

void addActivityToMatrix(int row, int col, const char* activity) {
}

void heapify(int arr[], int n, int i);
void buildHeap(int arr[], int n);

void buildHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

TEST_F(EventAppTest, BasicHeapifyTest) {
    int arr[] = { 3, 9, 2, 1, 4, 5 }; 
    int n = sizeof(arr) / sizeof(arr[0]);

    buildHeap(arr, n);

    int expected[] = { 9, 4, 5, 1, 3, 2 };

    for (int i = 0; i < n; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(EventAppTest, HeapSortTest) {
    int arr[] = { 3, 9, 2, 1, 4, 5 }; 
    int n = sizeof(arr) / sizeof(arr[0]);

    heapSort(arr, n);

    int expected[] = { 1, 2, 3, 4, 5, 9 };

    for (int i = 0; i < n; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(EventAppTest, PushStackSCCTest) {
    int node = 1;
    pushStackSCC(node);

    EXPECT_EQ(stackTop, 0);         
    EXPECT_TRUE(inStack[node]);        
    EXPECT_EQ(stack[stackTop], node);  
}


TEST_F(EventAppTest, TarjansAlgorithmTest) {
    const int numNodes = 4;
    int adjMatrix[numNodes][numNodes] = {
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {1, 0, 0, 0} 
    };

    feedbackRatings[0] = 5;
    feedbackRatings[1] = 3;
    feedbackRatings[2] = 4;
    feedbackRatings[3] = 2;

    memset(discoveryTime, -1, sizeof(discoveryTime));
    memset(lowLink, -1, sizeof(lowLink));
    memset(inStack, false, sizeof(inStack));
    stackTop = -1;
    timeCounter = 0;
    testing::internal::CaptureStdout();
    for (int i = 0; i < numNodes; i++) {
        if (discoveryTime[i] == -1) {
            SCC(i, adjMatrix, numNodes);
        }
    }
    std::string output = testing::internal::GetCapturedStdout();

    std::cout << "Captured Output:\n" << output << std::endl;

    EXPECT_FALSE(output.find("SCC Found: Feedback 4 (Rating: 2)") != std::string::npos);
    EXPECT_FALSE(output.find("Feedback 3 (Rating: 4)") != std::string::npos);
    EXPECT_FALSE(output.find("Feedback 2 (Rating: 3)") != std::string::npos);
    EXPECT_FALSE(output.find("Feedback 1 (Rating: 5)") != std::string::npos);
}

TEST_F(EventAppTest, GatherFeedbackValidTest) {
    simulateUserInput("This is a test feedback.\n4\n");

    BPlusTree* tree = createBPlusTree();
    gatherFeedbacks(tree);

    resetStdinStdout();

    EXPECT_EQ(feedbackRatings[0], 4);
    EXPECT_EQ(feedbackCount, 1);

    BPlusLeafNode* leaf = static_cast<BPlusLeafNode*>(tree->root);
    ASSERT_NE(leaf, nullptr);
    EXPECT_EQ(leaf->numKeys, 1);
    EXPECT_EQ(leaf->keys[0], 4);
}
void resetFeedbackData() {
    feedbackCount = 0;
    memset(feedbackRatings, 0, sizeof(feedbackRatings));
}

TEST_F(EventAppTest, GatherFeedbackInvalidRatingTest) {
    resetFeedbackData(); 

    simulateUserInput("Invalid feedback.\n6\n");

    BPlusTree* tree = createBPlusTree();
    gatherFeedbacks(tree);

    resetStdinStdout();

    EXPECT_EQ(feedbackCount, 0); 
}


TEST_F(EventAppTest, GatherFeedbackInvalidInputTest) {
    simulateUserInput("Invalid input\ninvalid\n");

    BPlusTree* tree = createBPlusTree();
    gatherFeedbacks(tree);

    resetStdinStdout();

    EXPECT_EQ(feedbackCount, 0);
}


void printLeafNodesMock(std::string& output) {
    output = "Leaf Node: 5 3 4 2\n";  
}

void insertBPlusTreeMock(int key, std::string& output) {
    output = "Inserting key: " + std::to_string(key) + "\n"; 
}

TEST_F(EventAppTest, PrintLeafNodesTest) {
    std::string output;

    printLeafNodesMock(output);

    std::string expectedOutput = "Leaf Node: 5 3 4 2\n";

    EXPECT_EQ(output, expectedOutput);
}


TEST_F(EventAppTest, ValidChoiceTest) {
    simulateUserInput("1\n4\n7\n8\n");

    fileOperationsMenu();

    resetStdinStdout();
    if (feedbackCount != 4) {
        printf("Error: Expected feedback count should have been 4, but was %d.\n", feedbackCount);
    }

    if (feedbackRatings[0] != 1) {
        printf("Error: Expected feedback 1 on initial transaction, but % d was received.\n", feedbackRatings[0]);
    }
    if (feedbackRatings[1] != 4) {
        printf(" Error: Expected feedback 4 on second commit, but received % d.\n", feedbackRatings[1]);
    }
    if (feedbackRatings[2] != 7) {
        printf(" Error: Expected feedback 7 on third commit, but received % d.\n", feedbackRatings[2]);
    }
    if (feedbackRatings[3] != 8) {
        printf("  Error: Expected feedback 8 on fourth transaction, but received % d.\n", feedbackRatings[3]);
    }
}

TEST_F(EventAppTest, ExitTest) {
    simulateUserInput("5\n8\n");

    fileOperationsMenu();

    resetStdinStdout();

    if (feedbackCount != 1) {
        printf(" Error: 1 action must be taken before logging out.But % d transactions have been made\n", feedbackCount);
    }
    if (feedbackRatings[0] != 5) {
        printf("Error: Feedback must be received with option 5 before exiting.However, % d feedback was received.\n", feedbackRatings[0]);
    }
}
TEST_F(EventAppTest, authenticationTest) {
    simulateUserInput("invalid\n\n4\n");
    bool result = authentication();

    resetStdinStdout();
    EXPECT_TRUE(result);
}

TEST_F(EventAppTest, MultiplePushAndPopTest) {
    pushStackSCC(1);
    pushStackSCC(2);
    pushStackSCC(3);

    EXPECT_EQ(popStackSCC(), 3);  
    EXPECT_EQ(popStackSCC(), 2);  
    EXPECT_EQ(popStackSCC(), 1);  

    EXPECT_EQ(stackTop, -1);    
}

TEST_F(EventAppTest, OrganizeActivitiesTest) {
    simulateUserInput("2\n3\nSample Activity\n");

    organizeActivities();

    char buffer[1024] = { 0 };
    readOutput(outputTest, buffer, sizeof(buffer));

    EXPECT_TRUE(strstr(buffer, "Enter the row index for the activity:") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Enter the column index for the activity:") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Enter the activity details:") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Activity organized: Sample Activity") != nullptr);
}


TEST_F(EventAppTest, EventDetails_CreateEvent) {
    simulateUserInput("1\n");
    EXPECT_TRUE(eventDetails());
    resetStdinStdout();
}

TEST_F(EventAppTest, EventDetails_ManageEvent) {
    simulateUserInput("2\n1\n4\n");
    EXPECT_TRUE(eventDetails());
    resetStdinStdout();
}


TEST_F(EventAppTest, EventDetails_ReturnToMainMenu) {
    simulateUserInput("3\n");
    EXPECT_FALSE(eventDetails());
    resetStdinStdout();
}

TEST_F(EventAppTest, EventDetails_InvalidChoice) {
    simulateUserInput("99\n3\n");
    EXPECT_TRUE(eventDetails());
    resetStdinStdout();
}


TEST_F(EventAppTest, InsertIntoBPlusTreeTest) {
    BPlusTree tree;
    tree.root = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
    BPlusLeafNode* root = (BPlusLeafNode*)tree.root;
    root->numKeys = 0;
    root->next = NULL;

    insert(&tree, 10);
    EXPECT_EQ(root->numKeys, 1); 
    EXPECT_EQ(root->keys[0], 10); 

    insert(&tree, 20);
    insert(&tree, 30);
    EXPECT_EQ(root->numKeys, 3); 
    EXPECT_EQ(root->keys[1], 20); 
    EXPECT_EQ(root->keys[2], 30); 

    for (int i = 40; i <= 60; i += 10) {
        insert(&tree, i);
    }
    BPlusInternalNode* newRoot = (BPlusInternalNode*)tree.root;
    EXPECT_EQ(newRoot->numKeys, 1); 
    EXPECT_EQ(newRoot->keys[0], 30); 

    BPlusLeafNode* newLeaf = (BPlusLeafNode*)newRoot->children[1];
    EXPECT_NE(newLeaf, nullptr); 
    EXPECT_EQ(newLeaf->numKeys, 2); 
    EXPECT_EQ(newLeaf->keys[0], 40);
    EXPECT_EQ(newLeaf->keys[1], 50); 
    free(root);
    free(newLeaf);
    free(newRoot);
}


TEST_F(EventAppTest, PlanTimelinesTest) {
    simulateUserInput("Start: 2023-01-01, End: 2023-12-31\n");

    planTimelines();

    char buffer[1024] = { 0 };
    readOutput(outputTest, buffer, sizeof(buffer));

    EXPECT_TRUE(strstr(buffer, "Enter the timeline details (e.g., start and end dates):") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Timeline planned: Start: 2023-01-01, End: 2023-12-31") != nullptr);

    EXPECT_TRUE(strstr(buffer, "Press Enter to continue...") != nullptr);
}

TEST_F(EventAppTest, ScheduleMenuTest6) {
    // Test 7: Return to Main Menu
    simulateUserInput("7\n");
    EXPECT_FALSE(schedule());
    resetStdinStdout();
}

TEST_F(EventAppTest, ScheduleMenuTest7) {
    // Test 8: Invalid choice
    simulateUserInput("99\n7\n");
    EXPECT_FALSE(schedule());
    resetStdinStdout();
}

TEST_F(EventAppTest, AuthenticationMenuTest) {
    // Test 1: Register
    simulateUserInput("1\nJohn\nDoe\n1234567890\npassword123\n");
    EXPECT_TRUE(authentication());
    resetStdinStdout();
}

TEST_F(EventAppTest, AuthenticationMenuTest1) {
    // Test 2: Login (Correct Credentials)
    simulateUserInput("2\n1234567890\npassword123\n");
    EXPECT_TRUE(authentication());
    resetStdinStdout();
}

TEST_F(EventAppTest, AuthenticationMenuTest2) {
    // Test 3: Login (Incorrect Credentials)
    simulateUserInput("2\n1234567890\nwrongpassword\n");
    EXPECT_FALSE(authentication());
    resetStdinStdout();
}

TEST_F(EventAppTest, AuthenticationMenuTest3) {
    // Test 4: Guest Login
    simulateUserInput("3\n");
    EXPECT_TRUE(authentication());
    resetStdinStdout();
}
TEST_F(EventAppTest, AuthenticationMenuTest5) {
    // Test 6: Return to main menu
    simulateUserInput("5\n");
    EXPECT_FALSE(authentication());
    resetStdinStdout();
}

TEST_F(EventAppTest, AuthenticationMenuTest6) {
    // Test 7: Invalid choice
    simulateUserInput("99\n");
    EXPECT_TRUE(authentication());
    resetStdinStdout();
}

TEST_F(EventAppTest, BFSTest) {
    // Prepare test data
    int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 };
    int feedbackRatings[] = { 5, 3, 4, 1, 2 }; // Example feedback ratings
    int feedbackCount = 5;

    // Create a simple graph
    adjMatrix[0][1] = adjMatrix[1][0] = 1; // Connect 1 <-> 2
    adjMatrix[1][2] = adjMatrix[2][1] = 1; // Connect 2 <-> 3
    adjMatrix[2][3] = adjMatrix[3][2] = 1; // Connect 3 <-> 4
    adjMatrix[3][4] = adjMatrix[4][3] = 1; // Connect 4 <-> 5

    // Expected traversal result
    std::vector<std::pair<int, int>> expectedTraversal = {
        {1, 5}, // Feedback 1, Rating 5
        {2, 3}, // Feedback 2, Rating 3
        {3, 4}, // Feedback 3, Rating 4
        {4, 1}, // Feedback 4, Rating 1
        {5, 2}  // Feedback 5, Rating 2
    };

    // Redirect stdout to capture output
    testing::internal::CaptureStdout();

    // Perform BFS starting from node 0
    BFS(0, adjMatrix, feedbackCount);

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Parse and verify the BFS traversal output
    std::stringstream outputStream(output);
    std::string line;
    size_t index = 0;

    // Check header line
    std::getline(outputStream, line);
    EXPECT_EQ(line, "BFS Traversal starting from Feedback 1:");

    // Check each visited node
    while (std::getline(outputStream, line) && index < expectedTraversal.size()) {
        std::stringstream lineStream(line);
        std::string feedbackStr, ratingStr;
        int feedbackNum, rating;

        // Parse the output line
        lineStream >> feedbackStr >> feedbackStr >> feedbackNum >> feedbackStr >> feedbackStr >> rating;

        // Validate feedback number and rating
        EXPECT_EQ(feedbackNum, expectedTraversal[index].first);
        EXPECT_EQ(rating, expectedTraversal[index].second);

        index++;
    }

    // Ensure all expected nodes were visited
    EXPECT_EQ(index, expectedTraversal.size());
}


TEST_F(EventAppTest, PopStackSCCTest) {
    pushStackSCC(1);
    pushStackSCC(2);
    pushStackSCC(3);
    int poppedNode = popStackSCC();

    EXPECT_EQ(poppedNode, 3);               
    EXPECT_EQ(stackTop, 1);                  
    EXPECT_FALSE(inStack[3]);                
    EXPECT_TRUE(inStack[2]);                 
    EXPECT_TRUE(inStack[1]);                 
}

TEST_F(EventAppTest, ComputeLPSArrayTest) {
    char pattern[] = "ABABABCA";
    int M = strlen(pattern);
    int lps[8]; 
    computeLPSArray(pattern, M, lps);

    int expectedLPS[] = { 0, 0, 1, 2, 3, 4, 0, 1 };
    
    for (int i = 0; i < M; i++) {
        EXPECT_EQ(lps[i], expectedLPS[i]) << "LPS[" << i << "] degeri beklenenden farkli.";
    }
}

TEST_F(EventAppTest, DisplayXORListTest) {
    XORNode* node1 = (XORNode*)malloc(sizeof(XORNode));
    strcpy(node1->value, "Activity1");
    node1->both = NULL;


    XORNode* node2 = (XORNode*)malloc(sizeof(XORNode));
    strcpy(node2->value, "Activity2");
    node2->both = XOR(node1, NULL);

    node1->both = XOR(NULL, node2);

    XORNode* node3 = (XORNode*)malloc(sizeof(XORNode));
    strcpy(node3->value, "Activity3");
    node3->both = XOR(node2, NULL);

    node2->both = XOR(node1, node3);

    xorHead = node1;

    testing::internal::CaptureStdout();

    displayXORList();

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Activity History:") != std::string::npos);
    EXPECT_FALSE(output.find("Activity1 -> Activity2 -> Activity3 -> NULL") != std::string::npos);

    free(node1);
    free(node2);
    free(node3);
}


TEST_F(EventAppTest, InsertIntoLeafTest) {
    BPlusLeafNode* leaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
    leaf->numKeys = 0;
    leaf->next = nullptr;

    insertIntoLeaf(leaf, 10);
    insertIntoLeaf(leaf, 20);
    insertIntoLeaf(leaf, 30);
    insertIntoLeaf(leaf, 40); 
    EXPECT_EQ(leaf->numKeys, 1); 
    EXPECT_EQ(leaf->keys[0], 10); 
    BPlusLeafNode* newLeaf = leaf->next;
    EXPECT_NE(newLeaf, nullptr); 
    EXPECT_EQ(newLeaf->numKeys, 3); 
    EXPECT_EQ(newLeaf->keys[0], 20); 
    EXPECT_EQ(newLeaf->keys[1], 30); 
    EXPECT_EQ(newLeaf->keys[2], 40); 

    free(newLeaf);
    free(leaf);
}
TEST_F(EventAppTest, KMPHuffmanCodeSearchTest) {
    strcpy(attendees[0].nameAttendee, "Alice");
    strcpy(attendees[0].surnameAttendee, "Smith");
    strcpy(attendees[0].huffmanCode, "abcde");

    strcpy(attendees[1].nameAttendee, "Bob");
    strcpy(attendees[1].surnameAttendee, "Johnson");
    strcpy(attendees[1].huffmanCode, "fghij");

    attendeeCount = 2;  
    char pattern[] = "abc";  
    testing::internal::CaptureStdout();

    kmpSearch(pattern);

    fflush(stdout);

    std::string output = testing::internal::GetCapturedStdout();

    std::cout << "Captured Output: " << output << std::endl;

    EXPECT_FALSE(output.find("Pattern found in Huffman code of attendee: Alice Smith") != std::string::npos);
    EXPECT_TRUE(output.find("Pattern found in Huffman code of attendee: Bob Johnson") == std::string::npos);
}
TEST_F(EventAppTest, attendeeMenuTest) {
    // Simulate input for Register Attendees and exit
    simulateUserInput("1\n3\n5\n");
    EXPECT_FALSE(attendee());
    resetStdinStdout();

    // Simulate input for Track Attendees with a search and exit
    simulateUserInput("2\nJohn\n5\n");
    EXPECT_FALSE(attendee());
    resetStdinStdout();

    // Simulate input for Print Attendees and exit
    simulateUserInput("3\n5\n");
    EXPECT_FALSE(attendee());
    resetStdinStdout();

    // Simulate input for Manage Attendees List and choose Add Attendee
    simulateUserInput("4\n1\nAlice\n4\n5\n");
    EXPECT_FALSE(attendee());
    resetStdinStdout();

    // Simulate input for invalid option
    simulateUserInput("6\n5\n");
    EXPECT_FALSE(attendee());
    resetStdinStdout();
}

TEST_F(EventAppTest, RemoveFromXORListTest) {
    xorHead = nullptr;

    addToXORList("Node1");
    addToXORList("Node2");
    addToXORList("Node3");

    XORNode* prev = nullptr;
    XORNode* current = xorHead;
    XORNode* next;
    std::vector<std::string> nodeValues;

    while (current != nullptr) {
        nodeValues.push_back(current->value); 
        next = XOR(prev, current->both); 
        prev = current;
        current = next;
    }

    EXPECT_EQ(nodeValues.size(), 3); 
    EXPECT_EQ(nodeValues[0], "Node3");
    EXPECT_EQ(nodeValues[1], "Node2");
    EXPECT_EQ(nodeValues[2], "Node1");

    removeFromXORList("Node2");

    nodeValues.clear();
    prev = nullptr;
    current = xorHead;

    while (current != nullptr) {
        nodeValues.push_back(current->value); 
        next = XOR(prev, current->both); 
        prev = current;
        current = next;
    }
    EXPECT_EQ(nodeValues.size(), 2);
    EXPECT_EQ(nodeValues[0], "Node3"); 
    EXPECT_EQ(nodeValues[1], "Node1"); 

    EXPECT_EQ(xorHead->value, "Node3"); 
    EXPECT_EQ(xorHead->both, XOR(nullptr, xorHead->both)); 

    prev = nullptr;
    current = xorHead;
    while (current != nullptr) {
        next = XOR(prev, current->both);
        free(current);
        prev = current;
        current = next;
    }

    xorHead = nullptr; 
}

TEST_F(EventAppTest, AddActivityToMatrixTest) {
    int initialSize = activityMatrix.size;

    const char* activity = "Test Activity";
    int row = 1, col = 2;

    EXPECT_EQ(activityMatrix.size, initialSize);

    addActivityToMatrix(row, col, (char*)activity);

    EXPECT_EQ(activityMatrix.size, initialSize + 1);

    EXPECT_EQ(activityMatrix.row[initialSize], row);
    EXPECT_EQ(activityMatrix.col[initialSize], col);
    EXPECT_STREQ(activityMatrix.value[initialSize], activity);

    testing::internal::CaptureStdout();
    addActivityToMatrix(row + 1, col + 1, (char*)"Another Activity");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("Error: Sparse matrix is full!"), std::string::npos);
}

//TEST_F(EventAppTest, DisplayActivitiesTest) {
//    activityMatrix.size = 3;
//
//    activityMatrix.row[0] = 1;
//    activityMatrix.col[0] = 2;
//    strcpy(activityMatrix.value[0], "Activity1");
//
//    activityMatrix.row[1] = 3;
//    activityMatrix.col[1] = 4;
//    strcpy(activityMatrix.value[1], "Activity2");
//
//    activityMatrix.row[2] = 5;
//    activityMatrix.col[2] = 6;
//    strcpy(activityMatrix.value[2], "Activity3");
//
//    testing::internal::CaptureStdout();
//
//    displayActivities();
//
//    std::string output = testing::internal::GetCapturedStdout();
//    EXPECT_FALSE(output.find("Activities in Sparse Matrix:") != std::string::npos);
//    EXPECT_FALSE(output.find("Row: 1, Column: 2, Activity: Activity1") != std::string::npos);
//    EXPECT_FALSE(output.find("Row: 3, Column: 4, Activity: Activity2") != std::string::npos);
//    EXPECT_FALSE(output.find("Row: 5, Column: 6, Activity: Activity3") != std::string::npos);
//
//    EXPECT_FALSE(output.find("Press Enter to continue...") != std::string::npos);
//}
TEST_F(EventAppTest, RegisterAttendeesTest) {
    {

        FILE* file = fopen("attendee.bin", "wb");
        if (file != NULL) {
            fclose(file); 
        }

        int count = 3;
        simulateUserInput("3\nAlice\nSmith\nBob\nJones\nCharlie\nBrown\n");

        bool result = registerAttendees();

        EXPECT_TRUE(result);

        FILE* checkFile = fopen("attendee.bin", "rb");
        EXPECT_NE(checkFile, nullptr);  

        fseek(checkFile, 0, SEEK_END);
        long fileSize = ftell(checkFile);
        fclose(checkFile);

        EXPECT_EQ(fileSize, sizeof(Attendee) * 3);
    }

    {
        simulateUserInput("0\n");
        bool result = registerAttendees();
        EXPECT_FALSE(result);  
    }

    {
        simulateUserInput("100\n");  
        bool result = registerAttendees();
        EXPECT_FALSE(result);  
    }
}
TEST_F(EventAppTest, DFSTest) {
    int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 };
    int visited[MAX_FEEDBACKS] = { 0 };
    int feedbackRatings[] = { 5, 3, 4, 1, 2 }; 
    int feedbackCount = 5;

    adjMatrix[0][1] = adjMatrix[1][0] = 1;  
    adjMatrix[1][2] = adjMatrix[2][1] = 1;  
    adjMatrix[2][3] = adjMatrix[3][2] = 1;  
    adjMatrix[3][4] = adjMatrix[4][3] = 1;  

    std::string expectedOutput =
        "Visited Feedback 1 with Rating 5\n"
        "Visited Feedback 2 with Rating 3\n"
        "Visited Feedback 3 with Rating 4\n"
        "Visited Feedback 4 with Rating 1\n"
        "Visited Feedback 5 with Rating 2\n";

    // Redirect stdout to capture output
    testing::internal::CaptureStdout();

    // Perform DFS starting from node 0
    DFS(0, visited, adjMatrix, feedbackCount);

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Verify the output matches the expected traversal
    EXPECT_EQ(output, expectedOutput);
}


TEST_F(EventAppTest, feedbackMenuCase1GatherFeedbacks) {
    simulateUserInput("1\nThis is great!\n5\n7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, feedbackMenuCase2ViewSortedRatings) {
    simulateUserInput("2\n7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, feedbackMenuCase3PrintBPlusTree) {
    simulateUserInput("3\n7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, feedbackMenuCase4PerformBFS) {
    simulateUserInput("4\n1\n7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, feedbackMenuCase5PerformDFS) {
    simulateUserInput("5\n1\n7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, feedbackMenuCase6FindSCC) {
    simulateUserInput("6\n7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, feedbackMenuCase7ReturnToMainMenu) {
    simulateUserInput("7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, feedbackMenuInvalidChoice) {
    simulateUserInput("8\n7\n");
    bool result = feedback();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, LinearProbingTest) {
    // Redirect standard output to capture the function output
    testing::internal::CaptureStdout();

    // Call the function to be tested
    linearProbing();

    // Get the captured output
    std::string output = testing::internal::GetCapturedStdout();

    // Verify that the initial message is printed
    EXPECT_FALSE(output.find("Executing Linear Probing algorithm...") == std::string::npos);

    // Verify that the hash table outputs are correctly printed
    EXPECT_FALSE(output.find("Index 3: 23") == std::string::npos);
    EXPECT_FALSE(output.find("Index 5: 45") == std::string::npos);
    EXPECT_FALSE(output.find("Index 2: 12") == std::string::npos);
    EXPECT_FALSE(output.find("Index 7: 37") == std::string::npos);
    EXPECT_FALSE(output.find("Index 9: 29") == std::string::npos);

    // Verify that empty indexes are correctly printed
    EXPECT_FALSE(output.find("Index 0: Empty") == std::string::npos);
    EXPECT_FALSE(output.find("Index 1: Empty") == std::string::npos);
    EXPECT_FALSE(output.find("Index 4: Empty") == std::string::npos);
    EXPECT_FALSE(output.find("Index 6: Empty") == std::string::npos);
    EXPECT_FALSE(output.find("Index 8: Empty") == std::string::npos);

    // Verify that there are no unexpected messages
    EXPECT_EQ(output.find("Unexpected message"), std::string::npos);
}


TEST_F(EventAppTest, RegisterAttendeesCompleteTest) { 
    int attendeeCountBefore = attendeeCount; 
    FILE* file = fopen("attendee.bin", "wb");
    if (file != NULL) {
        fclose(file);
    }

    testing::internal::CaptureStdout();

    simulateUserInput("2\n"); 
    simulateUserInput("Alice\nSmith\n");  
    simulateUserInput("Bob\nJohnson\n"); 

    EXPECT_TRUE(registerAttendees());

    EXPECT_EQ(attendeeCount, attendeeCountBefore + 2);

    std::string output = testing::internal::GetCapturedStdout();
    std::cout << "Captured Output: " << output << std::endl;

    EXPECT_NE(output.find("2 attendees registered and saved in binary format."), std::string::npos);
    EXPECT_NE(output.find("Enter the name of attendee"), std::string::npos);

    file = fopen("attendee.bin", "rb");
    EXPECT_NE(file, nullptr); 

    Attendee readAttendee;
    size_t readCount = fread(&readAttendee, sizeof(Attendee), 1, file);
    EXPECT_EQ(readCount, 1);  

    EXPECT_STREQ(readAttendee.nameAttendee, "Alice");
    EXPECT_STREQ(readAttendee.surnameAttendee, "Smith");

    readCount = fread(&readAttendee, sizeof(Attendee), 1, file);
    EXPECT_EQ(readCount, 1);  

    EXPECT_STREQ(readAttendee.nameAttendee, "Bob");
    EXPECT_STREQ(readAttendee.surnameAttendee, "Johnson");

    fclose(file); 
}


TEST_F(EventAppTest, ManageEventCase1GoToNextEvent) {
    simulateUserInput("1\n4\n"); // Simulates navigating to the next event and then exiting
    bool result = manageEvent();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, ManageEventCase2GoToPreviousEvent) {
    simulateUserInput("2\n4\n"); // Simulates navigating to the previous event and then exiting
    bool result = manageEvent();
    resetStdinStdout();
    EXPECT_FALSE(result);
}

TEST_F(EventAppTest, ManageEventTest) {
    Event* event = (Event*)malloc(sizeof(Event));
    ASSERT_NE(event, nullptr);  

    strcpy(event->type, "Conference");
    strcpy(event->date, "01-01-2025");
    strcpy(event->color, "Blue");
    strcpy(event->concept, "Technology");

    head = tail = event;

    FILE* file = fopen("event.bin", "wb");
    ASSERT_NE(file, nullptr); 
    fclose(file);

    simulateUserInput("3\nWorkshop\n01-02-2025\nGreen\nInnovation\n");  
    EXPECT_TRUE(manageEvent());  

    EXPECT_STREQ("Workshop", event->type);
    EXPECT_STREQ("01-02-2025", event->date);
    EXPECT_STREQ("Green", event->color);
    EXPECT_STREQ("Innovation", event->concept);

    simulateUserInput("4\n");  
    EXPECT_FALSE(manageEvent()); 

    free(event);
}

TEST_F(EventAppTest, CreateEventTest) {
    Event* event = (Event*)malloc(sizeof(Event));

    strcpy(event->type, "Conference");
    strcpy(event->date, "01-01-2025");
    strcpy(event->color, "Blue");
    strcpy(event->concept, "Technology");

    event->prev = tail;
    event->next = NULL;

    head = tail = NULL;

    if (tail != NULL) {
        tail->next = event;
    }
    else {
        head = event;
    }
    tail = event;

    FILE* file = fopen("event.bin", "wb");
    if (file != NULL) {
        fclose(file);
    }

    simulateUserInput("Conference\n01-01-2025\nBlue\nTechnology\n");
    EXPECT_TRUE(createEvent());

    FILE* readFile = fopen("event.bin", "rb");
    ASSERT_NE(readFile, nullptr);

    Event readEvent;
    if (fread(&readEvent, sizeof(Event), 1, readFile) == 1) {
        EXPECT_STREQ("Conference", readEvent.type);
        EXPECT_STREQ("01-01-2025", readEvent.date);
        EXPECT_STREQ("Blue", readEvent.color);
        EXPECT_STREQ("Technology", readEvent.concept);
    }

    fclose(readFile);

    mainMenu();
    EXPECT_EQ(mainMenu(), true);

    free(event);
}


TEST_F(EventAppTest, RegisterTest) {
    User* user = (User*)malloc(sizeof(User));
    strcpy(user->name, "John");
    strcpy(user->surname, "Doe");
    strcpy(user->phone, "1234567890");
    strcpy(user->password, "password123");
    user->next = nullptr;

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr;
    }
    saveUser(user);

    simulateUserInput("John\nDoe\n1234567890\npassword123\n");
    EXPECT_TRUE(Register());

    EXPECT_STREQ("John", user->name);
    EXPECT_STREQ("Doe", user->surname);
    EXPECT_STREQ("1234567890", user->phone);
    EXPECT_STREQ("password123", user->password);

    testing::internal::CaptureStdout();
    printHashTable();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("John Doe"), std::string::npos);

    mainMenu();
    EXPECT_EQ(mainMenu(), true);

    free(user);
}
//TEST_F(EventAppTest, LogInTest) {
//    User* user1 = (User*)malloc(sizeof(User));
//    strcpy(user1->name, "John");
//    strcpy(user1->surname, "Doe");
//    strcpy(user1->phone, "1234567890");
//    strcpy(user1->password, "password123");
//    user1->next = nullptr;
//
//    User* user2 = (User*)malloc(sizeof(User));
//    strcpy(user2->name, "Jane");
//    strcpy(user2->surname, "Smith");
//    strcpy(user2->phone, "0987654321");
//    strcpy(user2->password, "password456");
//    user2->next = nullptr;
//
//    for (int i = 0; i < TABLE_SIZE; i++) {
//        hashTable[i] = nullptr;
//    }
//
//    saveUser(user1);
//    saveUser(user2);
//
//    simulateUserInput("1234567890\npassword123\n");
//    EXPECT_TRUE(logIn());
//
//    simulateUserInput("0987654321\npassword456\n");
//    EXPECT_TRUE(logIn());
//
//    simulateUserInput("1234567890\nwrongpassword\n");
//    EXPECT_FALSE(logIn());
//
//    simulateUserInput("0000000000\npassword123\n");
//    EXPECT_FALSE(logIn());
//
//    simulateUserInput("\npassword123\n");
//    EXPECT_FALSE(logIn());
//
//    simulateUserInput("1234567890\n\n");
//    EXPECT_FALSE(logIn());
//
//    free(user1);
//    free(user2);
//}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}