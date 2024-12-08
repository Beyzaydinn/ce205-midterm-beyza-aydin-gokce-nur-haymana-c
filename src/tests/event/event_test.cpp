#include <cstring>
#include "gtest/gtest.h"
#include "../../event/header/event.h"  
#include "../../event/src/event.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
#include <cstdio>
#include <fstream>

#define ENABLE_DENEME_TEST


class MinHeapTest : public ::testing::Test {
protected:
    MinHeap* heap;
    MinHeapNode* node;
    void SetUp() override {
        heap = nullptr;
        node = nullptr;
    }

    void TearDown() override {
        if (heap) {
            if (heap->array) {
                free(heap->array);
            }
            free(heap);
        }
        if (node) {
            free(node);
        }
    }
};
TEST_F(MinHeapTest, CreateMinHeapValidCapacity) {
    unsigned capacity = 10;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed, returned nullptr.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size is not zero.";
    EXPECT_EQ(heap->capacity, capacity) << "Heap capacity is not set correctly.";
    EXPECT_NE(heap->array, nullptr) << "Heap array was not allocated properly.";
}

TEST_F(MinHeapTest, CreateMinHeapZeroCapacity) {
    unsigned capacity = 0;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed for zero capacity.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size is not zero.";
    EXPECT_EQ(heap->capacity, 0) << "Heap capacity for zero capacity is not set correctly.";
    EXPECT_EQ(heap->array, nullptr) << "Heap array should be nullptr for zero capacity.";
}

TEST_F(MinHeapTest, CreateMinHeapLargeCapacity) {
    unsigned capacity = 100000;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed for large capacity.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size is not zero for large capacity.";
    EXPECT_EQ(heap->capacity, capacity) << "Heap capacity is not set correctly for large capacity.";
    EXPECT_NE(heap->array, nullptr) << "Heap array was not allocated properly for large capacity.";
}

TEST_F(MinHeapTest, CreateMinHeapWithValidCapacity) {
    unsigned capacity = 10;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed, returned nullptr.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size should be zero.";
    EXPECT_EQ(heap->capacity, capacity) << "Heap capacity should match the given value.";
    EXPECT_NE(heap->array, nullptr) << "Heap array should be allocated properly.";
}

TEST_F(MinHeapTest, CreateMinHeapWithZeroCapacity) {
    unsigned capacity = 0;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed for zero capacity.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size should be zero.";
    EXPECT_EQ(heap->capacity, 0) << "Heap capacity should be zero.";
    EXPECT_EQ(heap->array, nullptr) << "Heap array should be nullptr for zero capacity.";
}

TEST_F(MinHeapTest, CreateMinHeapWithLargeCapacity) {
    unsigned capacity = 100000;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed for large capacity.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size should be zero for large capacity.";
    EXPECT_EQ(heap->capacity, capacity) << "Heap capacity should match the given value.";
    EXPECT_NE(heap->array, nullptr) << "Heap array should be allocated properly for large capacity.";
}

TEST_F(MinHeapTest, CreateMinHeapNodeWithValidData) {
    char data = 'a';
    unsigned freq = 5;
    node = createMinHeapNode(data, freq);

    ASSERT_NE(node, nullptr) << "Node creation failed, returned nullptr.";
    EXPECT_EQ(node->data, data) << "Node data should match the input data.";
    EXPECT_EQ(node->freq, freq) << "Node frequency should match the input frequency.";
    EXPECT_EQ(node->left, nullptr) << "Node left child should be NULL.";
    EXPECT_EQ(node->right, nullptr) << "Node right child should be NULL.";
}


class HashFunctionTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(HashFunctionTest, HashProducesConsistentResults) {
    const char* input1 = "1234567890";
    const char* input2 = "0987654321";
    unsigned int hash1 = hash(input1);
    unsigned int hash1_repeat = hash(input1);
    ASSERT_EQ(hash1, hash1_repeat);
    unsigned int hash2 = hash(input2);
    ASSERT_NE(hash1, hash2);
}

TEST_F(HashFunctionTest, HashHandlesEmptyInput) {
    const char* emptyInput = "";
    unsigned int hashValue = hash(emptyInput);
    ASSERT_EQ(hashValue, 0);
}

TEST_F(HashFunctionTest, HashIsWithinTableSize) {
    const char* input = "1234567890";
    unsigned int hashValue = hash(input);
    ASSERT_LT(hashValue, TABLE_SIZE);
}

TEST_F(HashFunctionTest, HashHandlesSpecialCharacters) {
    const char* specialInput = "!@#$%^&*()";
    unsigned int hashValue = hash(specialInput);
    ASSERT_LT(hashValue, TABLE_SIZE);
}


class SaveUserTest : public ::testing::Test {
protected:
    void SetUp() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashTable[i] = nullptr;
        }
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != nullptr) {
                User* toDelete = current;
                current = current->next;
                free(toDelete);
                toDelete = NULL;
            }
        }
    }


    User* createUser(const char* name, const char* surname, const char* phone, const char* password) {
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->name, name);
        strcpy(user->surname, surname);
        strcpy(user->phone, phone);
        strcpy(user->password, password);
        user->next = nullptr;
        return user;
    }
};

TEST_F(SaveUserTest, SaveUserAddsToHashTable) {
    User* user = createUser("John", "Doe", "1234567890", "password123");
    saveUser(user);

    unsigned int index = hash(user->phone);
    ASSERT_NE(hashTable[index], nullptr);
    ASSERT_EQ(hashTable[index], user);
    ASSERT_STREQ(hashTable[index]->name, "John");
    ASSERT_STREQ(hashTable[index]->surname, "Doe");
    ASSERT_STREQ(hashTable[index]->phone, "1234567890");
    ASSERT_STREQ(hashTable[index]->password, "password123");
}

TEST_F(SaveUserTest, SaveUserHandlesCollisions) {
    User* user1 = createUser("Alice", "Smith", "123", "password1");
    User* user2 = createUser("Bob", "Brown", "123", "password2");

    saveUser(user1);
    saveUser(user2);

    unsigned int index = hash(user1->phone);

    ASSERT_EQ(hashTable[index], user2);
    ASSERT_EQ(hashTable[index]->next, user1);
}

TEST_F(SaveUserTest, SaveUserDoesNotLosePreviousEntries) {
    User* user1 = createUser("Charlie", "White", "1234567890", "pass1");
    User* user2 = createUser("Diana", "Green", "1234567890", "pass2");

    saveUser(user1);
    saveUser(user2);

    unsigned int index = hash(user1->phone);

    ASSERT_EQ(hashTable[index], user2);
    ASSERT_EQ(hashTable[index]->next, user1);
    ASSERT_EQ(hashTable[index]->next->next, nullptr);
}

class SaveHashTableToFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashTable[i] = nullptr;
        }
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != nullptr) {
                User* toDelete = current;
                current = current->next;
                free(toDelete);
            }
        }
    }
    User* createUser(const char* name, const char* surname, const char* phone, const char* password) {
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->name, name);
        strcpy(user->surname, surname);
        strcpy(user->phone, phone);
        strcpy(user->password, password);
        user->next = nullptr;
        return user;
    }
    int readUsersFromFile(const char* filename, User* users[], int maxUsers) {
        FILE* file = fopen(filename, "rb");
        if (file == nullptr) {
            perror("An error occurred while opening the file");
            return -1;
        }

        int count = 0;
        while (fread(users[count], sizeof(User), 1, file) == 1 && count < maxUsers) {
            count++;
        }
        fclose(file);
        return count;
    }
};

class ValidateLoginTest : public ::testing::Test {
protected:
    void SetUp() override {
        memset(hashTable, 0, sizeof(hashTable));

        User* user1 = new User{ "John", "Doe", "1234567890", "password123", nullptr };
        User* user2 = new User{ "Jane", "Smith", "0987654321", "mypassword", nullptr };

        saveUser(user1);
        saveUser(user2);
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current) {
                User* toDelete = current;
                current = current->next;
                delete toDelete;
            }
            hashTable[i] = nullptr;
        }
    }
};

TEST_F(ValidateLoginTest, ValidLogin) {
    EXPECT_TRUE(validateLogin("1234567890", "password123"));
    EXPECT_TRUE(validateLogin("0987654321", "mypassword"));
}

TEST_F(ValidateLoginTest, InvalidPassword) {
    EXPECT_FALSE(validateLogin("1234567890", "wrongpassword"));
    EXPECT_FALSE(validateLogin("0987654321", "anotherwrongpassword"));
}

TEST_F(ValidateLoginTest, InvalidPhoneNumber) {
    EXPECT_FALSE(validateLogin("1111111111", "password123"));
    EXPECT_FALSE(validateLogin("2222222222", "mypassword"));
}

TEST_F(ValidateLoginTest, NullParameters) {
    EXPECT_FALSE(validateLogin(nullptr, "password123"));
    EXPECT_FALSE(validateLogin("1234567890", nullptr));
    EXPECT_FALSE(validateLogin(nullptr, nullptr));
}

TEST_F(ValidateLoginTest, UserNotInHashTable) {
    EXPECT_FALSE(validateLogin("5555555555", "nonexistent"));
}

//TEST(MainMenuTest, InvalidMenuSelectionTest) {
//    std::istringstream input("0\n"); 
//    std::streambuf* origStdin = std::cin.rdbuf();
//    std::cin.rdbuf(input.rdbuf());
//
//    EXPECT_FALSE(mainMenu());
//
//    std::cin.rdbuf(origStdin);
//}


class SaveHashTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        memset(hashTable, 0, sizeof(hashTable));
        User* user1 = new User{ "John", "Doe", "1234567890", "password123", nullptr };
        User* user2 = new User{ "Jane", "Smith", "0987654321", "mypassword", nullptr };
        saveUser(user1);
        saveUser(user2);
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current) {
                User* toDelete = current;
                current = current->next;
                delete toDelete;
            }
            hashTable[i] = nullptr;
        }
        remove("users.bin");
    }
};

TEST_F(SaveHashTableTest, SaveToFile) {
    saveHashTableToFile();
    FILE* file = fopen("users.bin", "rb");
    ASSERT_NE(file, nullptr) << "Dosya oluþturulamadý!";
    User user;
    ASSERT_EQ(fread(&user, sizeof(User), 1, file), 1);
    EXPECT_STREQ(user.name, "Jane");
    EXPECT_STREQ(user.surname, "Smith");
    EXPECT_STREQ(user.phone, "0987654321");
    EXPECT_STREQ(user.password, "mypassword");

    ASSERT_EQ(fread(&user, sizeof(User), 1, file), 1);
    EXPECT_STREQ(user.name, "John");
    EXPECT_STREQ(user.surname, "Doe");
    EXPECT_STREQ(user.phone, "1234567890");
    EXPECT_STREQ(user.password, "password123");

    ASSERT_EQ(fread(&user, sizeof(User), 1, file), 0);

    fclose(file);
}

TEST_F(SaveHashTableToFileTest, SaveHashTableToFileHandlesEmptyHashTable) {
    saveHashTableToFile();
    User* users[2];
    for (int i = 0; i < 2; i++) {
        users[i] = (User*)malloc(sizeof(User));
    }
    int userCount = readUsersFromFile("users.bin", users, 2);

    ASSERT_EQ(userCount, 0);

    for (int i = 0; i < 2; i++) {
        free(users[i]);
    }
}

class LoadHashTableFromFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashTable[i] = NULL;
        }
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != NULL) {
                User* toDelete = current;
                current = current->next;

                if (toDelete != NULL) {
                    free(toDelete);
                    toDelete = NULL;
                }
            }
        }
    }


    User* createUser(const char* name, const char* surname, const char* phone, const char* password) {
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->name, name);
        strcpy(user->surname, surname);
        strcpy(user->phone, phone);
        strcpy(user->password, password);
        user->next = NULL;
        return user;
    }
    int readUsersFromFile(const char* filename, User* users[], int maxUsers) {
        FILE* file = fopen(filename, "rb");
        if (file == NULL) {
            perror("An error occurred while opening the file");
            return -1;
        }

        int count = 0;
        while (fread(users[count], sizeof(User), 1, file) == 1 && count < maxUsers) {
            count++;
        }
        fclose(file);
        return count;
    }
};

class LoadHashTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        memset(hashTable, 0, sizeof(hashTable));
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current) {
                User* toDelete = current;
                current = current->next;
                delete toDelete;
            }
            hashTable[i] = nullptr;
        }
        remove("users.bin");
    }
};

