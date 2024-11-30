#include <cstring>
#include "gtest/gtest.h"
#include "../../event/header/event.h"  
#include "../../event/src/event.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
#include <cstdio>
#define ENABLE_DENEME_TEST

class MinHeapTest : public ::testing::Test {
protected:
    MinHeap* heap;
    MinHeapNode* node;  // Eksik olan de�i�ken burada tan�mlan�yor

    // Test �ncesi setup
    void SetUp() override {
        heap = nullptr;  // Null olarak ba�lat�yoruz
        node = nullptr;  // Null olarak ba�lat�yoruz
    }

    // Test sonras� cleanup
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

// Heap olu�turma testi: Ge�erli kapasite ile
TEST_F(MinHeapTest, CreateMinHeapWithValidCapacity) {
    unsigned capacity = 10;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed, returned nullptr.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size should be zero.";
    EXPECT_EQ(heap->capacity, capacity) << "Heap capacity should match the given value.";
    EXPECT_NE(heap->array, nullptr) << "Heap array should be allocated properly.";
}

// Heap olu�turma testi: S�f�r kapasite ile
TEST_F(MinHeapTest, CreateMinHeapWithZeroCapacity) {
    unsigned capacity = 0;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed for zero capacity.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size should be zero.";
    EXPECT_EQ(heap->capacity, 0) << "Heap capacity should be zero.";
    EXPECT_EQ(heap->array, nullptr) << "Heap array should be nullptr for zero capacity.";
}

// Heap olu�turma testi: B�y�k kapasite ile
TEST_F(MinHeapTest, CreateMinHeapWithLargeCapacity) {
    unsigned capacity = 100000;
    heap = createMinHeap(capacity);

    ASSERT_NE(heap, nullptr) << "Heap creation failed for large capacity.";
    EXPECT_EQ(heap->size, 0) << "Initial heap size should be zero for large capacity.";
    EXPECT_EQ(heap->capacity, capacity) << "Heap capacity should match the given value.";
    EXPECT_NE(heap->array, nullptr) << "Heap array should be allocated properly for large capacity.";
}

// MinHeapNode olu�turma testi: Ge�erli veri ile
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
        // Gerekli ba�lang�� ayarlar� yap�labilir (�u an i�in bo�)
    }

    void TearDown() override {
        // Test sonras� temizlik i�lemleri yap�labilir (�u an i�in bo�)
    }
};

TEST_F(HashFunctionTest, HashProducesConsistentResults) {
    const char* input1 = "1234567890";
    const char* input2 = "0987654321";

    // Ayn� girdiler i�in ayn� hash de�eri �retilmesi beklenir
    unsigned int hash1 = hash(input1);
    unsigned int hash1_repeat = hash(input1);
    ASSERT_EQ(hash1, hash1_repeat);

    // Farkl� girdiler i�in farkl� hash de�erleri beklenir (muhtemel)
    unsigned int hash2 = hash(input2);
    ASSERT_NE(hash1, hash2);
}

TEST_F(HashFunctionTest, HashHandlesEmptyInput) {
    const char* emptyInput = "";
    unsigned int hashValue = hash(emptyInput);

    // Bo� bir giri� i�in hash de�eri 0 olabilir (�rnek beklenti)
    ASSERT_EQ(hashValue, 0);
}

TEST_F(HashFunctionTest, HashIsWithinTableSize) {
    const char* input = "1234567890";

    // Hash de�erinin TABLE_SIZE s�n�rlar� i�inde oldu�unu do�rula
    unsigned int hashValue = hash(input);
    ASSERT_LT(hashValue, TABLE_SIZE);
}

TEST_F(HashFunctionTest, HashHandlesSpecialCharacters) {
    const char* specialInput = "!@#$%^&*()";

    // �zel karakterlerin hash hesaplamas�nda bir sorun yaratmad���n� kontrol et
    unsigned int hashValue = hash(specialInput);

    // Sadece TABLE_SIZE s�n�r�nda oldu�unu kontrol ediyoruz
    ASSERT_LT(hashValue, TABLE_SIZE);
}


class SaveUserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Hash tablosunu s�f�rla
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashTable[i] = nullptr;
        }
    }

    void TearDown() override {
        // Hash tablosunu temizle
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != nullptr) {
                User* toDelete = current;
                current = current->next;

                // Belle�i yaln�zca bir kez serbest b�rak�n ve i�aret�iyi NULL yap�n
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
    ASSERT_NE(hashTable[index], nullptr);  // Hash tablosunda bir kullan�c� olmal�
    ASSERT_EQ(hashTable[index], user);    // Eklenen kullan�c� do�ru olmal�
    ASSERT_STREQ(hashTable[index]->name, "John");
    ASSERT_STREQ(hashTable[index]->surname, "Doe");
    ASSERT_STREQ(hashTable[index]->phone, "1234567890");
    ASSERT_STREQ(hashTable[index]->password, "password123");
}

TEST_F(SaveUserTest, SaveUserHandlesCollisions) {
    // Ayn� hash de�erini �retecek telefon numaralar�n� test edin
    User* user1 = createUser("Alice", "Smith", "123", "password1");
    User* user2 = createUser("Bob", "Brown", "123", "password2"); // Ayn� hash de�erine zorla

    saveUser(user1);
    saveUser(user2);

    unsigned int index = hash(user1->phone);
    ASSERT_NE(hashTable[index], nullptr);

    // user2 en ba�ta olmal� (son eklenen)
    ASSERT_EQ(hashTable[index], user2);
    ASSERT_EQ(hashTable[index]->next, user1);
}

TEST_F(SaveUserTest, SaveUserDoesNotLosePreviousEntries) {
    User* user1 = createUser("Charlie", "White", "1234567890", "pass1");
    User* user2 = createUser("Diana", "Green", "1234567890", "pass2");

    saveUser(user1);
    saveUser(user2);

    unsigned int index = hash(user1->phone);
    ASSERT_NE(hashTable[index], nullptr);

    // user2 en ba�ta, user1 onun arkas�nda olmal�
    ASSERT_EQ(hashTable[index], user2);
    ASSERT_EQ(hashTable[index]->next, user1);
    ASSERT_EQ(hashTable[index]->next->next, nullptr);
}

class SaveHashTableToFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Hash tablosunu s�f�rla
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashTable[i] = nullptr;
        }
    }

    void TearDown() override {
        // Belle�i temizle
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != nullptr) {
                User* toDelete = current;
                current = current->next;
                free(toDelete);
            }
        }
    }

    // Kullan�c� olu�turma fonksiyonu
    User* createUser(const char* name, const char* surname, const char* phone, const char* password) {
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->name, name);
        strcpy(user->surname, surname);
        strcpy(user->phone, phone);
        strcpy(user->password, password);
        user->next = nullptr;
        return user;
    }

    // Dosyadan kullan�c�y� okuma fonksiyonu
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