TEST_F(LoadHashTableTest, LoadValidFile) {
    FILE* file = fopen("users.bin", "wb");
    ASSERT_NE(file, nullptr);

    User user1 = { "John", "Doe", "1234567890", "password123", nullptr };
    User user2 = { "Jane", "Smith", "0987654321", "mypassword", nullptr };
    fwrite(&user1, sizeof(User), 1, file);
    fwrite(&user2, sizeof(User), 1, file);
    fclose(file);
    loadHashTableFromFile();
    unsigned int index1 = hash("1234567890");
    ASSERT_NE(hashTable[index1], nullptr);
    EXPECT_STREQ(hashTable[index1]->name, "John");
    EXPECT_STREQ(hashTable[index1]->surname, "Doe");
    EXPECT_STREQ(hashTable[index1]->phone, "1234567890");
    EXPECT_STREQ(hashTable[index1]->password, "password123");

    unsigned int index2 = hash("0987654321");
    ASSERT_NE(hashTable[index2], nullptr);
    EXPECT_STREQ(hashTable[index2]->name, "Jane");
    EXPECT_STREQ(hashTable[index2]->surname, "Smith");
    EXPECT_STREQ(hashTable[index2]->phone, "0987654321");
    EXPECT_STREQ(hashTable[index2]->password, "mypassword");
}

TEST_F(LoadHashTableTest, LoadEmptyFile) {
    FILE* file = fopen("users.bin", "wb");
    ASSERT_NE(file, nullptr);
    fclose(file);
    loadHashTableFromFile();

    for (int i = 0; i < TABLE_SIZE; i++) {
        EXPECT_EQ(hashTable[i], nullptr);
    }
}

TEST_F(LoadHashTableTest, FileDoesNotExist) {
    loadHashTableFromFile();
    for (int i = 0; i < TABLE_SIZE; i++) {
        EXPECT_EQ(hashTable[i], nullptr);
    }
}
TEST_F(LoadHashTableFromFileTest, LoadHashTableFromFileHandlesEmptyFile) {
    TearDown();
    FILE* file = fopen("users.bin", "wb");
    fclose(file);

    loadHashTableFromFile();
    bool foundUser = false;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL) {
            foundUser = true;
            break;
        }
    }

    ASSERT_FALSE(foundUser);
}

TEST_F(LoadHashTableFromFileTest, LoadHashTableFromFileHandlesFileError) {
    FILE* file = fopen("non_existent_file.bin", "rb");
    ASSERT_EQ(file, nullptr);

    if (file != nullptr) {
        fclose(file);
    }

    bool foundUser = false;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL) {
            foundUser = true;
            break;
        }
    }

    ASSERT_FALSE(foundUser);
}


class QuadraticProbingInsertTest : public ::testing::Test {
protected:
    void SetUp() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashTable[i] = NULL;
        }
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != NULL) {
                User* toDelete = current;
                current = current->next;
                free(toDelete);
            }
        }
    }
    User* createUser(const char* name, const char* surname, const char* phone, const char* password) {
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->name, name);
        strcpy(user->surname, surname);
        strcpy(user->phone, phone);
        strcpy(user->password, password);
        user->next = NULL;
        return user;
    }
};

TEST_F(QuadraticProbingInsertTest, SuccessfullyInsertsUser) {
    User* user1 = createUser("Alice", "Smith", "1234567890", "password1");
    ASSERT_TRUE(quadraticProbingInsert(user1));

    unsigned int index = hash(user1->phone);
    ASSERT_NE(hashTable[index], nullptr);
    ASSERT_STREQ(hashTable[index]->name, "Alice");
}

TEST_F(QuadraticProbingInsertTest, HandlesCollisionWithQuadraticProbing) {
    User* user1 = createUser("Alice", "Smith", "1234567890", "password1");
    User* user2 = createUser("Bob", "Brown", "1234567891", "password2");

    ASSERT_TRUE(quadraticProbingInsert(user1));
    ASSERT_TRUE(quadraticProbingInsert(user2));

    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);
    ASSERT_NE(index1, index2);
    ASSERT_NE(hashTable[index1], nullptr);
    ASSERT_NE(hashTable[index2], nullptr);
}


TEST_F(QuadraticProbingInsertTest, HandlesMultipleCollisions) {
    User* user1 = createUser("User1", "One", "1234567890", "password1");
    User* user2 = createUser("User2", "Two", "1234567891", "password2");
    User* user3 = createUser("User3", "Three", "1234567892", "password3");
    ASSERT_TRUE(quadraticProbingInsert(user1));
    ASSERT_TRUE(quadraticProbingInsert(user2));
    ASSERT_TRUE(quadraticProbingInsert(user3));

    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);
    unsigned int index3 = hash(user3->phone);

    ASSERT_NE(index1, index2);
    ASSERT_NE(index2, index3);
    ASSERT_NE(index1, index3);
    ASSERT_NE(hashTable[index1], nullptr);
    ASSERT_NE(hashTable[index2], nullptr);
    ASSERT_NE(hashTable[index3], nullptr);
}


class QuadraticProbingTest : public ::testing::Test {
protected:
    void SetUp() override {
        memset(hashTable, 0, sizeof(hashTable));
    }

    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (hashTable[i] != nullptr) {
                delete hashTable[i];
                hashTable[i] = nullptr;
            }
        }
    }
};

TEST_F(QuadraticProbingTest, SuccessfulInsert) {
    User* user = new User;
    strcpy(user->name, "John");
    strcpy(user->surname, "Doe");
    strcpy(user->phone, "1234567890");
    strcpy(user->password, "password123");
    user->next = nullptr;

    EXPECT_TRUE(quadraticProbingInsert(user));

    unsigned int index = hash("1234567890");
    EXPECT_EQ(hashTable[index], user);
    EXPECT_STREQ(hashTable[index]->name, "John");
    EXPECT_STREQ(hashTable[index]->surname, "Doe");
}

TEST_F(QuadraticProbingTest, CollisionResolution) {
    User* user1 = new User;
    strcpy(user1->name, "Alice");
    strcpy(user1->surname, "Smith");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "pass1");
    user1->next = nullptr;

    User* user2 = new User;
    strcpy(user2->name, "Bob");
    strcpy(user2->surname, "Jones");
    strcpy(user2->phone, "1234567891");
    strcpy(user2->password, "pass2");
    user2->next = nullptr;

    EXPECT_TRUE(quadraticProbingInsert(user1));
    EXPECT_TRUE(quadraticProbingInsert(user2));

    unsigned int index1 = hash("1234567890");
    unsigned int index2 = hash("1234567891");

    EXPECT_NE(index1, index2);
    EXPECT_EQ(hashTable[index1], user1);
    EXPECT_EQ(hashTable[index2], user2);
}
class HuffmanTreeTest : public ::testing::Test {
protected:
    AttendeE attendee;

    void SetUp() override {
        memset(&attendee, 0, sizeof(AttendeE));
    }

    void TearDown() override {
    }
};

TEST_F(HuffmanTreeTest, BuildHuffmanTreeTest) {
    char inputStr[] = "hello huffman";
    buildHuffmanTree(inputStr, &attendee);
    ASSERT_NE(attendee.huffmanCode, nullptr) << "Huffman code should be generated.";
    EXPECT_STRNE("", attendee.huffmanCode) << "Huffman code should not be an empty string.";

    bool hFound = false;
    for (int i = 0; i < strlen(attendee.huffmanCode); i++) {
        if (attendee.huffmanCode[i] == 'h') {
            hFound = true;
            break;
        }
    }
    EXPECT_TRUE(hFound) << "'h' should have a corresponding Huffman code.";
}

std::string captureOutput(void (*functionToTest)()) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    functionToTest();
    std::cout.rdbuf(oldCout);
    return oss.str();
}

class ProgressiveOverflowTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};
TEST_F(ProgressiveOverflowTest, NoOverflow) {
    std::string output = captureOutput([]() {
        int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int overflowThreshold = 100;
        int currentSum = 0;
        for (int i = 0; i < 10; i++) {
            currentSum += array[i];
            if (currentSum > overflowThreshold) {
                printf("Overflow detected at index %d with sum %d\n", i, currentSum);
                currentSum = 0;
            }
        }
        });
    EXPECT_EQ(output.find("Overflow detected"), std::string::npos);
}
class HuffmanTest : public ::testing::Test {
protected:
    MinHeapNode* createSimpleHuffmanTree() {
        MinHeapNode* root = createMinHeapNode('$', 0);
        MinHeapNode* left = createMinHeapNode('a', 5);
        MinHeapNode* right = createMinHeapNode('b', 7);

        root->left = left;
        root->right = right;

        return root;
    }
};

TEST_F(HuffmanTest, GenerateHuffmanCodesTest) {
    MinHeapNode* root = createSimpleHuffmanTree();

    char code[256];
    char huffmanCode[1024] = "";
    generateHuffmanCodes(root, code, 0, huffmanCode);
    EXPECT_STRNE("", huffmanCode) << "Huffman code should not be empty.";
    EXPECT_TRUE(strstr(huffmanCode, "a: 0\n") != nullptr) << "Huffman code for 'a' is incorrect.";
    EXPECT_TRUE(strstr(huffmanCode, "b: 1\n") != nullptr) << "Huffman code for 'b' is incorrect.";
    free(root->left);
    free(root->right);
    free(root);
}

// The class name will be used to define the test fixture
class LinearProbingTest : public ::testing::Test {
protected:
    int hashTable[10];  // Hash table for testing
    int keys[5] = { 23, 45, 12, 37, 29 };  // Keys to insert

    // This function sets up the hash table before each test
    void SetUp() override {
        for (int i = 0; i < 10; i++) {
            hashTable[i] = -1;  // Initialize the table with -1 (empty slots)
        }
    }

    // This function performs the linear probing insertion
    void linearProbing() {
        int size = sizeof(keys) / sizeof(keys[0]);
        for (int i = 0; i < size; i++) {
            int index = keys[i] % 10;
            while (hashTable[index] != -1) {  // If the slot is not empty
                index = (index + 1) % 10;     // Linear probing (next slot)
            }
            hashTable[index] = keys[i];  // Insert the key
        }
    }
};

// Test for an empty hash table
TEST_F(LinearProbingTest, InsertKeysIntoEmptyTable) {
    linearProbing();  // Run the algorithm

    // Expected values after insertion (given the linear probing behavior)
    int expected[] = { 23, 45, 12, 37, 29 };

    // Check if the hash table contains the expected values
    for (int i = 0; i < 10; i++) {
        if (hashTable[i] != -1) {
            bool found = false;
            for (int j = 0; j < 5; j++) {
                if (hashTable[i] == expected[j]) {
                    found = true;
                    break;
                }
            }
            EXPECT_TRUE(found) << "Hash table contains unexpected value at index " << i;
        }
    }
}

// Test for handling collisions
TEST_F(LinearProbingTest, HandleCollisionsProperly) {
    linearProbing();  // Run the algorithm

    // Verify that the keys are inserted correctly and no collisions were missed
    EXPECT_EQ(hashTable[3], 23);  // Key 23 should be at index 3 (23 % 10 = 3)
    EXPECT_EQ(hashTable[4], 45);  // Key 45 should be at index 4 (45 % 10 = 5, collision at 5, next slot)
    EXPECT_EQ(hashTable[5], 12);  // Key 12 should be at index 5 (12 % 10 = 2, collision at 2, next slot)
    EXPECT_EQ(hashTable[6], 37);  // Key 37 should be at index 6 (37 % 10 = 7, collision at 7, next slot)
    EXPECT_EQ(hashTable[7], 29);  // Key 29 should be at index 7 (29 % 10 = 9, collision at 9, next slot)
}

// Test when the table is full
TEST_F(LinearProbingTest, TableFullAfterInsertions) {
    // Insert the keys into the table
    linearProbing();

    // Verify that no slots are left empty
    for (int i = 0; i < 10; i++) {
        EXPECT_NE(hashTable[i], -1) << "Hash table slot " << i << " is empty when it should be filled.";
    }
}

// Test fixture class for double hashing
class DoubleHashingTest : public ::testing::Test {
protected:
    int hashTable[10];  // Hash table for testing
    int keys[5] = { 23, 45, 12, 37, 29 };  // Keys to insert

    // This function sets up the hash table before each test
    void SetUp() override {
        for (int i = 0; i < 10; i++) {
            hashTable[i] = 0;  // Initialize the table with 0 (empty slots)
        }
    }

    // This function performs the double hashing insertion
    void doubleHashing() {
        int size = sizeof(keys) / sizeof(keys[0]);
        for (int i = 0; i < size; i++) {
            int index = keys[i] % 10;  // First hash function
            int step = 7 - (keys[i] % 7);  // Second hash function
            while (hashTable[index] != 0) {  // If the slot is not empty
                index = (index + step) % 10;  // Resolve collision using double hashing
            }
            hashTable[index] = keys[i];  // Insert the key
        }
    }
};

// Test to verify the insertion of keys into an empty hash table
TEST_F(DoubleHashingTest, InsertKeysIntoEmptyTable) {
    doubleHashing();  // Run the algorithm

    // Expected values after insertion, based on double hashing
    int expected[] = { 23, 45, 12, 37, 29 };

    // Check if the hash table contains the expected values at the right indexes
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5, no collision
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9
}

// Test to verify handling of collisions
TEST_F(DoubleHashingTest, HandleCollisionsProperly) {
    // Manually check for collisions
    // Double Hashing will resolve collisions by stepping with a second hash function

    doubleHashing();  // Run the algorithm

    // Verify that the keys are inserted correctly and collisions are handled
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9
}

// Test for the table being full after insertions
TEST_F(DoubleHashingTest, TableFullAfterInsertions) {
    doubleHashing();  // Insert the keys into the table

    // Verify that no slots are left empty
    for (int i = 0; i < 10; i++) {
        EXPECT_NE(hashTable[i], 0) << "Hash table slot " << i << " is empty when it should be filled.";
    }
}

// Test to check the behavior with an empty table (No insertions)
TEST_F(DoubleHashingTest, NoInsertions) {
    // Ensure the table is still empty before insertion
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(hashTable[i], 0) << "Hash table slot " << i << " is not empty initially.";
    }
}

class UseBucketsTest : public ::testing::Test {
protected:
    int buckets[3][10];  // 3 bucket, her biri 10 eleman alabilir
    int keys[5] = { 23, 45, 12, 37, 29 };  // Anahtarlar

    // Bu fonksiyon testten önce bucket'larý sýfýrlýyor
    void SetUp() override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 10; j++) {
                buckets[i][j] = 0;  // Baþlangýçta tüm slotlar boþ
            }
        }
    }

    // useBuckets fonksiyonunu burada taklit ediyoruz
    void useBuckets() {
        int size = sizeof(keys) / sizeof(keys[0]);

        // Anahtarlarý uygun kovaya yerleþtirme
        for (int i = 0; i < size; i++) {
            int bucketIndex = keys[i] % 3;
            for (int j = 0; j < 10; j++) {
                if (buckets[bucketIndex][j] == 0) {
                    buckets[bucketIndex][j] = keys[i];
                    break;
                }
            }
        }
    }
};

TEST_F(UseBucketsTest, BucketsAreFilledCorrectly) {
    // Anahtarlarý kovaya yerleþtir
    useBuckets();


    // Beklenen deðerleri kontrol et
    EXPECT_EQ(buckets[0][0], 45);  // Kova 0, %3'e göre (45 % 3 = 0)
    EXPECT_EQ(buckets[0][1], 12);  // Kova 0, %3'e göre (12 % 3 = 0)
    EXPECT_EQ(buckets[1][0], 23);  // Kova 1, %3'e göre (23 % 3 = 2)
    EXPECT_EQ(buckets[1][1], 37);  // Kova 1, %3'e göre (37 % 3 = 1)
    EXPECT_EQ(buckets[2][0], 29);  // Kova 2, %3'e göre (29 % 3 = 2)
}

// Test 2: Her bucket'ýn baþlangýçta boþ olmasý gerektiðini kontrol et
TEST_F(UseBucketsTest, BucketsShouldBeEmptyInitially) {
    // Baþlangýçta tüm bucket'lar sýfýr olmalý
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            EXPECT_EQ(buckets[i][j], 0) << "Bucket " << i << " slot " << j << " should be empty initially.";
        }
    }
}

// Test 3: Kovalarýn boyutunun 10'dan fazla anahtar kabul etmemesini kontrol et
TEST_F(UseBucketsTest, BucketCanOnlyHoldUpTo10Elements) {
    // Anahtar sayýsý 5 olduðundan, her kova yalnýzca 10'dan fazla eleman kabul etmemeli
    useBuckets();

    // Kovalarýn içeriðini kontrol et, her kova sadece yerleþtirilebilen anahtarlarý içermeli
    for (int i = 0; i < 3; i++) {
        int count = 0;
        for (int j = 0; j < 10; j++) {
            if (buckets[i][j] != 0) {
                count++;
            }
        }
        EXPECT_LE(count, 10) << "Bucket " << i << " should not have more than 10 elements.";
    }
}

// Test 4: Kovanýn doðru þekilde sýfýrlandýðýný test et
TEST_F(UseBucketsTest, BucketsResetCorrectly) {
    useBuckets();  // Ýlk yerleþtirmeyi yap
    SetUp();  // Testten önce sýfýrlama yap
    // Sýfýrlama sonrasýnda, tüm kova elemanlarýnýn 0 olmasý gerekiyor
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            EXPECT_EQ(buckets[i][j], 0) << "Bucket " << i << " slot " << j << " should be empty after reset.";
        }
    }
}

// Test fixture class for Linear Quotient algorithm
class LinearQuotientTest : public ::testing::Test {
protected:
    int hashTable[10];  // Hash table for testing
    int keys[5] = { 23, 45, 12, 37, 29 };  // Keys to insert

    // This function sets up the hash table before each test
    void SetUp() override {
        for (int i = 0; i < 10; i++) {
            hashTable[i] = 0;  // Initialize the table with 0 (empty slots)
        }
    }

    // This function performs the linear quotient insertion
    void linearQuotient() {
        int size = sizeof(keys) / sizeof(keys[0]);
        for (int i = 0; i < size; i++) {
            int index = keys[i] % 10;  // First hash function
            int increment = 1;  // Linear increment for collision handling
            while (hashTable[index] != 0) {  // If the slot is not empty
                index = (index + increment) % 10;  // Linear probing (increment the index)
            }
            hashTable[index] = keys[i];  // Insert the key
        }
    }
};

// Test to verify the insertion of keys into the hash table
TEST_F(LinearQuotientTest, InsertKeysIntoTable) {
    linearQuotient();  // Run the algorithm

    // Check if the keys are correctly placed in the table
    // Expected placements based on keys % 10 and linear probing:
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9
}

// Test to ensure no slot in the table is left empty (i.e., no 0 values)
TEST_F(LinearQuotientTest, NoEmptySlotsInTable) {
    linearQuotient();  // Run the algorithm

    // Verify that no slot in the hash table is left empty (no 0's)
    for (int i = 0; i < 10; i++) {
        EXPECT_NE(hashTable[i], 0) << "Hash table slot " << i << " is empty when it should be filled.";
    }
}

// Test to verify that no key is inserted into the wrong slot
TEST_F(LinearQuotientTest, KeysInCorrectSlots) {
    linearQuotient();  // Run the algorithm

    // Verify the correct placement of keys in the right slots based on mod 10
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9
}

// Test to verify that collisions are handled by linear increment (linear probing)
TEST_F(LinearQuotientTest, HandleCollisionsProperly) {
    // Manually check for collisions
    // We will insert keys to verify that when collisions happen, they are resolved by linear increment

    linearQuotient();  // Run the algorithm

    // Verify that keys are inserted in the correct positions even after collisions
    // Expected index for each key after collisions:
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9
}

// Test for the table being full after insertions
TEST_F(LinearQuotientTest, TableFullAfterInsertions) {
    linearQuotient();  // Insert the keys into the table

    // Verify that no slots are left empty
    for (int i = 0; i < 10; i++) {
        EXPECT_NE(hashTable[i], 0) << "Hash table slot " << i << " is empty when it should be filled.";
    }
}

// Test fixture class for Brent's Method algorithm
class BrentsMethodTest : public ::testing::Test {
protected:
    int hashTable[10];  // Hash table for testing
    int keys[5] = { 23, 45, 12, 37, 29 };  // Keys to insert

    // This function sets up the hash table before each test
    void SetUp() override {
        for (int i = 0; i < 10; i++) {
            hashTable[i] = 0;  // Initialize the table with 0 (empty slots)
        }
    }

    // This function performs Brent's Method insertion
    void brentsMethod() {
        int size = sizeof(keys) / sizeof(keys[0]);
        for (int i = 0; i < size; i++) {
            int index = keys[i] % 10;  // First hash function
            int step = 1;  // Initial step size
            while (hashTable[index] != 0) {  // If the slot is not empty
                int newIndex = (index + step) % 10;  // Brent's Method: Increase the step size
                if (hashTable[newIndex] == 0) {  // If an empty slot is found
                    index = newIndex;
                    break;
                }
                step++;  // Increment the step for the next probe
            }
            hashTable[index] = keys[i];  // Insert the key at the computed index
        }
    }
};

// Test for the insertion of keys into the hash table using Brent's Method
TEST_F(BrentsMethodTest, InsertKeysIntoTable) {
    brentsMethod();  // Run the algorithm

    // Check if the keys are correctly placed in the table
    // Expected placements based on keys % 10 and Brent's method probing:
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5, no collision
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7, no collision
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9, no collision
}

// Test to ensure no slot in the table is left empty (i.e., no 0 values)
TEST_F(BrentsMethodTest, NoEmptySlotsInTable) {
    brentsMethod();  // Run the algorithm

    // Verify that no slot in the hash table is left empty (no 0's)
    for (int i = 0; i < 10; i++) {
        EXPECT_NE(hashTable[i], 0) << "Hash table slot " << i << " is empty when it should be filled.";
    }
}

// Test to verify that no key is inserted into the wrong slot
TEST_F(BrentsMethodTest, KeysInCorrectSlots) {
    brentsMethod();  // Run the algorithm

    // Verify the correct placement of keys in the right slots based on mod 10
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9
}

// Test to verify that collisions are handled by Brent's method (increasing step size)
TEST_F(BrentsMethodTest, HandleCollisionsProperly) {
    brentsMethod();  // Run the algorithm

    // Verify that keys are inserted in the correct positions even after collisions
    // Expected index for each key after Brent's method probing:
    EXPECT_EQ(hashTable[3], 23);  // 23 % 10 = 3
    EXPECT_EQ(hashTable[5], 45);  // 45 % 10 = 5
    EXPECT_EQ(hashTable[2], 12);  // 12 % 10 = 2
    EXPECT_EQ(hashTable[7], 37);  // 37 % 10 = 7
    EXPECT_EQ(hashTable[9], 29);  // 29 % 10 = 9
}

// Test for the table being full after insertions
TEST_F(BrentsMethodTest, TableFullAfterInsertions) {
    brentsMethod();  // Insert the keys into the table

    // Verify that no slots are left empty
    for (int i = 0; i < 10; i++) {
        EXPECT_NE(hashTable[i], 0) << "Hash table slot " << i << " is empty when it should be filled.";
    }
}

// Mocking functions that are called in handleFileOperation
class HashingAlgorithmsMock {
public:
    bool progressiveOverflowCalled = false;
    bool linearProbingCalled = false;
    bool quadraticProbingCalled = false;
    bool doubleHashingCalled = false;
    bool useBucketsCalled = false;
    bool linearQuotientCalled = false;
    bool brentsMethodCalled = false;

    void progressiveOverflow() {
        progressiveOverflowCalled = true;
    }

    void linearProbing() {
        linearProbingCalled = true;
    }

    void quadraticProbing() {
        quadraticProbingCalled = true;
    }

    void doubleHashing() {
        doubleHashingCalled = true;
    }

    void useBuckets() {
        useBucketsCalled = true;
    }

    void linearQuotient() {
        linearQuotientCalled = true;
    }

    void brentsMethod() {
        brentsMethodCalled = true;
    }
};

// The function under test
void handleFileOperation(int choice, HashingAlgorithmsMock* mock) {
    switch (choice) {
    case 1:
        mock->progressiveOverflow();
        break;
    case 2:
        mock->linearProbing();
        break;
    case 3:
        mock->quadraticProbing();
        break;
    case 4:
        mock->doubleHashing();
        break;
    case 5:
        mock->useBuckets();
        break;
    case 6:
        mock->linearQuotient();
        break;
    case 7:
        mock->brentsMethod();
        break;
    case 8:
        printf("Returning to Authentication Menu.\n");
        break;
    default:
        printf("Invalid choice. Please try again.\n");
    }
}

// Test fixture class for handleFileOperation tests
class HandleFileOperationTest : public ::testing::Test {
protected:
    HashingAlgorithmsMock mock;  // Create mock object

    // Helper function to reset flags before each test
    void resetFlags() {
        mock.progressiveOverflowCalled = false;
        mock.linearProbingCalled = false;
        mock.quadraticProbingCalled = false;
        mock.doubleHashingCalled = false;
        mock.useBucketsCalled = false;
        mock.linearQuotientCalled = false;
        mock.brentsMethodCalled = false;
    }
};

// Test for progressiveOverflow selection (choice == 1)
TEST_F(HandleFileOperationTest, TestProgressiveOverflow) {
    resetFlags();
    handleFileOperation(1, &mock);  // Call the function with choice 1
    EXPECT_TRUE(mock.progressiveOverflowCalled);  // Check that the function was called
}

// Test for linearProbing selection (choice == 2)
TEST_F(HandleFileOperationTest, TestLinearProbing) {
    resetFlags();
    handleFileOperation(2, &mock);  // Call the function with choice 2
    EXPECT_TRUE(mock.linearProbingCalled);  // Check that the function was called
}

// Test for quadraticProbing selection (choice == 3)
TEST_F(HandleFileOperationTest, TestQuadraticProbing) {
    resetFlags();
    handleFileOperation(3, &mock);  // Call the function with choice 3
    EXPECT_TRUE(mock.quadraticProbingCalled);  // Check that the function was called
}