TEST_F(SaveHashTableToFileTest, SaveHashTableToFileCorrectlySavesUsers) {
    // Kullan�c�lar� olu�tur ve hash tablosuna kaydet
    User* user1 = createUser("Alice", "Smith", "123456", "password1");
    User* user2 = createUser("Bob", "Brown", "654321", "password2");

    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    // Hash tablosuna ekle (user2 �nce ekleniyor, sonra user1)
    user2->next = hashTable[index2]; // user2'nin next'ini hashTable[index2]'ye ba�la
    hashTable[index2] = user2;       // user2, hashTable[index2]'de ilk s�rada
    user1->next = hashTable[index1]; // user1'in next'ini hashTable[index1] 'e ba�la
    hashTable[index1] = user1;       // user1, hashTable[index1]de ilk s�rada

    // Kullan�c�lar� dosyaya kaydet
    saveHashTableToFile();

    // Belle�i temizle
    TearDown(); // Hash tablosunu temizle

    // Dosyay� kontrol et
    User* users[2];
    for (int i = 0; i < 2; i++) {
        users[i] = (User*)malloc(sizeof(User));
    }

    // Dosyadan kullan�c�lar� oku
    int userCount = readUsersFromFile("users.bin", users, 2);

    // Dosyadan okunan kullan�c�lar�n do�rulu�unu kontrol et
    ASSERT_EQ(userCount, 2);  // �ki kullan�c� kaydedildi�i i�in say�s� 2 olmal�

    // Hash s�ras�na g�re kullan�c�lar�n do�ru s�rada oldu�unu kontrol et
    ASSERT_STREQ(users[0]->name, "Bob");
    ASSERT_STREQ(users[0]->surname, "Brown");
    ASSERT_STREQ(users[0]->phone, "654321");
    ASSERT_STREQ(users[0]->password, "password2");

    ASSERT_STREQ(users[1]->name, "Alice");
    ASSERT_STREQ(users[1]->surname, "Smith");
    ASSERT_STREQ(users[1]->phone, "123456");
    ASSERT_STREQ(users[1]->password, "password1");

    // Belle�i temizle
    for (int i = 0; i < 2; i++) {
        free(users[i]);
    }
}


TEST_F(SaveHashTableToFileTest, SaveHashTableToFileHandlesEmptyHashTable) {
    // Bo� hash tablosunu kaydet
    saveHashTableToFile();

    // Dosyay� kontrol et
    User* users[2];
    for (int i = 0; i < 2; i++) {
        users[i] = (User*)malloc(sizeof(User));
    }

    // Dosyadan kullan�c�lar� oku
    int userCount = readUsersFromFile("users.bin", users, 2);

    // Dosyada kullan�c� olmamal�
    ASSERT_EQ(userCount, 0);

    // Belle�i temizle
    for (int i = 0; i < 2; i++) {
        free(users[i]);
    }
}



class LoadHashTableFromFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Hash tablosunu s�f�rla
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
                    free(toDelete);  // Ge�erli bir i�aret�i oldu�unda serbest b�rak
                    toDelete = NULL;
                }
            }
        }
    }


    // Kullan�c� olu�turma fonksiyonu
    User* createUser(const char* name, const char* surname, const char* phone, const char* password) {
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->name, name);
        strcpy(user->surname, surname);
        strcpy(user->phone, phone);
        strcpy(user->password, password);
        user->next = NULL;
        return user;
    }

    // Dosyadan kullan�c�y� okuma fonksiyonu
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

TEST_F(LoadHashTableFromFileTest, LoadHashTableFromFileCorrectlyLoadsUsers) {
    // Kullan�c�lar� dosyaya kaydedelim
    User* user1 = createUser("Alice", "Smith", "123456", "password1");
    User* user2 = createUser("Bob", "Brown", "654321", "password2");

    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    // Hash tablosuna kullan�c�lar� ekleyelim
    user2->next = hashTable[index2]; // user2'nin next'ini hashTable[index2]'ye ba�la
    hashTable[index2] = user2;       // user2, hashTable[index2]'de ilk s�rada
    user1->next = hashTable[index1]; // user1'in next'ini hashTable[index1] 'e ba�la
    hashTable[index1] = user1;       // user1, hashTable[index1]de ilk s�rada

    // Kullan�c�lar� dosyaya kaydet
    saveHashTableToFile();

    // Hash tablosunu temizle
    TearDown();

    // Dosyadan kullan�c�lar� y�kleyelim
    loadHashTableFromFile();

    // Dosyadan okunan kullan�c�lar�n do�rulu�unu kontrol et
    User* users[2];
    for (int i = 0; i < 2; i++) {
        users[i] = (User*)malloc(sizeof(User));
    }

    int userCount = readUsersFromFile("users.bin", users, 2);

    ASSERT_EQ(userCount, 2);  // Dosyadan iki kullan�c� okundu�una emin olal�m

    // Kullan�c�lar� kontrol edelim
    ASSERT_STREQ(users[0]->name, "Bob");
    ASSERT_STREQ(users[0]->surname, "Brown");
    ASSERT_STREQ(users[0]->phone, "654321");
    ASSERT_STREQ(users[0]->password, "password2");

    ASSERT_STREQ(users[1]->name, "Alice");
    ASSERT_STREQ(users[1]->surname, "Smith");
    ASSERT_STREQ(users[1]->phone, "123456");
    ASSERT_STREQ(users[1]->password, "password1");

    // Belle�i temizle
    for (int i = 0; i < 2; i++) {
        free(users[i]);
    }
}