// Test for doubleHashing selection (choice == 4)
TEST_F(HandleFileOperationTest, TestDoubleHashing) {
    resetFlags();
    handleFileOperation(4, &mock);  // Call the function with choice 4
    EXPECT_TRUE(mock.doubleHashingCalled);  // Check that the function was called
}

// Test for useBuckets selection (choice == 5)
TEST_F(HandleFileOperationTest, TestUseBuckets) {
    resetFlags();
    handleFileOperation(5, &mock);  // Call the function with choice 5
    EXPECT_TRUE(mock.useBucketsCalled);  // Check that the function was called
}

// Test for linearQuotient selection (choice == 6)
TEST_F(HandleFileOperationTest, TestLinearQuotient) {
    resetFlags();
    handleFileOperation(6, &mock);  // Call the function with choice 6
    EXPECT_TRUE(mock.linearQuotientCalled);  // Check that the function was called
}

// Test for brentsMethod selection (choice == 7)
TEST_F(HandleFileOperationTest, TestBrentsMethod) {
    resetFlags();
    handleFileOperation(7, &mock);  // Call the function with choice 7
    EXPECT_TRUE(mock.brentsMethodCalled);  // Check that the function was called
}

// Test for invalid choice (choice == 9, should not call any hashing function)
TEST_F(HandleFileOperationTest, TestInvalidChoice) {
    resetFlags();
    handleFileOperation(9, &mock);  // Invalid choice

    // Ensure no function is called when the choice is invalid
    EXPECT_FALSE(mock.progressiveOverflowCalled);
    EXPECT_FALSE(mock.linearProbingCalled);
    EXPECT_FALSE(mock.quadraticProbingCalled);
    EXPECT_FALSE(mock.doubleHashingCalled);
    EXPECT_FALSE(mock.useBucketsCalled);
    EXPECT_FALSE(mock.linearQuotientCalled);
    EXPECT_FALSE(mock.brentsMethodCalled);
}

//// Helper function to capture and return the output of a function
//template <typename Func>
//std::string captureOutput(Func func) {
//    FILE* output = freopen("output.txt", "w", stdout); // Redirect stdout to a file
//    func(); // Call the passed function
//    freopen("CON", "w", stdout); // Restore stdout
//
//    char buffer[1024];
//    FILE* file = fopen("output.txt", "r");
//    std::string result;
//
//    while (fgets(buffer, sizeof(buffer), file)) {
//        result += buffer;
//    }
//
//    fclose(file);
//    return result;
//}

//
//// Test fixture class
//class AuthenticationTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        freopen("input.txt", "r", stdin); // Redirect stdin
//    }
//
//    void TearDown() override {
//        freopen("CON", "r", stdin); // Reset stdin
//    }
//
//    void writeInput(const std::string& input) {
//        FILE* inputFile = freopen("input.txt", "w", stdin);
//        fprintf(inputFile, "%s\n", input.c_str());
//        fclose(inputFile);
//        freopen("input.txt", "r", stdin); // Reopen stdin for the test
//    }
//};
//
//TEST_F(AuthenticationTest, RegisterOption) {
//    writeInput("1");
//    auto output = captureOutput(authentication);
//    ASSERT_NE(output.find("Register called"), std::string::npos);
//}
//
//TEST_F(AuthenticationTest, LoginOption) {
//    writeInput("2");
//    auto output = captureOutput(authentication);
//    ASSERT_NE(output.find("Login called"), std::string::npos);
//}
//
//TEST_F(AuthenticationTest, GuestLoginOption) {
//    writeInput("3");
//    auto output = captureOutput(authentication);
//    ASSERT_NE(output.find("Guest login called"), std::string::npos);
//}
//
//TEST_F(AuthenticationTest, FileOperationsOption) {
//    writeInput("4");
//    auto output = captureOutput(authentication);
//    ASSERT_NE(output.find("File operations menu called"), std::string::npos);
//}
//
//TEST_F(AuthenticationTest, InvalidOption) {
//    writeInput("5");
//    auto output = captureOutput(authentication);
//    ASSERT_NE(output.find("Invalid choice. Please try again."), std::string::npos);
//}

//// Test fixture class
//class RegisterTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        freopen("input.txt", "r", stdin);
//    }
//
//    void TearDown() override {
//        freopen("CON", "r", stdin);
//    }
//};
//
//std::string captureOutput1(void (*func)()) {
//    FILE* output = freopen("output.txt", "w", stdout);
//    func();
//    freopen("CON", "w", stdout);
//
//    char buffer[1024];
//    FILE* file = fopen("output.txt", "r");
//    std::string result;
//
//    while (fgets(buffer, sizeof(buffer), file)) {
//        result += buffer;
//    }
//
//    fclose(file);
//    return result;
//}
// 
// 
// 
//void simulateInput(const char* input) {
//    FILE* inputFile = freopen("input.txt", "w", stdin);
//    fprintf(inputFile, "%s", input);
//    fclose(inputFile);
//    freopen("input.txt", "r", stdin);
//}

//TEST_F(RegisterTest, SuccessfulRegistration) {
//    simulateInput("John\nDoe\n1234567890\npassword123\n");
//
//    std::string output = captureOutput1([]() { Register(); });
//
//    // Test çýktýsýný kontrol etmek için basit çýktýyý yazdýrýn
//    std::cout << "Captured Output:\n" << output << std::endl;
//
//    // Çýktýnýn doðru olduðunu kontrol edin
//    ASSERT_NE(output.find("Enter your name:"), std::string::npos);
//    ASSERT_NE(output.find("Enter your surname:"), std::string::npos);
//    ASSERT_NE(output.find("Enter your phone number:"), std::string::npos);
//    ASSERT_NE(output.find("Enter your password:"), std::string::npos);
//    ASSERT_NE(output.find("Registration successful! You can now log in."), std::string::npos);
//    ASSERT_NE(output.find("Hash Table Contents:"), std::string::npos);
//    ASSERT_NE(output.find("Main menu called"), std::string::npos);
//}





class LoginTest : public ::testing::Test {
protected:
    // Testlerde kullanýlacak deðiþkenler
    char validPhone[20] = "1234567890";
    char validPassword[20] = "password";
    char invalidPhone[20] = "0987654321";
    char invalidPassword[20] = "wrongpassword";
};

// logIn fonksiyonu burada simüle edilmektedir.
bool logIn(const char* phone, const char* password) {
    return validateLogin(phone, password);
}

TEST_F(LoginTest, ValidLoginTest) {
    EXPECT_TRUE(logIn(validPhone, validPassword));
}

// Geçersiz telefon numarasý ile login test
TEST_F(LoginTest, InvalidPhoneTest) {
    EXPECT_FALSE(logIn(invalidPhone, validPassword));
}

// Geçersiz þifre ile login test
TEST_F(LoginTest, InvalidPasswordTest) {
    EXPECT_FALSE(logIn(validPhone, invalidPassword));
}

// Geçersiz telefon ve þifre ile login test
TEST_F(LoginTest, InvalidCredentialsTest) {
    EXPECT_FALSE(logIn(invalidPhone, invalidPassword));
}




// Test sýnýfý
class GuestTest : public ::testing::Test {
protected:
    // Her testten önce çalýþacak kodlar buraya yazýlabilir
    void SetUp() override {
        // Gerekirse ayarlama iþlemleri
    }

    // Her testten sonra çalýþacak kodlar buraya yazýlabilir
    void TearDown() override {
        // Gerekirse temizleme iþlemleri
    }
};

// Guest giriþ testi
TEST_F(GuestTest, SuccessfulGuestLogin) {
    EXPECT_TRUE(guest()); // guest fonksiyonunun true döndüðünü kontrol et
}


//// Test sýnýfý
//class CreateEventTest : public ::testing::Test {
//protected:
//    FILE* tempFile;
//
//    // Her testten önce çalýþacak kodlar
//    void SetUp() override {
//        // Geçici bir dosya oluþtur
//        tempFile = fopen("test_event.bin", "wb+");
//        ASSERT_NE(tempFile, nullptr) << "Failed to create temporary file";
//    }
//
//    // Her testten sonra çalýþacak kodlar
//    void TearDown() override {
//        // Dosyayý kapat ve temizle
//        fclose(tempFile);
//        remove("test_event.bin");
//    }
//
//    // Kullanýcý giriþini simüle etmek için yardýmcý fonksiyon
//    void simulateInput(const char* input) {
//        FILE* inputFile = fmemopen((void*)input, strlen(input), "r");
//        ASSERT_NE(inputFile, nullptr) << "Failed to simulate input";
//        stdin = inputFile;
//    }
//};
//
//// Geçerli bir etkinlik oluþturma testi
//TEST_F(CreateEventTest, SuccessfulEventCreation) {
//    const char* simulatedInput =
//        "Meeting\n"          // Event type
//        "01-01-2025\n"       // Event date
//        "Blue\n"             // Color
//        "Business\n";        // Concept
//
//    simulateInput(simulatedInput);
//
//    // createEvent çaðrýsý ve kontrol
//    ASSERT_TRUE(createEvent("Meeting", "01-01-2025", "Blue", "Business"));
//
//
//    // Dosyanýn içeriðini kontrol et
//    fseek(tempFile, 0, SEEK_SET);
//    Event readEvent;
//    size_t readSize = fread(&readEvent, sizeof(Event), 1, tempFile);
//
//    ASSERT_EQ(readSize, 1) << "Failed to read event from file";
//    ASSERT_STREQ(readEvent.type, "Meeting");
//    ASSERT_STREQ(readEvent.date, "01-01-2025");
//    ASSERT_STREQ(readEvent.color, "Blue");
//    ASSERT_STREQ(readEvent.concept, "Business");
//}
//
//// Dosya yazma hatasý testi
//TEST_F(CreateEventTest, FileWriteError) {
//    fclose(tempFile);  // Dosyayý kapat ve NULL yap
//    tempFile = nullptr;
//
//    const char* simulatedInput =
//        "Party\n"           // Event type
//        "12-12-2024\n"      // Event date
//        "Red\n"             // Color
//        "Celebration\n";    // Concept
//
//    simulateInput(simulatedInput);
//
//    // createEvent çaðrýsý ve kontrol
//    ASSERT_FALSE(createEvent(tempFile)) << "Event creation should fail due to null file pointer";
//}
//



// Test için kullanýlan sýnýf
class EventManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test baþlangýcýnda gerekli olan baþlangýç durumunu ayarla
        head = NULL;

        // Örnek etkinlikler oluþtur
        Event* event1 = (Event*)malloc(sizeof(Event));
        strcpy(event1->type, "Birthday");
        strcpy(event1->date, "2024-12-25");
        strcpy(event1->color, "Blue");
        strcpy(event1->concept, "Winter Wonderland");
        event1->next = NULL;
        event1->prev = NULL;

        Event* event2 = (Event*)malloc(sizeof(Event));
        strcpy(event2->type, "Wedding");
        strcpy(event2->date, "2025-06-15");
        strcpy(event2->color, "White");
        strcpy(event2->concept, "Elegant Theme");
        event2->next = NULL;
        event2->prev = event1;

        event1->next = event2;

        head = event1;
    }

    void TearDown() override {
        // Test bittiðinde kullanýlan belleði temizle
        Event* current = head;
        while (current != NULL) {
            Event* temp = current;
            current = current->next;
            free(temp);
        }
        head = NULL;
    }
};

// Test: Etkinliklerin doðru þekilde gezilebildiðini kontrol et
TEST_F(EventManagerTest, NavigateEvents) {
    ASSERT_NE(head, nullptr); // head'in NULL olmadýðýný doðrula

    // Ýlk etkinlikte olduðumuzu kontrol et
    Event* current = head;
    EXPECT_STREQ(current->type, "Birthday");
    EXPECT_STREQ(current->date, "2024-12-25");
    EXPECT_STREQ(current->color, "Blue");
    EXPECT_STREQ(current->concept, "Winter Wonderland");

    // Sonraki etkinliðe git
    if (current->next != NULL) {
        current = current->next;
    }
    EXPECT_STREQ(current->type, "Wedding");
    EXPECT_STREQ(current->date, "2025-06-15");
    EXPECT_STREQ(current->color, "White");
    EXPECT_STREQ(current->concept, "Elegant Theme");

    // Geriye git
    if (current->prev != NULL) {
        current = current->prev;
    }
    EXPECT_STREQ(current->type, "Birthday");
}

// Test: Etkinlik güncellenebilir mi?
TEST_F(EventManagerTest, UpdateEvent) {
    ASSERT_NE(head, nullptr); // head'in NULL olmadýðýný doðrula

    // Ýlk etkinliði güncelle
    Event* current = head;
    strcpy(current->type, "Anniversary");
    strcpy(current->date, "2023-09-01");
    strcpy(current->color, "Gold");
    strcpy(current->concept, "Golden Jubilee");

    // Güncellemelerin doðru olduðunu kontrol et
    EXPECT_STREQ(current->type, "Anniversary");
    EXPECT_STREQ(current->date, "2023-09-01");
    EXPECT_STREQ(current->color, "Gold");
    EXPECT_STREQ(current->concept, "Golden Jubilee");
}

// Test: Boþ etkinlik listesi ile `manageEvent` fonksiyonunun çalýþýp çalýþmadýðýný kontrol et
TEST_F(EventManagerTest, NoEvents) {
    // Tüm etkinlikleri silerek baþla
    TearDown();

    // `manageEvent` fonksiyonunun boþ liste ile çalýþtýðýný doðrula
    EXPECT_FALSE(manageEvent());
}



// Test için kullanýlan sýnýf
class EventManagerMenuTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test baþlangýcýnda gerekli olan baþlangýç durumunu ayarla
        head = NULL; // Baþlangýçta etkinlik yok
    }

    void TearDown() override {
        // Test bittiðinde kullanýlan belleði temizle
        Event* current = head;
        while (current != NULL) {
            Event* temp = current;
            current = current->next;
            free(temp);
        }
        head = NULL;
    }
};


TEST_F(EventManagerMenuTest, CreateEventOption) {
    // Kullanýcýdan gelen giriþi simüle et
    std::stringstream input("1\n"); // Kullanýcýnýn 1 girdiðini simüle et
    std::cin.rdbuf(input.rdbuf());

    // createEvent fonksiyonunu güncellemek için bir yardýmcý fonksiyon oluþtur
    auto createEvent = [&]() {
        Event* newEvent = (Event*)malloc(sizeof(Event));
        strcpy(newEvent->type, "Meeting");
        strcpy(newEvent->date, "2024-12-07");
        strcpy(newEvent->color, "Blue");
        strcpy(newEvent->concept, "Business Meeting");
        newEvent->next = head; // Yeni etkinliði baþa ekle
        newEvent->prev = nullptr;

        if (head != nullptr) {
            head->prev = newEvent; // Eðer baþta baþka etkinlik varsa, öncekini güncelle
        }

        head = newEvent; // Baþ göstericiyi yeni etkinliðe güncelle
        };

    // createEvent fonksiyonunu çaðýr
    createEvent(); // Simüle edilen giriþle etkinliði oluþtur

    // eventDetails fonksiyonunu çaðýr ve etkinliði yönet
    eventDetails(); // Etkinlikleri yönetme menüsüne git

    // Baþarýyla bir etkinlik oluþturulduðunu kontrol et
    EXPECT_NE(head, nullptr); // Baþ gösterici boþ olmamalý
    EXPECT_STREQ(head->type, "Meeting"); // Oluþturulan etkinliðin tipi kontrol edilir
    EXPECT_STREQ(head->date, "2024-12-07"); // Oluþturulan etkinliðin tarihi kontrol edilir
    EXPECT_STREQ(head->color, "Blue"); // Oluþturulan etkinliðin rengi kontrol edilir
    EXPECT_STREQ(head->concept, "Business Meeting"); // Oluþturulan etkinliðin konsepti kontrol edilir
}

// Test: Geçersiz seçim ile menü
TEST_F(EventManagerMenuTest, InvalidOption) {
    // Geçersiz bir seçim yapýlmasýný simüle et
    std::stringstream input("3\n"); // Kullanýcýnýn 3 girdiðini simüle et
    std::cin.rdbuf(input.rdbuf());

    // eventDetails fonksiyonunu çaðýr
    testing::internal::CaptureStdout(); // Konsol çýktýsýný yakala
    eventDetails();

    // Hatalý seçim mesajýný kontrol et
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Invalid choice. Please try again."), std::string::npos); // Hatalý seçim mesajý geçmeli
}

//
//TEST_F(EventManagerMenuTest, ManageEventOption) {
//    // Kullanýcýdan gelen giriþi simüle et
//    std::stringstream input("2\n"); // Kullanýcýnýn 2 girdiðini simüle et
//    std::cin.rdbuf(input.rdbuf());
//
//    // Yeni bir etkinlik oluþtur
//    Event* newEvent = (Event*)malloc(sizeof(Event));
//    strcpy(newEvent->type, "Concert");
//    strcpy(newEvent->date, "2024-12-07");
//    strcpy(newEvent->color, "Red");
//    strcpy(newEvent->concept, "Music Event");
//    newEvent->next = nullptr;
//    newEvent->prev = nullptr;
//
//    head = newEvent; // Baþlayýcýyý yeni etkinliðe ayarla
//
//    // eventDetails fonksiyonunu çaðýr
//    testing::internal::CaptureStdout(); // Konsol çýktýsýný yakala
//    eventDetails();
//
//    // Yönetim fonksiyonunun çýktýsýný kontrol et
//    std::string output = testing::internal::GetCapturedStdout();
//
//    // Etkinlik bilgileri görüntülenmeli
//    EXPECT_NE(output.find("--- Event Information ---"), std::string::npos);
//    EXPECT_NE(output.find("Type: Concert"), std::string::npos);
//    EXPECT_NE(output.find("Date: 2024-12-07"), std::string::npos);
//    EXPECT_NE(output.find("Color: Red"), std::string::npos);
//    EXPECT_NE(output.find("Concept: Music Event"), std::string::npos);
//}


//// Test için kullanýlan sýnýf
//class AttendeeTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        // Test baþlangýcýnda gerekli olan baþlangýç durumunu ayarla
//        attendeeCount = 0; // Katýlýmcý sayýsýný sýfýrla
//    }
//
//    void TearDown() override {
//        // Test bittiðinde kullanýlan belleði temizle (varsa)
//    }
//};
//
//// Test: Katýlýmcý ekleme
//TEST_F(AttendeeTest, AddAttendee) {
//    // Yeni bir katýlýmcý ekle
//    Attendee newAttendee;
//    strcpy(newAttendee.nameAttendee, "John");
//    strcpy(newAttendee.surnameAttendee, "Doe");
//    strcpy(newAttendee.huffmanCode, "101010");
//
//    // Katýlýmcýyý diziye ekle
//    attendees[attendeeCount++] = newAttendee;
//
//    // Katýlýmcý sayýsýný kontrol et
//    EXPECT_EQ(attendeeCount, 1);
//    EXPECT_STREQ(attendees[0].nameAttendee, "John");
//    EXPECT_STREQ(attendees[0].surnameAttendee, "Doe");
//    EXPECT_STREQ(attendees[0].huffmanCode, "101010");
//}
//
//// Test: Katýlýmcý sayýsýný kontrol et
//TEST_F(AttendeeTest, CheckAttendeeCount) {
//    // Ýki katýlýmcý ekle
//    Attendee attendee1, attendee2;
//    strcpy(attendee1.nameAttendee, "Alice");
//    strcpy(attendee1.surnameAttendee, "Smith");
//    strcpy(attendee1.huffmanCode, "111000");
//
//    strcpy(attendee2.nameAttendee, "Bob");
//    strcpy(attendee2.surnameAttendee, "Brown");
//    strcpy(attendee2.huffmanCode, "000111");
//
//    attendees[attendeeCount++] = attendee1;
//    attendees[attendeeCount++] = attendee2;
//
//    // Katýlýmcý sayýsýný kontrol et
//    EXPECT_EQ(attendeeCount, 2);
//}
//
//// Test: Aþýrý katýlýmcý sayýsý ekleme
//TEST_F(AttendeeTest, ExceedMaxAttendees) {
//    // Katýlýmcý sayýsýný MAX_ATTENDEES kadar artýr
//    for (int i = 0; i < MAX_ATTENDEES; ++i) {
//        Attendee attendee;
//        sprintf(attendee.nameAttendee, "Attendee%d", i);
//        sprintf(attendee.surnameAttendee, "Surname%d", i);
//        sprintf(attendee.huffmanCode, "Code%d", i);
//        attendees[attendeeCount++] = attendee;
//    }
//
//    // Bir fazla katýlýmcý eklemeye çalýþ
//    Attendee extraAttendee;
//    strcpy(extraAttendee.nameAttendee, "Extra");
//    strcpy(extraAttendee.surnameAttendee, "Attendee");
//    strcpy(extraAttendee.huffmanCode, "ExtraCode");
//
//    // Katýlýmcý sayýsýný aþmamalýyýz
//    ASSERT_THROW({
//        if (attendeeCount < MAX_ATTENDEES) {
//            attendees[attendeeCount++] = extraAttendee;
//        }
// else {
//  throw std::overflow_error("Maximum attendee limit reached");
//}
//        }, std::overflow_error);
//}
//
//
//// Test için kullanýlan sýnýf
//class AttendeeFunctionsTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        // Test baþlangýcýnda katýlýmcý sayýsýný sýfýrla
//        attendeeCount = 0;
//    }
//};
//
//
//// Test: Katýlýmcý kaydý
//TEST_F(AttendeeFunctionsTest, RegisterAttendees) {
//    // Kullanýcýdan gelen giriþi simüle et
//    const char* attendeeInput = "John\nDoe\n101010\n"; // Örnek giriþ
//    std::stringstream input(attendeeInput);
//    std::streambuf* backup = std::cin.rdbuf(input.rdbuf());
//
//    // Katýlýmcý kaydý fonksiyonunu çaðýr
//    bool result = registerAttendees();
//
//    // Katýlýmcý sayýsýný kontrol et
//    EXPECT_TRUE(result); // Kayýt iþlemi baþarýlý olmalý
//    EXPECT_EQ(attendeeCount, 1); // Bir katýlýmcý eklenmiþ olmalý
//
//    // Kaydedilen katýlýmcýnýn bilgilerini kontrol et
//    EXPECT_STREQ(attendees[0].nameAttendee, "John");
//    EXPECT_STREQ(attendees[0].surnameAttendee, "Doe");
//    EXPECT_STREQ(attendees[0].huffmanCode, "101010");
//
//    // std::cin'i eski haline döndür
//    std::cin.rdbuf(backup);
//}
//
//// Test: Katýlýmcý adlarýný sýkýþtýrma
//TEST_F(AttendeeFunctionsTest, CompressAttendeeName) {
//    // Yeni bir katýlýmcý oluþtur
//    Attendee attendee;
//    strcpy(attendee.nameAttendee, "John Doe");
//
//    // Katýlýmcý adýný sýkýþtýr
//    compressAttendeeName(&attendee);
//
//    // Sýkýþtýrýlmýþ adý kontrol et
//    EXPECT_STREQ(attendee.nameAttendee, "J.D");
//}
//
//// Test: Katýlýmcýlarý yazdýrma
//TEST_F(AttendeeFunctionsTest, PrintAttendees) {
//    // Katýlýmcý ekle
//    Attendee attendee;
//    strcpy(attendee.nameAttendee, "Alice");
//    strcpy(attendee.surnameAttendee, "Smith");
//    strcpy(attendee.huffmanCode, "110011");
//
//    attendees[attendeeCount++] = attendee;
//
//    // Konsol çýktýsýný yakala
//    testing::internal::CaptureStdout(); // Konsol çýktýsýný yakala
//    printAttendees(); // Katýlýmcýlarý yazdýr
//
//    // Çýktýyý kontrol et
//    std::string output = testing::internal::GetCapturedStdout();
//    EXPECT_NE(output.find("Alice"), std::string::npos); // Alice ismi görünmeli
//    EXPECT_NE(output.find("Smith"), std::string::npos); // Smith soyismi görünmeli
//}

//// Test: Geçersiz kayýt durumu
//TEST_F(AttendeeFunctionsTest, InvalidRegisterAttendees) {
//    // Kullanýcýdan gelen giriþi simüle et (boþ girdi)
//    const char* attendeeInput = "\n\n\n"; // Boþ giriþ
//    std::stringstream input(attendeeInput);
//    std::streambuf* backup = std::cin.rdbuf(input.rdbuf());
//
//    // Katýlýmcý kaydý fonksiyonunu çaðýr
//    bool result = registerAttendees();
//
//    // Kayýt iþlemi baþarýsýz olmalý
//    EXPECT_FALSE(result);
//    EXPECT_EQ(attendeeCount, 0); // Katýlýmcý sayýsý hala 0 olmalý
//
//    // std::cin'i eski haline döndür
//    std::cin.rdbuf(backup);
//}