TEST_F(LoadHashTableFromFileTest, LoadHashTableFromFileHandlesEmptyFile) {
    // Hash tablosunu s�f�rlayal�m
    TearDown(); // �ncelikle hash tablosunu temizliyoruz

    // Dosyay� sim�le etmek i�in bo� bir dosya olu�turaca��z
    FILE* file = fopen("users.bin", "wb");
    fclose(file);  // Dosyay� bo� b�rak�yoruz

    // Dosyadan kullan�c�lar� y�kleyelim
    loadHashTableFromFile();

    // Hash tablosunda kullan�c� olmamal�
    bool foundUser = false;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL) {
            foundUser = true;
            break;
        }
    }

    ASSERT_FALSE(foundUser);  // Hi� kullan�c� olmamal�
}

TEST_F(LoadHashTableFromFileTest, LoadHashTableFromFileHandlesFileError) {
    // Bu testte dosya a��l�rken hata olu�mas�n� sim�le edelim
    FILE* file = fopen("non_existent_file.bin", "rb");

    // Dosya do�ru bir �ekilde a��lmad�ysa, i�aret�i nullptr olmal�
    ASSERT_EQ(file, nullptr);  // Dosya a��lamazsa nullptr d�ner.

    if (file != nullptr) {
        fclose(file);  // Dosya a��ld�ysa kapatal�m
    }

    // Hash tablosunda kullan�c� olmamal�
    bool foundUser = false;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL) {
            foundUser = true;
            break;
        }
    }

    ASSERT_FALSE(foundUser);  // Hi� kullan�c� bulunmamal�
}


class QuadraticProbingInsertTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Hash tablosunu s�f�rla
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashTable[i] = NULL;
        }
    }

    void TearDown() override {
        // Belle�i temizle
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != NULL) {
                User* toDelete = current;
                current = current->next;
                free(toDelete);
            }
        }
    }

    // Kullan�c� olu�turma fonksiyonu
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

    // Kullan�c�y� ekleyelim
    ASSERT_TRUE(quadraticProbingInsert(user1));

    // Kullan�c�n�n do�ru �ekilde eklendi�ini kontrol edelim
    unsigned int index = hash(user1->phone);
    ASSERT_NE(hashTable[index], nullptr);  // Kullan�c�, hash tablosunda olmal�
    ASSERT_STREQ(hashTable[index]->name, "Alice");
}

TEST_F(QuadraticProbingInsertTest, HandlesCollisionWithQuadraticProbing) {
    // Ayn� hash indeksine d��ecek 2 kullan�c� ekleyelim
    User* user1 = createUser("Alice", "Smith", "1234567890", "password1");
    User* user2 = createUser("Bob", "Brown", "1234567891", "password2");

    // Kullan�c�lar� ekleyelim
    ASSERT_TRUE(quadraticProbingInsert(user1));
    ASSERT_TRUE(quadraticProbingInsert(user2));

    // �kinci kullan�c� farkl� bir indekse yerle�mi� olmal� (�ak��ma nedeniyle)
    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    // �kinci kullan�c�n�n yeni bir konumda olmas� gerekti�ini kontrol edelim
    ASSERT_NE(index1, index2);  // Farkl� indeksler
    ASSERT_NE(hashTable[index1], nullptr);  // �lk kullan�c� yerle�meli
    ASSERT_NE(hashTable[index2], nullptr);  // �kinci kullan�c� yerle�meli
}