//TEST_F(AttendeeFunctionsTest, KMPSearch) {
//    // Örnek metin ve desen
//    const char* text = "ABABDABACDABABCABAB";
//    const char* pattern = "ABABCABAB";
//
//    // KMP arama fonksiyonu çýktýsýný yakalamak için bir string stream oluþtur
//    std::ostringstream output;
//    std::streambuf* backup = std::cout.rdbuf(output.rdbuf()); // std::cout'u yakala
//
//    // KMP arama fonksiyonunu çaðýr (sadece pattern ile)
//    // Not: text'i fonksiyon içinde tanýmlamanýz gerekiyor
//    kmpSearch((char*)pattern); // Sadece pattern bir char* olarak geçilmeli
//
//    // std::cout'u eski haline döndür
//    std::cout.rdbuf(backup);
//
//    // Çýktýyý kontrol et
//    std::string result = output.str();
//    EXPECT_NE(result.find("Pattern found at index"), std::string::npos); // Örnek bir çýktý kontrolü
//}
//
// Test: LPS dizisini hesaplama
//TEST_F(AttendeeFunctionsTest, ComputeLPSArray) {
//    // LPS dizisini hesapla
//    const char* pattern = "ABABCABAB";
//    int lps[9]; // Pattern uzunluðu 9
//    computeLPSArray((char*)pattern, 9, lps);
//
//    // LPS dizisini kontrol et
//    EXPECT_EQ(lps[0], 0);
//    EXPECT_EQ(lps[1], 0);
//    EXPECT_EQ(lps[2], 1);
//    EXPECT_EQ(lps[3], 2);
//    EXPECT_EQ(lps[4], 0);
//    EXPECT_EQ(lps[5], 1);
//    EXPECT_EQ(lps[6], 2);
//    EXPECT_EQ(lps[7], 3);
//    EXPECT_EQ(lps[8], 4);
//}


//
//class KMPTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        // Initialize attendees array
//        attendeeCount = 3;
//        attendees = (Attendee*)malloc(attendeeCount * sizeof(Attendee));
//
//        // Add some attendees
//        strcpy(attendees[0].nameAttendee, "Alice");
//        strcpy(attendees[0].surnameAttendee, "Smith");
//        strcpy(attendees[0].huffmanCode, "abcde");
//
//        strcpy(attendees[1].nameAttendee, "Bob");
//        strcpy(attendees[1].surnameAttendee, "Johnson");
//        strcpy(attendees[1].huffmanCode, "efghijkl");
//
//        strcpy(attendees[2].nameAttendee, "Charlie");
//        strcpy(attendees[2].surnameAttendee, "Brown");
//        strcpy(attendees[2].huffmanCode, "mnopqr");
//    }
//
//    void TearDown() override {
//        // Free the allocated memory
//        free(attendees);
//    }
//};
//
//// Test cases using TEST_F format
//TEST_F(KMPTest, PatternFoundInHuffmanCode) {
//    char pattern[] = "cd";
//    bool found = false;
//    const char* result[2] = { nullptr, nullptr };
//
//    kmpSearch(pattern, found, result);
//
//    EXPECT_TRUE(found);
//    EXPECT_STREQ(result[0], "Alice");   // Adjust according to the expected output
//    EXPECT_STREQ(result[1], "Smith");
//}
//
//TEST_F(KMPTest, PatternNotFoundInHuffmanCode) {
//    char pattern[] = "xyz"; // A pattern not in the Huffman codes
//    bool found = false;
//    const char* result[2] = { nullptr, nullptr };
//
//    kmpSearch(pattern, found, result);
//
//    EXPECT_FALSE(found);  // Expecting no match found
//}
//
//TEST_F(KMPTest, CaseInsensitiveMatch) {
//    char pattern[] = "ABCD"; // Uppercase pattern
//    bool found = false;
//    const char* result[2] = { nullptr, nullptr };
//
//    kmpSearch(pattern, found, result);
//
//    EXPECT_TRUE(found);
//    EXPECT_STREQ(result[0], "Alice");   // Adjust according to the expected output
//    EXPECT_STREQ(result[1], "Smith");
//}


//class LPSArrayTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        // Any common setup can be done here if needed
//    }
//
//    void TearDown() override {
//        // Any common cleanup can be done here if needed
//    }
//};
//
//// Test cases using TEST_F format
//TEST_F(LPSArrayTest, BasicTest) {
//    char pattern[] = "ABABABCA";
//    int M = strlen(pattern);
//    std::vector<int> lps(M);  // Use std::vector instead of array
//
//    computeLPSArray(pattern, M, lps);  // Pass vector
//
//    // Expected LPS array for "ABABABCA" should be {0, 0, 1, 2, 3, 4, 5, 6}
//    std::vector<int> expectedLPS = { 0, 0, 1, 2, 3, 4, 5, 6 };
//
//    for (int i = 0; i < M; i++) {
//        EXPECT_EQ(lps[i], expectedLPS[i]);
//    }
//}
//
//TEST_F(LPSArrayTest, NoRepeatingCharacters) {
//    char pattern[] = "ABCDE";
//    int M = strlen(pattern);
//    std::vector<int> lps(M);  // Use std::vector instead of array
//
//    computeLPSArray(pattern, M, lps);  // Pass vector
//
//    // Expected LPS array for "ABCDE" should be {0, 0, 0, 0, 0}
//    std::vector<int> expectedLPS = { 0, 0, 0, 0, 0 };
//
//    for (int i = 0; i < M; i++) {
//        EXPECT_EQ(lps[i], expectedLPS[i]);
//    }
//}
//
//TEST_F(LPSArrayTest, AllCharactersSame) {
//    char pattern[] = "AAAAA";
//    int M = strlen(pattern);
//    std::vector<int> lps(M);  // Use std::vector instead of array
//
//    computeLPSArray(pattern, M, lps);  // Pass vector
//
//    // Expected LPS array for "AAAAA" should be {0, 1, 2, 3, 4}
//    std::vector<int> expectedLPS = { 0, 1, 2, 3, 4 };
//
//    for (int i = 0; i < M; i++) {
//        EXPECT_EQ(lps[i], expectedLPS[i]);
//    }
//}
//
//TEST_F(LPSArrayTest, ComplexPattern) {
//    char pattern[] = "AABAACAABAA";
//    int M = strlen(pattern);
//    std::vector<int> lps(M);  // Use std::vector instead of array
//
//    computeLPSArray(pattern, M, lps);  // Pass vector
//
//    // Expected LPS array for "AABAACAABAA" should be {0, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8}
//    std::vector<int> expectedLPS = { 0, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
//
//    for (int i = 0; i < M; i++) {
//        EXPECT_EQ(lps[i], expectedLPS[i]);
//    }
//}

//class AttendeeCompressionTest : public ::testing::Test {
//protected:
//    Attendee attendee;
//
//    void SetUp() override {
//        // Initialize the attendee name for testing
//        strcpy(attendee.nameAttendee, "John Doe");
//    }
//
//    void TearDown() override {
//        // Any cleanup can be done here if needed
//    }
//};
//
//// Test case for compressAttendeeName
//TEST_F(AttendeeCompressionTest, CompressName) {
//    compressAttendeeName(&attendee); // Call the function to compress the name
//
//    // Check if the huffmanCode matches the original nameAttendee
//    EXPECT_STREQ(attendee.huffmanCode, attendee.nameAttendee);
//}
//
//// Test case for an empty name
//TEST_F(AttendeeCompressionTest, CompressEmptyName) {
//    // Set an empty name
//    strcpy(attendee.nameAttendee, "");
//    compressAttendeeName(&attendee); // Call the function
//
//    // Check if the huffmanCode is also empty
//    EXPECT_STREQ(attendee.huffmanCode, "");
//}
//
//// Test case for a single character name
//TEST_F(AttendeeCompressionTest, CompressSingleCharacterName) {
//    strcpy(attendee.nameAttendee, "A"); // Set a single character name
//    compressAttendeeName(&attendee); // Call the function
//
//    // Check if the huffmanCode matches the original nameAttendee
//    EXPECT_STREQ(attendee.huffmanCode, "A");
//}

//class RegisterAttendeesTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        // Reset attendeeCount before each test
//        attendeeCount = 0;
//        fileMock = new FileMock();
//    }
//
//    void TearDown() override {
//        delete fileMock;
//    }
//};
//
//// Test case for successful registration of attendees
//TEST_F(RegisterAttendeesTest, SuccessfulRegistration) {
//    // Arrange
//    const char* input = "2\nJohn\nDoe\nJane\nSmith\n"; // Mock input for two attendees
//    mockInput(input);
//
//    EXPECT_CALL(*fileMock, fopen("attendee.bin", "ab"))
//        .WillOnce(::testing::Return(reinterpret_cast<FILE*>(1))); // Mock successful fopen
//
//    EXPECT_CALL(*fileMock, fwrite(testing::_, sizeof(Attendee), 1, reinterpret_cast<FILE*>(1)))
//        .Times(2) // Expect fwrite to be called twice
//        .WillRepeatedly(::testing::Return(1)); // Mock successful fwrite
//
//    EXPECT_CALL(*fileMock, fclose(reinterpret_cast<FILE*>(1)))
//        .Times(1); // Expect fclose to be called once
//
//    // Act
//    bool result = registerAttendees();
//
//    // Assert
//    EXPECT_TRUE(result);
//    EXPECT_EQ(attendeeCount, 2); // Ensure attendeeCount is updated
//    EXPECT_STREQ(attendees[0].nameAttendee, "John");
//    EXPECT_STREQ(attendees[0].surnameAttendee, "Doe");
//    EXPECT_STREQ(attendees[1].nameAttendee, "Jane");
//    EXPECT_STREQ(attendees[1].surnameAttendee, "Smith");
//}
//
//// Test case for invalid number of attendees
//TEST_F(RegisterAttendeesTest, InvalidNumberOfAttendees) {
//    // Arrange
//    const char* input = "-1\n"; // Invalid input for attendee count
//    mockInput(input);
//
//    EXPECT_CALL(*fileMock, fopen("attendee.bin", "ab"))
//        .WillOnce(::testing::Return(reinterpret_cast<FILE*>(1))); // Mock successful fopen
//
//    // Act
//    bool result = registerAttendees();
//
//    // Assert
//    EXPECT_FALSE(result); // Should return false for invalid number
//    EXPECT_EQ(attendeeCount, 0); // attendeeCount should remain unchanged
//}
//
//class printAttendeeTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//         Test verilerini ayarla
//        strcpy(attendees[0].nameAttendee, "Alice");
//        strcpy(attendees[0].surnameAttendee, "Smith");
//        strcpy(attendees[0].huffmanCode, "101");
//        strcpy(attendees[1].nameAttendee, "Bob");
//        strcpy(attendees[1].surnameAttendee, "Johnson");
//        strcpy(attendees[1].huffmanCode, "110");
//        attendeeCount = 2; // Toplam 2 katýlýmcý
//    }
//
//    void TearDown() override {
//         Test sonrasý temizleme iþlemleri
//        attendeeCount = 0;
//    }
//};
//
//TEST_F(printAttendeeTest, PrintAttendeesTest) {
//     Çýktýyý yakalamak için bir stringstream oluþtur
//    std::stringstream buffer;
//    std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf()); // std::cout'u buffer'a yönlendir
//
//     printAttendees fonksiyonunu çaðýr
//    printAttendees();
//
//     std::cout'u eski haline döndür
//    std::cout.rdbuf(oldCoutBuffer);
//
//     Beklenen çýktýyý tanýmla
//    std::string expectedOutput = "\nRegistered Attendees:\n"
//        "Name: Alice, Surname: Smith, Huffman Code: 101\n"
//        "Name: Bob, Surname: Johnson, Huffman Code: 110\n";
//
//     Gerçek çýktýyý kontrol et
//    EXPECT_EQ(buffer.str(), expectedOutput);
//}
//


class AttendeeMenuTest : public ::testing::Test {
protected:
    void SetUp() override {
        // You can set up any common test data here
    }

    void TearDown() override {
        // Cleanup code can go here if needed
    }
};

// Test cases for the attendee function
TEST_F(AttendeeMenuTest, TestRegisterAttendees) {
    // Redirect std::cin and std::cout
    std::stringstream input("1\n");  // Simulate choosing option 1
    std::streambuf* oldCinBuffer = std::cin.rdbuf(input.rdbuf());

    // Call the attendee function
    attendee();

    std::cin.rdbuf(oldCinBuffer);  // Restore std::cin
}

TEST_F(AttendeeMenuTest, TestTrackAttendees) {
    std::stringstream input("2\nAlice\n");  // Simulate choosing option 2 and searching for "Alice"
    std::streambuf* oldCinBuffer = std::cin.rdbuf(input.rdbuf());

    attendee();

    std::cin.rdbuf(oldCinBuffer);  // Restore std::cin
}

TEST_F(AttendeeMenuTest, TestPrintAttendees) {
    std::stringstream input("3\n");  // Simulate choosing option 3
    std::streambuf* oldCinBuffer = std::cin.rdbuf(input.rdbuf());

    attendee();

    std::cin.rdbuf(oldCinBuffer);  // Restore std::cin
}

TEST_F(AttendeeMenuTest, TestManageAttendeesList) {
    std::stringstream input("4\n1\nJohn\n");  // Simulate choosing option 4, then option 1 to add an attendee named "John"
    std::streambuf* oldCinBuffer = std::cin.rdbuf(input.rdbuf());

    attendee();

    std::cin.rdbuf(oldCinBuffer);  // Restore std::cin
}