TEST_F(QuadraticProbingInsertTest, FailsWhenHashTableIsFull) {
    // Hash tablosunu tamamen doldural�m
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* user = createUser("User", "Test", "1234567890", "password");
        ASSERT_TRUE(quadraticProbingInsert(user));  // Kullan�c� ba�ar�yla eklenmeli
    }

    // Tablo dolduktan sonra kullan�c� eklemeye �al��al�m
    User* extraUser = createUser("Extra", "User", "9876543210", "password123");
    ASSERT_FALSE(quadraticProbingInsert(extraUser));  // Hash tablosu dolmu�, ekleme ba�ar�s�z olmal�
}

TEST_F(QuadraticProbingInsertTest, HandlesMultipleCollisions) {
    // �ak��ma olu�turacak 3 kullan�c� ekleyelim
    User* user1 = createUser("User1", "One", "1234567890", "password1");
    User* user2 = createUser("User2", "Two", "1234567891", "password2");
    User* user3 = createUser("User3", "Three", "1234567892", "password3");

    // Kullan�c�lar� ekleyelim
    ASSERT_TRUE(quadraticProbingInsert(user1));
    ASSERT_TRUE(quadraticProbingInsert(user2));
    ASSERT_TRUE(quadraticProbingInsert(user3));

    // �ak��malara ra�men kullan�c�lar�n do�ru yerlere yerle�ti�ini kontrol edelim
    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);
    unsigned int index3 = hash(user3->phone);

    // �ak��ma nedeniyle farkl� indeksler olacak
    ASSERT_NE(index1, index2);
    ASSERT_NE(index2, index3);
    ASSERT_NE(index1, index3);

    // Kullan�c�lar�n hash tablosuna ba�ar�yla yerle�ti�ini kontrol edelim
    ASSERT_NE(hashTable[index1], nullptr);
    ASSERT_NE(hashTable[index2], nullptr);
    ASSERT_NE(hashTable[index3], nullptr);
}

class HuffmanTreeTest : public ::testing::Test {
protected:
    AttendeE attendee;

    void SetUp() override {
        // AttendeE nesnesini ba�lang�� i�in temizleyin
        memset(&attendee, 0, sizeof(AttendeE));
    }

    void TearDown() override {
        // Test sonras� yap�lacak temizleme i�lemleri
    }
};

TEST_F(HuffmanTreeTest, BuildHuffmanTreeTest) {
    // Test i�in �rnek bir karakter dizisi
    char inputStr[] = "hello huffman";

    // buildHuffmanTree fonksiyonunu �a��r
    buildHuffmanTree(inputStr, &attendee);

    // Beklenen Huffman kodunun do�rulu�unu kontrol et
    ASSERT_NE(attendee.huffmanCode, nullptr) << "Huffman code should be generated.";
    EXPECT_STRNE("", attendee.huffmanCode) << "Huffman code should not be an empty string.";

    // �rnek kontrol: Kodun belirli bir karakter i�in do�ru olmas�
    // Bu �rnekte 'h' karakterinin Huffman kodunu kontrol edebiliriz.
    bool hFound = false;
    for (int i = 0; i < strlen(attendee.huffmanCode); i++) {
        if (attendee.huffmanCode[i] == 'h') {
            hFound = true;
            break;
        }
    }
    EXPECT_TRUE(hFound) << "'h' should have a corresponding Huffman code.";
}
// Test fixture class
class HuffmanTest : public ::testing::Test {
protected:
    // Helper function to create a simple Huffman tree for testing
    MinHeapNode* createSimpleHuffmanTree() {
        // Create nodes for a simple Huffman tree
        MinHeapNode* root = createMinHeapNode('$', 0);
        MinHeapNode* left = createMinHeapNode('a', 5);
        MinHeapNode* right = createMinHeapNode('b', 7);

        root->left = left;
        root->right = right;

        return root;
    }
};