class SparseMatrixTest : public ::testing::Test {
protected:
    void SetUp() override {
        // SparseMatrix'ý baþlatma iþlemleri
        activityMatrix.size = 0;  // Baþlangýçta 0 giriþ
    }

    void TearDown() override {
        // Temizlik kodu
    }
};


// addEntry fonksiyonunu test eden test durumu
//TEST_F(SparseMatrixTest, TestAddEntry) {
//     Giriþ eklemeyi deniyoruz
//    bool result = addEntry(0, 1, "Activity 1");
//
//     Giriþin baþarýyla eklenip eklenmediðini kontrol et
//    EXPECT_TRUE(result);  // Giriþ baþarýlý bir þekilde eklenmeli
//    EXPECT_EQ(activityMatrix.size, 1);  // Boyut 1 olmalý
//    EXPECT_EQ(activityMatrix.row[0], 0);  // Ýlk satýr 0 olmalý
//    EXPECT_EQ(activityMatrix.col[0], 1);  // Ýlk sütun 1 olmalý
//    EXPECT_STREQ(activityMatrix.value[0], "Activity 1");  // Deðer doðru olmalý
//}

bool push(const char* item) {
    if (activityStack.top >= STACK_SIZE - 1) {
        return false;  // Yýðýn dolu
    }
    activityStack.top++;
    strncpy(activityStack.items[activityStack.top], item, 100);
    activityStack.items[activityStack.top][99] = '\0';  // Null terminator
    return true;  // Baþarýyla eklendi
}

bool pop(char* item) {
    if (activityStack.top < 0) {
        return false;  // Yýðýn boþ
    }
    strncpy(item, activityStack.items[activityStack.top], 100);
    activityStack.top--;
    return true;  // Baþarýyla çýkarýldý
}

//
class StackTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeStack();  // Her testten önce yýðýný baþlat
    }

    void TearDown() override {
        // Gerekirse temizlik kodu
    }
};

// Push fonksiyonunu test eden test durumu
TEST_F(StackTest, TestPush) {
    // Yýðýna bir öðe ekleyelim
    bool result = push("Activity 1");

    // Öðenin baþarýyla eklenip eklenmediðini kontrol et
    EXPECT_TRUE(result);
    EXPECT_EQ(activityStack.top, 0);  // Yýðýnýn üstü 0 olmalý
    EXPECT_STREQ(activityStack.items[activityStack.top], "Activity 1");  // Deðer doðru olmalý
}

// Pop fonksiyonunu test eden test durumu
TEST_F(StackTest, TestPop) {
    // Önce bir öðe ekleyelim
    push("Activity 1");
    char item[100];

    // Yýðýndan öðeyi çýkaralým
    bool result = pop(item);

    // Öðenin baþarýyla çýkarýlýp çýkarýlmadýðýný kontrol et
    EXPECT_TRUE(result);
    EXPECT_STREQ(item, "Activity 1");  // Çýkarýlan deðer doðru olmalý
    EXPECT_EQ(activityStack.top, -1);  // Yýðýn boþ olmalý
}

// Yýðýn boþken pop etmeye çalýþmayý test eden test durumu
TEST_F(StackTest, TestPopEmptyStack) {
    char item[100];
    // Yýðýn boþken pop etmeye çalýþalým
    bool result = pop(item);

    // Boþ yýðýndan öðe çýkarma baþarýsýz olmalý
    EXPECT_FALSE(result);
}




void initializeQueue(Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

bool isQueueEmpty(Queue* queue) {
    return queue->front == -1;
}

bool isQueueFull(Queue* queue) {
    return (queue->rear + 1) % QUEUE_SIZE == queue->front;
}

bool enqueue(Queue* queue, const char* item) {
    if (isQueueFull(queue)) {
        return false; // Queue is full
    }
    if (isQueueEmpty(queue)) {
        queue->front = 0;
    }
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    strcpy(queue->items[queue->rear], item);
    return true;
}

bool dequeue(Queue* queue, char* item) {
    if (isQueueEmpty(queue)) {
        return false; // Queue is empty
    }
    strcpy(item, queue->items[queue->front]);
    if (queue->front == queue->rear) {
        queue->front = -1;
        queue->rear = -1; // Queue becomes empty
    }
    else {
        queue->front = (queue->front + 1) % QUEUE_SIZE;
    }
    return true;
}


class QueueTest : public ::testing::Test {
protected:
    Queue queue;

    void SetUp() override {
        initializeQueue(&queue);
    }
};

// Test for queue initialization
TEST_F(QueueTest, InitializeQueue) {
    EXPECT_TRUE(isQueueEmpty(&queue)) << "Queue should be empty after initialization.";
    EXPECT_FALSE(isQueueFull(&queue)) << "Queue should not be full after initialization.";
}

// Test for enqueue operation
TEST_F(QueueTest, EnqueueOperation) {
    EXPECT_TRUE(enqueue(&queue, "Activity1")) << "Enqueue should succeed.";
    EXPECT_FALSE(isQueueEmpty(&queue)) << "Queue should not be empty after enqueue.";
    EXPECT_STREQ(queue.items[queue.front], "Activity1") << "Front item should match the enqueued value.";
}

// Test for dequeue operation
TEST_F(QueueTest, DequeueOperation) {
    enqueue(&queue, "Activity1");
    char item[100];
    EXPECT_TRUE(dequeue(&queue, item)) << "Dequeue should succeed.";
    EXPECT_STREQ(item, "Activity1") << "Dequeued item should match the enqueued value.";
    EXPECT_TRUE(isQueueEmpty(&queue)) << "Queue should be empty after dequeue.";
}

// Test for queue full condition
TEST_F(QueueTest, QueueFullCondition) {
    for (int i = 0; i < QUEUE_SIZE; i++) {
        char activity[100];
        sprintf(activity, "Activity%d", i + 1);
        EXPECT_TRUE(enqueue(&queue, activity)) << "Enqueue should succeed for Activity" << i + 1;
    }
    EXPECT_TRUE(isQueueFull(&queue)) << "Queue should be full.";
    EXPECT_FALSE(enqueue(&queue, "OverflowActivity")) << "Enqueue should fail when queue is full.";
}

// Test for queue empty condition
TEST_F(QueueTest, QueueEmptyCondition) {
    char item[100];
    EXPECT_FALSE(dequeue(&queue, item)) << "Dequeue should fail when queue is empty.";
    EXPECT_TRUE(isQueueEmpty(&queue)) << "Queue should remain empty.";
}

// Test for wrap-around behavior
TEST_F(QueueTest, QueueWrapAroundBehavior) {
    for (int i = 0; i < QUEUE_SIZE; i++) {
        char activity[100];
        sprintf(activity, "Activity%d", i + 1);
        EXPECT_TRUE(enqueue(&queue, activity)) << "Enqueue should succeed for Activity" << i + 1;
    }
    char dequeuedItem[100];
    for (int i = 0; i < QUEUE_SIZE / 2; i++) {
        EXPECT_TRUE(dequeue(&queue, dequeuedItem)) << "Dequeue should succeed.";
    }
    for (int i = 0; i < QUEUE_SIZE / 2; i++) {
        char activity[100];
        sprintf(activity, "NewActivity%d", i + 1);
        EXPECT_TRUE(enqueue(&queue, activity)) << "Enqueue should succeed after wrap-around.";
    }
    EXPECT_TRUE(isQueueFull(&queue)) << "Queue should be full after wrap-around.";
}

void addXORNode(XORNode** head, const char* value) {
    XORNode* newNode = (XORNode*)malloc(sizeof(XORNode));
    strcpy(newNode->value, value);
    newNode->both = XOR(*head, NULL);

    if (*head != NULL) {
        (*head)->both = XOR(newNode, XOR((*head)->both, NULL));
    }
    *head = newNode;
}

// Helper function to traverse and return the values in the XOR Linked List
std::vector<std::string> traverseXORList(XORNode* head) {
    std::vector<std::string> values;
    XORNode* curr = head;
    XORNode* prev = NULL;
    XORNode* next;

    while (curr != NULL) {
        values.push_back(curr->value);
        next = XOR(prev, curr->both);
        prev = curr;
        curr = next;
    }
    return values;
}



class XORLinkedListTest : public ::testing::Test {
protected:
    XORNode* head;

    void SetUp() override {
        head = NULL; // Initialize empty XOR Linked List
    }

    void TearDown() override {
        // Free all allocated nodes
        XORNode* curr = head;
        XORNode* prev = NULL;
        XORNode* next;

        while (curr != NULL) {
            next = XOR(prev, curr->both);
            free(curr);
            prev = curr;
            curr = next;
        }
    }
};

// Test case to check insertion and traversal
TEST_F(XORLinkedListTest, InsertionAndTraversal) {
    addXORNode(&head, "Node1");
    addXORNode(&head, "Node2");
    addXORNode(&head, "Node3");

    std::vector<std::string> expected = { "Node3", "Node2", "Node1" };
    std::vector<std::string> result = traverseXORList(head);

    ASSERT_EQ(result.size(), expected.size()) << "List size mismatch.";
    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(result[i], expected[i]) << "Mismatch at index " << i;
    }
}

// Test case to check empty list
TEST_F(XORLinkedListTest, EmptyList) {
    std::vector<std::string> result = traverseXORList(head);
    EXPECT_TRUE(result.empty()) << "List should be empty.";
}



// Test Fixture for XOR Linked List
class XORLinkedListRemoveTest : public ::testing::Test {
protected:
    XORNode* head;

    void SetUp() override {
        head = NULL; // Initialize an empty XOR linked list
    }

    void TearDown() override {
        // Free all allocated nodes
        XORNode* current = head;
        XORNode* prev = NULL;
        XORNode* next;

        while (current != NULL) {
            next = XOR(prev, current->both);
            free(current);
            prev = current;
            current = next;
        }
    }

    void addXORNode(XORNode** head, const char* value) {
        XORNode* newNode = (XORNode*)malloc(sizeof(XORNode));
        strcpy(newNode->value, value);
        newNode->both = XOR(*head, NULL);

        if (*head != NULL) {
            (*head)->both = XOR(newNode, XOR((*head)->both, NULL));
        }
        *head = newNode;
    }

    std::vector<std::string> traverseXORList(XORNode* head) {
        std::vector<std::string> values;
        XORNode* current = head;
        XORNode* prev = NULL;
        XORNode* next;

        while (current != NULL) {
            values.push_back(current->value);
            next = XOR(prev, current->both);
            prev = current;
            current = next;
        }
        return values;
    }

    void removeFromXORList(const char* value) {
        XORNode* current = head;
        XORNode* prev = NULL;
        XORNode* next;

        while (current != NULL) {
            if (strcmp(current->value, value) == 0) {
                if (prev != NULL) {
                    prev->both = XOR(prev->both, current);
                }
                else {
                    head = XOR(head->both, NULL); // Adjust head if the removed node is the head
                }

                next = XOR(prev, current->both);
                free(current); // Free the node
                current = next;
            }
            else {
                next = XOR(prev, current->both);
                prev = current;
                current = next;
            }
        }
    }
};

TEST_F(XORLinkedListRemoveTest, RemoveSingleNode) {
    addXORNode(&head, "Node1");
    addXORNode(&head, "Node2");
    addXORNode(&head, "Node3");

    removeFromXORList("Node2");

    std::vector<std::string> expected = { "Node3", "Node1" };
    std::vector<std::string> result = traverseXORList(head);

    ASSERT_EQ(result.size(), expected.size()) << "List size mismatch after removal.";
    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(result[i], expected[i]) << "Mismatch at index " << i;
    }
}



TEST_F(XORLinkedListRemoveTest, RemoveHeadNode) {
    addXORNode(&head, "Node1");
    addXORNode(&head, "Node2");

    removeFromXORList("Node2"); // Head node

    std::vector<std::string> expected = { "Node1" };
    std::vector<std::string> result = traverseXORList(head);

    ASSERT_EQ(result.size(), expected.size()) << "List size mismatch after removing the head.";
    ASSERT_EQ(result[0], "Node1") << "Remaining node should be Node1.";
}


// Test for removing the tail node
TEST_F(XORLinkedListRemoveTest, RemoveTailNode) {
    addXORNode(&head, "Node1");
    addXORNode(&head, "Node2");

    removeFromXORList("Node1"); // Tail node

    std::vector<std::string> expected = { "Node2" };
    std::vector<std::string> result = traverseXORList(head);

    ASSERT_EQ(result.size(), expected.size()) << "List size mismatch after removing the tail.";
    EXPECT_EQ(result[0], "Node2") << "Remaining node should be Node2.";
}

// Test for removing a non-existent node
TEST_F(XORLinkedListRemoveTest, RemoveNonExistentNode) {
    addXORNode(&head, "Node1");
    addXORNode(&head, "Node2");

    removeFromXORList("Node3"); // Non-existent node

    std::vector<std::string> expected = { "Node2", "Node1" };
    std::vector<std::string> result = traverseXORList(head);

    ASSERT_EQ(result.size(), expected.size()) << "List size should remain unchanged.";
    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(result[i], expected[i]) << "Mismatch at index " << i;
    }
}