// Test for generateHuffmanCodes function
TEST_F(HuffmanTest, GenerateHuffmanCodesTest) {
    // Create a simple Huffman tree
    MinHeapNode* root = createSimpleHuffmanTree();

    char code[256];
    char huffmanCode[1024] = ""; // Buffer to hold the complete Huffman code string

    // Generate Huffman codes
    generateHuffmanCodes(root, code, 0, huffmanCode);

    // Check if the generated Huffman codes are correct
    EXPECT_STRNE("", huffmanCode) << "Huffman code should not be empty.";

    // Verify the output structure (this example checks for specific codes)
    EXPECT_TRUE(strstr(huffmanCode, "a: 0\n") != nullptr) << "Huffman code for 'a' is incorrect.";
    EXPECT_TRUE(strstr(huffmanCode, "b: 1\n") != nullptr) << "Huffman code for 'b' is incorrect.";

    // Clean up memory
    free(root->left);
    free(root->right);
    free(root);
}
// Test fixture class for User authentication tests
class UserAuthTest : public ::testing::Test {
protected:
    // Test setup: create a sample user and add it to the hash table
    void SetUp() override {
        // Initialize a sample user
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->phone, "1234567890");
        strcpy(user->password, "password123");
        strcpy(user->name, "John");
        strcpy(user->surname, "Doe");
        user->next = nullptr;

        // Hash table setup
        unsigned int index = hash(user->phone);
        hashTable[index] = user;
    }

    // Test teardown: free memory and clean up
    void TearDown() override {
        for (int i = 0; i < TABLE_SIZE; i++) {
            User* current = hashTable[i];
            while (current != nullptr) {
                User* next = current->next;
                free(current);
                current = next;
            }
            // Bu sat�rda hashTable[i]'yi nullptr yaparak bellek kontrol�n� art�rabilirsiniz
            hashTable[i] = nullptr;
        }
    }
};

TEST_F(UserAuthTest, ValidateLoginCorrectPassword) {
    // HashTable'� ba�lat ve kullan�c� ekle
    User user;
    strcpy(user.phone, "123456789");
    strcpy(user.password, "password");
    unsigned int index = hash(user.phone);
    hashTable[index] = &user; // Kullan�c�y� hash table'a ekle

    // Do�ru giri� testi
    ASSERT_TRUE(validateLogin("123456789", "password")) << "Login should succeed with correct credentials.";
}

TEST_F(UserAuthTest, ValidateLoginIncorrectPassword) {
    // HashTable'� ba�lat ve kullan�c� ekle
    User user;
    strcpy(user.phone, "123456789");
    strcpy(user.password, "password");
    unsigned int index = hash(user.phone);
    hashTable[index] = &user; // Kullan�c�y� hash table'a ekle

    // Yanl�� parola testi
    ASSERT_FALSE(validateLogin("123456789", "wrongpassword")) << "Login should fail with incorrect password.";
}

TEST_F(UserAuthTest, ValidateLoginNonExistentUser) {
    // Var olmayan kullan�c� testi
    ASSERT_FALSE(validateLogin("000000000", "password")) << "Login should fail for non-existent users.";
}

TEST_F(UserAuthTest, ValidateLoginNullInput) {
    // Null giri� testi
    ASSERT_FALSE(validateLogin(NULL, "password")) << "Login should fail with NULL phone.";
    ASSERT_FALSE(validateLogin("123456789", NULL)) << "Login should fail with NULL password.";
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