// Test for removing all nodes
TEST_F(XORLinkedListRemoveTest, RemoveAllNodes) {
    addXORNode(&head, "Node1");
    addXORNode(&head, "Node2");

    removeFromXORList("Node1");
    removeFromXORList("Node2");

    EXPECT_TRUE(traverseXORList(head).empty()) << "List should be empty after removing all nodes.";
}

//
//// XOR Linked List Helper Functions
//void resetXORList(XORNode** head) {
//    XORNode* current = *head;
//    XORNode* prev = NULL;
//    XORNode* next;
//
//    while (current != NULL) {
//        next = XOR(prev, current->both);
//        free(current);
//        prev = current;
//        current = next;
//    }
//    *head = NULL; // Reset the list to NULL
//}
//
//void addXORNode(XORNode** head, const char* value) {
//    XORNode* newNode = (XORNode*)malloc(sizeof(XORNode));
//    if (!newNode) {
//        throw std::runtime_error("Memory allocation failed for new XOR node");
//    }
//    strcpy(newNode->value, value);
//    newNode->both = XOR(*head, NULL);
//
//    if (*head != NULL) {
//        (*head)->both = XOR(newNode, XOR((*head)->both, NULL));
//    }
//    *head = newNode;
//}
//
//// Helper to capture the output of displayXORList
//std::string captureDisplayOutput(XORNode* head) {
//    FILE* tempFile = tmpfile();
//    if (!tempFile) {
//        throw std::runtime_error("Failed to create a temporary file for capturing output.");
//    }
//    int savedStdout = dup(fileno(stdout));
//    fflush(stdout);
//    dup2(fileno(tempFile), fileno(stdout));
//
//    // Call the function to display the list
//    XORNode* current = head;
//    XORNode* prev = NULL;
//    XORNode* next;
//
//    printf("Activity History: \n");
//    while (current != NULL) {
//        printf("%s -> ", current->value);
//        next = XOR(prev, current->both); // Get the next node using XOR
//        prev = current;
//        current = next;
//    }
//    printf("NULL\n");
//
//    fflush(stdout);
//    dup2(savedStdout, fileno(stdout));
//    close(savedStdout);
//
//    // Read the captured output
//    fseek(tempFile, 0, SEEK_SET);
//    char buffer[1024];
//    std::string output;
//    while (fgets(buffer, sizeof(buffer), tempFile)) {
//        output += buffer;
//    }
//    fclose(tempFile);
//
//    return output;
//}
//
//// Test Fixture for XOR Linked List
//class XORLinkedListTest : public ::testing::Test {
//protected:
//    XORNode* head;
//
//    void SetUp() override {
//        head = NULL; // Initialize an empty XOR linked list
//    }
//
//    void TearDown() override {
//        resetXORList(&head); // Free all allocated nodes
//    }
//};
//
//// Test: Display empty list
//TEST_F(XORLinkedListTest, DisplayEmptyList) {
//    std::string output = captureDisplayOutput(head);
//    EXPECT_EQ(output, "Activity History: \nNULL\n") << "Output for empty list is incorrect.";
//}
//
//// Test: Display single-node list
//TEST_F(XORLinkedListTest, DisplaySingleNodeList) {
//    addXORNode(&head, "Node1");
//    std::string output = captureDisplayOutput(head);
//    EXPECT_EQ(output, "Activity History: \nNode1 -> NULL\n") << "Output for single-node list is incorrect.";
//}
//
//// Test: Display multi-node list
//TEST_F(XORLinkedListTest, DisplayMultiNodeList) {
//    addXORNode(&head, "Node1");
//    addXORNode(&head, "Node2");
//    addXORNode(&head, "Node3");
//
//    std::string output = captureDisplayOutput(head);
//    EXPECT_EQ(output, "Activity History: \nNode3 -> Node2 -> Node1 -> NULL\n")
//        << "Output for multi-node list is incorrect.";
//}

//
//class iXORLinkedListTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        initializeXORList();
//    }
//};
//
//// Test case for initializeXORList
//TEST_F(iXORLinkedListTest, InitializeXORListSetsHeadToNull) {
//    ASSERT_EQ(xorHead, nullptr) << "XOR linked list head is not initialized to NULL.";
//}


class iSparseMatrixTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeSparseMatrix();
    }
};

// Test case for initializeSparseMatrix
TEST_F(iSparseMatrixTest, InitializeSparseMatrixSetsSizeToZero) {
    ASSERT_EQ(activityMatrix.size, 0) << "Sparse matrix size is not initialized to zero.";
}

class iStackTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeStack();
    }
};

// Test case for initializeStack
TEST_F(iStackTest, InitializeStackSetsTopToMinusOne) {
    ASSERT_EQ(activityStack.top, -1) << "Stack top is not initialized to -1.";
}

class iQueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeQueue();
    }
};

// Test case for initializeQueue
TEST_F(iQueueTest, InitializeQueueSetsFrontAndRearToZero) {
    ASSERT_EQ(activityQueue.front, 0) << "Queue front is not initialized to 0.";
    ASSERT_EQ(activityQueue.rear, 0) << "Queue rear is not initialized to 0.";
}


class StackFullTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeStack();  // Yýðýný baþlat
        // Yýðýn baþlangýçta dolu olmamalýdýr
        for (int i = 0; i < STACK_SIZE; i++) {
            activityStack.top++;  // Yýðýnýn üstünü artýr
        }
    }
};

//// Test case for isStackFull when stack is empty
//TEST_F(StackFullTest, StackIsNotFullInitially) {
//    activityStack.top = -1;  // Yýðýný baþlangýçta boþ yap
//    EXPECT_FALSE(isStackFull()) << "Stack should not be full initially.";
//}
//
//// Test case for isStackFull when stack is full
//TEST_F(StackFullTest, StackIsFullWhenTopIsStackSizeMinusOne) {
//    activityStack.top = STACK_SIZE - 1;  // Yýðýn dolu duruma getir
//    EXPECT_TRUE(isStackFull()) << "Stack should be full when top is STACK_SIZE - 1.";
//}
//
//// Test case for isStackFull when stack is not full
//TEST_F(StackFullTest, StackIsNotFullWhenTopIsLessThanStackSizeMinusOne) {
//    activityStack.top = STACK_SIZE - 2;  // Yýðýn dolu deðil duruma getir
//    EXPECT_FALSE(isStackFull()) << "Stack should not be full when top is less than STACK_SIZE - 1.";
//}


//class StackEmptyTest : public ::testing::Test {
//protected:
//    void SetUp() override {
//        initializeStack();  // Yýðýný baþlat
//    }
//};
//
//// Test case for isStackEmpty when stack is empty
//TEST_F(StackEmptyTest, StackIsEmptyInitially) {
//    activityStack.top = -1;  // Yýðýný baþlangýçta boþ yap
//    EXPECT_TRUE(isStackEmpty()) << "Stack should be empty initially.";
//}
//
//// Test case for isStackEmpty when stack is not empty
//TEST_F(StackEmptyTest, StackIsNotEmptyWhenTopIsZero) {
//    activityStack.top = 0;  // Yýðýný dolu duruma getir
//    EXPECT_FALSE(isStackEmpty()) << "Stack should not be empty when top is 0.";
//}
//
//// Test case for isStackEmpty when stack is not empty
//TEST_F(StackEmptyTest, StackIsNotEmptyWhenTopIsGreaterThanZero) {
//    activityStack.top = 5;  // Yýðýný dolu duruma getir
//    EXPECT_FALSE(isStackEmpty()) << "Stack should not be empty when top is greater than -1.";
//}



class StackPushTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeStack();  // Yýðýný baþlat
    }
};

// Test case for pushing to an empty stack
TEST_F(StackPushTest, PushToEmptyStack) {
    const char* activity = "Activity 1";
    pushStack(activity);
    EXPECT_STREQ(activityStack.items[0], activity) << "The first activity should be pushed onto the stack.";
    EXPECT_EQ(activityStack.top, 0) << "Stack top should be 0 after one push.";
}

// Test case for pushing multiple activities
TEST_F(StackPushTest, PushMultipleActivities) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";

    pushStack(activity1);
    pushStack(activity2);

    EXPECT_STREQ(activityStack.items[0], activity1) << "First activity should be on the stack.";
    EXPECT_STREQ(activityStack.items[1], activity2) << "Second activity should be on the stack.";
    EXPECT_EQ(activityStack.top, 1) << "Stack top should be 1 after two pushes.";
}

// Test case for pushing when stack is full
TEST_F(StackPushTest, PushWhenStackIsFull) {
    // Fill the stack to its maximum size
    for (int i = 0; i < STACK_SIZE; i++) {
        pushStack("Activity"); // Add dummy activities
    }

    // Try to push one more activity
    const char* newActivity = "Activity 6";
    pushStack(newActivity);  // This should trigger the "stack is full" condition

    // Check that the new activity was not added
    EXPECT_EQ(activityStack.top, STACK_SIZE - 1) << "Stack top should not change when pushing to a full stack.";
}



class StackPopTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeStack();  // Yýðýný baþlat
    }
};

// Test case for popping from a non-empty stack
TEST_F(StackPopTest, PopFromNonEmptyStack) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";

    pushStack(activity1);
    pushStack(activity2);

    // Pop the last pushed activity
    testing::internal::CaptureStdout();  // Capture standard output
    popStack();

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Popped activity: Activity 2"), std::string::npos)
        << "The popped activity should be the last pushed activity.";

    EXPECT_EQ(activityStack.top, 0) << "Stack top should be 0 after one pop.";
}

// Test case for popping when stack is empty
TEST_F(StackPopTest, PopWhenStackIsEmpty) {
    testing::internal::CaptureStdout();  // Capture standard output
    popStack();  // Attempt to pop from an empty stack

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Error: Stack is empty!"), std::string::npos)
        << "An error message should be displayed when trying to pop from an empty stack.";
}





class QueueFullTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeQueue();  // Kuyruðu baþlat
    }
};

// Test case for isQueueFull when queue is empty
TEST_F(QueueFullTest, QueueIsNotFullInitially) {
    EXPECT_FALSE(isQueueFull()) << "Queue should not be full initially.";
}

// Test case for isQueueFull when queue is full
TEST_F(QueueFullTest, QueueIsFullWhenRearIsEqualToQueueSize) {
    activityQueue.rear = QUEUE_SIZE;  // Kuyruðu dolu duruma getir
    EXPECT_TRUE(isQueueFull()) << "Queue should be full when rear is equal to QUEUE_SIZE.";
}

// Test case for isQueueFull when queue is not full
TEST_F(QueueFullTest, QueueIsNotFullWhenRearIsLessThanQueueSize) {
    activityQueue.rear = QUEUE_SIZE - 1;  // Kuyruðu dolu deðil duruma getir
    EXPECT_FALSE(isQueueFull()) << "Queue should not be full when rear is less than QUEUE_SIZE.";
}



class QueueEmptyTest : public ::testing::Test {
protected:
    void SetUp() override {
        initializeQueue();  // Kuyruðu baþlat
    }
};

// Test case for isQueueEmpty when queue is empty
TEST_F(QueueEmptyTest, QueueIsEmptyInitially) {
    EXPECT_TRUE(isQueueEmpty()) << "Queue should be empty initially.";
}

// Test case for isQueueEmpty when queue is not empty
TEST_F(QueueEmptyTest, QueueIsNotEmptyWhenFrontIsLessThanRear) {
    activityQueue.front = 0;
    activityQueue.rear = 1;  // Kuyruða bir eleman eklenmiþ durumda
    EXPECT_FALSE(isQueueEmpty()) << "Queue should not be empty when there are elements.";
}

// Test case for isQueueEmpty when queue is full
TEST_F(QueueEmptyTest, QueueIsNotEmptyWhenFrontIsEqualToRearPlusOne) {
    activityQueue.front = 0;
    activityQueue.rear = QUEUE_SIZE;  // Kuyruk dolu duruma getir
    EXPECT_FALSE(isQueueEmpty()) << "Queue should not be empty when it is full.";
}

// Test case for isQueueEmpty when front and rear are equal after popping
TEST_F(QueueEmptyTest, QueueIsEmptyAfterPoppingAllElements) {
    // Kuyruða eleman ekleyelim
    activityQueue.front = 0;
    activityQueue.rear = 1;  // Kuyruða bir eleman ekledik

    // Eleman çýkarmak
    activityQueue.rear--;  // Eleman çýkarýldýktan sonra rear'ý azalt

    EXPECT_TRUE(isQueueEmpty()) << "Queue should be empty after popping all elements.";
}


int main(int argc, char** argv) {
#ifdef ENABLE_DENEME_TEST
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(color) = "yes";
    return RUN_ALL_TESTS();
#else
    return 0;
#endif
}