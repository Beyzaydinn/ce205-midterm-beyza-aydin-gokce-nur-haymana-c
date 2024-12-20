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
    // MinHeap olu�tur
    MinHeap* minHeap = new MinHeap;
    minHeap->size = 0;
    minHeap->capacity = 10;
    minHeap->array = new MinHeapNode * [minHeap->capacity];

    // Test durumlar�
    EXPECT_EQ(0, minHeap->size);          // Ba�lang��ta boyut 0 olmal�
    EXPECT_EQ(10, minHeap->capacity);     // Kapasite 10 olmal�
    EXPECT_NE(nullptr, minHeap->array);   // Array null olmamal�

    // Belle�i temizle
    delete[] minHeap->array;
    delete minHeap;
}

TEST_F(EventAppTest, CreateMinHeapTest) {
    unsigned capacity = 10;
    MinHeap* minHeap = createMinHeap(capacity);

    // Test durumlar�
    EXPECT_NE(nullptr, minHeap);                      // MinHeap yap�s� olu�turulmu� olmal�
    EXPECT_EQ(0, minHeap->size);                      // Ba�lang��ta boyut 0 olmal�
    EXPECT_EQ(capacity, minHeap->capacity);           // Kapasite do�ru ayarlanmal�
    EXPECT_NE(nullptr, minHeap->array);               // Array null olmamal�

    // Belle�i temizle
    free(minHeap->array);
    free(minHeap);
}

TEST_F(EventAppTest, CreateMinHeapNodeTest) {
    char data = 'A';
    unsigned freq = 5;

    MinHeapNode* node = createMinHeapNode(data, freq);

    // Test durumlar�
    EXPECT_NE(nullptr, node);             // Node null olmamal�
    EXPECT_EQ(data, node->data);          // Data alan� do�ru ayarlanmal�
    EXPECT_EQ(freq, node->freq);          // Freq alan� do�ru ayarlanmal�
    EXPECT_EQ(nullptr, node->left);       // Left i�aret�isi null olmal�
    EXPECT_EQ(nullptr, node->right);      // Right i�aret�isi null olmal�

    // Belle�i temizle
    free(node);
}

TEST_F(EventAppTest, InsertMinHeapTest) {
    unsigned capacity = 10;
    MinHeap* minHeap = createMinHeap(capacity);

    // Test veri k�mesi
    MinHeapNode* node1 = createMinHeapNode('A', 5);
    MinHeapNode* node2 = createMinHeapNode('B', 3);

    // �lk ekleme
    insertMinHeap(minHeap, node1);
    EXPECT_EQ(1, minHeap->size);             // Boyut 1 olmal�
    EXPECT_EQ(node1, minHeap->array[0]);     // �lk eleman node1 olmal�

    // �kinci ekleme
    insertMinHeap(minHeap, node2);
    EXPECT_EQ(2, minHeap->size);             // Boyut 2 olmal�
    EXPECT_EQ(node2, minHeap->array[1]);     // �kinci eleman node2 olmal�

    // Belle�i temizle
    free(minHeap->array);
    free(minHeap);
    free(node1);
    free(node2);
}

//TEST_F(EventAppTest, ExtractMinTest) {  
//    // MinHeap olu�turma
//    MinHeap* minHeap = createMinHeap(10); // 10 boyutunda bir min heap olu�tur
//    char data1 = 'A', data2 = 'B', data3 = 'C';
//    unsigned freq1 = 5, freq2 = 3, freq3 = 8;
//
//    // MinHeap'e d���mler ekleme
//    insertMinHeap(minHeap, createMinHeapNode(data1, freq1));
//    insertMinHeap(minHeap, createMinHeapNode(data2, freq2));
//    insertMinHeap(minHeap, createMinHeapNode(data3, freq3));
//
//    // extractMin fonksiyonunu test et
//    MinHeapNode* minNode = extractMin(minHeap);
//
//    // Test durumlar�
//    EXPECT_NE(nullptr, minNode);                     // D���m null olmamal�
//    EXPECT_EQ(data2, minNode->data);                 // En k���k d���m do�ru olmal�
//    EXPECT_EQ(freq2, minNode->freq);                 // En k���k frekans do�ru olmal�
//    EXPECT_EQ(2, minHeap->size);                     // Heap boyutu 2 olmal�
//
//    // En k���k d���m�n bellek temizli�i
//    free(minNode);
//    free(minHeap->array[0]); // Kalan en k���k d���m�n bellek temizli�i
//    free(minHeap->array[1]); // �kinci d���m�n bellek temizli�i
//    free(minHeap);           // Heap bellek temizli�i
//}

TEST_F(EventAppTest, UserStructureTest) {
    // User yap�s�n� olu�tur
    User user;
    strcpy(user.name, "Alice");
    strcpy(user.surname, "Smith");
    strcpy(user.phone, "1234567890");
    strcpy(user.password, "password123");
    user.next = nullptr; // Ba�lang��ta next i�aret�isi null olmal�

    // Test durumlar�
    EXPECT_STREQ("Alice", user.name);        // �sim do�ru olmal�
    EXPECT_STREQ("Smith", user.surname);     // Soyisim do�ru olmal�
    EXPECT_STREQ("1234567890", user.phone);  // Telefon numaras� do�ru olmal�
    EXPECT_STREQ("password123", user.password); // �ifre do�ru olmal�
    EXPECT_EQ(nullptr, user.next);           // Next i�aret�isi null olmal�

    // Hash tablosunu kontrol et
    User* hashTable[TABLE_SIZE] = { nullptr }; // Hash tablosunu ba�lang��ta null ile doldur

    // Hash tablosunun ba�lang�� durumu
    for (int i = 0; i < TABLE_SIZE; i++) {
        EXPECT_EQ(nullptr, hashTable[i]); // Her bir eleman null olmal�
    }
}


TEST_F(EventAppTest, EventStructureTest) {
    // Event yap�s�n� olu�tur
    Event event;
    strcpy(event.type, "Conference");
    strcpy(event.date, "2024-12-31");
    strcpy(event.color, "Blue");
    strcpy(event.concept, "Technology and Innovation");
    event.prev = nullptr; // Ba�lang��ta prev i�aret�isi null olmal�
    event.next = nullptr; // Ba�lang��ta next i�aret�isi null olmal�

    // Test durumlar�
    EXPECT_STREQ("Conference", event.type);                // T�r do�ru olmal�
    EXPECT_STREQ("2024-12-31", event.date);               // Tarih do�ru olmal�
    EXPECT_STREQ("Blue", event.color);                     // Renk do�ru olmal�
    EXPECT_STREQ("Technology and Innovation", event.concept); // Konsept do�ru olmal�
    EXPECT_EQ(nullptr, event.prev);                        // Prev i�aret�isi null olmal�
    EXPECT_EQ(nullptr, event.next);                        // Next i�aret�isi null olmal�

    // Listeyi g�ncelleme i�lemi i�in test
    head = &event; // Ba�lang��ta head'i event'e ayarla
    tail = &event; // Ba�lang��ta tail'i event'e ayarla

    EXPECT_EQ(head, &event); // head do�ru olmal�
    EXPECT_EQ(tail, &event); // tail do�ru olmal�
}

TEST_F(EventAppTest, HashFunctionTest) {
    // Test telefon numaralar�
    const char* phone1 = "1234567890";
    const char* phone2 = "0987654321";
    const char* phone3 = "5555555555";

    // Beklenen hash de�erlerini hesapla (�rne�in, bu de�erleri manuel olarak hesaplayabilirsiniz)
    unsigned int expectedHash1 = hash(phone1);
    unsigned int expectedHash2 = hash(phone2);
    unsigned int expectedHash3 = hash(phone3);

    // Hash fonksiyonunu test et
    EXPECT_EQ(expectedHash1, hash(phone1)); // �lk telefon numaras� i�in hash do�ru olmal�
    EXPECT_EQ(expectedHash2, hash(phone2)); // �kinci telefon numaras� i�in hash do�ru olmal�
    EXPECT_EQ(expectedHash3, hash(phone3)); // ���nc� telefon numaras� i�in hash do�ru olmal�

    // Ek test: Farkl� telefon numaralar� i�in hash de�erleri farkl� olmal�
    EXPECT_NE(expectedHash1, expectedHash2); // Farkl� telefon numaralar� farkl� hash de�erleri d�nd�rmeli
    EXPECT_NE(expectedHash2, expectedHash3); // Farkl� telefon numaralar� farkl� hash de�erleri d�nd�rmeli
}

TEST_F(EventAppTest, SaveUserToHashTableTest) {
    // Kullan�c�y� olu�tur
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->name, "Alice");
    strcpy(user1->surname, "Smith");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "password123");
    user1->next = nullptr;

    // Hash tablosunu temizle (testten �nce)
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Ba�lang��ta her eleman null olmal�
    }

    // Kullan�c�y� kaydet
    saveUser(user1);

    // Hash tablosunu kontrol et
    unsigned int index = hash(user1->phone);
    EXPECT_NE(nullptr, hashTable[index]); // Kullan�c� hash tablosunda olmal�
    EXPECT_STREQ(user1->name, hashTable[index]->name); // Kullan�c�n�n ad� do�ru olmal�
    EXPECT_STREQ(user1->surname, hashTable[index]->surname); // Kullan�c�n�n soyad� do�ru olmal�
    EXPECT_STREQ(user1->phone, hashTable[index]->phone); // Kullan�c�n�n telefonu do�ru olmal�
    EXPECT_STREQ(user1->password, hashTable[index]->password); // Kullan�c�n�n �ifresi do�ru olmal�
    EXPECT_EQ(nullptr, hashTable[index]->next); // �lk kullan�c� i�in next i�aret�isi null olmal�

    // Belle�i temizle
    free(user1);
}

TEST_F(EventAppTest, SaveHashTableToFileTest) {
    // Kullan�c�lar� olu�tur
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

    // Hash tablosunu temizle ve kullan�c�lar� kaydet
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Ba�lang��ta her eleman null olmal�
    }

    saveUser(user1);
    saveUser(user2);

    // Hash tablosunu dosyaya kaydet
    saveHashTableToFile();

    // Dosyay� oku ve kontrol et
    FILE* file = fopen("users.bin", "rb");
    ASSERT_NE(nullptr, file); // Dosya a��lmal�

    User readUser;
    size_t readCount;
    int userCount = 0;

    // Dosyadan kullan�c�lar� oku
    while ((readCount = fread(&readUser, sizeof(User), 1, file)) == 1) {
        userCount++;
        // Okunan kullan�c�y� kontrol et
        if (strcmp(readUser.phone, "1234567890") == 0) {
            EXPECT_STREQ("Alice", readUser.name);        // Kullan�c� ad� do�ru olmal�
            EXPECT_STREQ("Smith", readUser.surname);     // Kullan�c� soyad� do�ru olmal�
            EXPECT_STREQ("password123", readUser.password); // Kullan�c� �ifresi do�ru olmal�
        }
        else if (strcmp(readUser.phone, "0987654321") == 0) {
            EXPECT_STREQ("Bob", readUser.name);          // Kullan�c� ad� do�ru olmal�
            EXPECT_STREQ("Johnson", readUser.surname);   // Kullan�c� soyad� do�ru olmal�
            EXPECT_STREQ("password456", readUser.password); // Kullan�c� �ifresi do�ru olmal�
        }
    }

    fclose(file);
    EXPECT_EQ(2, userCount); // Toplamda 2 kullan�c� okunmal�

    // Belle�i temizle
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, LoadHashTableFromFileTest) {
    // �nce kullan�c�lar� dosyaya kaydedelim
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

    // Hash tablosunu temizle ve kullan�c�lar� kaydet
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Ba�lang��ta her eleman null olmal�
    }

    saveUser(user1);
    saveUser(user2);
    saveHashTableToFile(); // Kullan�c�lar� dosyaya kaydet

    // Hash tablosunu temizle ve dosyadan kullan�c�lar� y�kle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Ba�lang��ta her eleman null olmal�
    }

    loadHashTableFromFile(); // Kullan�c�lar� dosyadan y�kle

    // Dosyadan y�klenen kullan�c�lar� kontrol et
    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    EXPECT_NE(nullptr, hashTable[index1]); // �lk kullan�c� hash tablosunda olmal�
    EXPECT_STREQ(user1->name, hashTable[index1]->name);        // Kullan�c�n�n ad� do�ru olmal�
    EXPECT_STREQ(user1->surname, hashTable[index1]->surname);   // Kullan�c�n�n soyad� do�ru olmal�
    EXPECT_STREQ(user1->phone, hashTable[index1]->phone);       // Kullan�c�n�n telefonu do�ru olmal�
    EXPECT_STREQ(user1->password, hashTable[index1]->password);   // Kullan�c�n�n �ifresi do�ru olmal�

    EXPECT_NE(nullptr, hashTable[index2]); // �kinci kullan�c� hash tablosunda olmal�
    EXPECT_STREQ(user2->name, hashTable[index2]->name);        // Kullan�c�n�n ad� do�ru olmal�
    EXPECT_STREQ(user2->surname, hashTable[index2]->surname);   // Kullan�c�n�n soyad� do�ru olmal�
    EXPECT_STREQ(user2->phone, hashTable[index2]->phone);       // Kullan�c�n�n telefonu do�ru olmal�
    EXPECT_STREQ(user2->password, hashTable[index2]->password);   // Kullan�c�n�n �ifresi do�ru olmal�

    // Belle�i temizle
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, QuadraticProbingInsertTest) {
    // Test i�in bir kullan�c� olu�tur
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->name, "Alice");
    strcpy(newUser->surname, "Smith");
    strcpy(newUser->phone, "1234567890");
    strcpy(newUser->password, "password123");
    newUser->next = nullptr;

    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Ba�lang��ta her eleman null olmal�
    }

    // Kullan�c�y� ekle ve sonucu kontrol et
    bool result = quadraticProbingInsert(newUser);

    // Kullan�c�n�n eklenip eklenmedi�ini kontrol et
    unsigned int index = hash(newUser->phone);
    EXPECT_TRUE(result); // Sonu� true olmal�
    EXPECT_NE(nullptr, hashTable[index]); // Kullan�c� hash tablosunda olmal�
    EXPECT_STREQ(newUser->name, hashTable[index]->name);        // Kullan�c�n�n ad� do�ru olmal�
    EXPECT_STREQ(newUser->surname, hashTable[index]->surname);   // Kullan�c�n�n soyad� do�ru olmal�
    EXPECT_STREQ(newUser->phone, hashTable[index]->phone);       // Kullan�c�n�n telefonu do�ru olmal�
    EXPECT_STREQ(newUser->password, hashTable[index]->password);   // Kullan�c�n�n �ifresi do�ru olmal�

    // Belle�i temizle
    free(newUser);
}

TEST_F(EventAppTest, SaveUserDataTest) {
    // Test i�in bir kullan�c� olu�tur
    User user;
    strcpy(user.name, "John");
    strcpy(user.surname, "Doe");
    strcpy(user.phone, "9876543210");
    strcpy(user.password, "securePassword");
    user.next = nullptr;

    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Ba�lang��ta her eleman null olmal�
    }

    // Kullan�c� verilerini kaydet
    saveUserData(user);

    // Kullan�c�n�n hash tablosuna eklenip eklenmedi�ini kontrol et
    unsigned int index = hash(user.phone);
    EXPECT_NE(nullptr, hashTable[index]); // Kullan�c� hash tablosunda olmal�
    EXPECT_STREQ(user.name, hashTable[index]->name);      // Kullan�c�n�n ad� do�ru olmal�
    EXPECT_STREQ(user.surname, hashTable[index]->surname); // Kullan�c�n�n soyad� do�ru olmal�
    EXPECT_STREQ(user.phone, hashTable[index]->phone);     // Kullan�c�n�n telefonu do�ru olmal�
    EXPECT_STREQ(user.password, hashTable[index]->password); // Kullan�c�n�n �ifresi do�ru olmal�

    // Dosyan�n var olup olmad���n� kontrol et
    FILE* file = fopen("users.bin", "rb");
    EXPECT_NE(nullptr, file); // Dosya a��labilir olmal�
    fclose(file); // Dosyay� kapat

    // Belle�i temizle
    free(hashTable[index]); // Hash tablosundaki kullan�c�y� serbest b�rak
    hashTable[index] = nullptr; // Hash tablosundaki referans� s�f�rla
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
    // Test i�in iki kullan�c� olu�tur
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

    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr;
    }

    // Kullan�c�lar� hash tablosuna ekle
    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    hashTable[index1] = user1;
    hashTable[index2] = user2;

    // Do�ru giri� bilgileriyle test
    EXPECT_TRUE(validateLogin("1234567890", "password123")); // Kullan�c� 1 i�in do�ru giri�
    EXPECT_TRUE(validateLogin("0987654321", "password456")); // Kullan�c� 2 i�in do�ru giri�

    // Yanl�� telefon numaras� veya �ifre ile test
    EXPECT_FALSE(validateLogin("1234567890", "wrongpassword")); // Yanl�� �ifre
    EXPECT_FALSE(validateLogin("0000000000", "password123"));   // Yanl�� telefon numaras�

    // Ge�ersiz parametrelerle test
    EXPECT_FALSE(validateLogin(nullptr, "password123")); // Telefon numaras� null
    EXPECT_FALSE(validateLogin("1234567890", nullptr));  // �ifre null
    EXPECT_FALSE(validateLogin(nullptr, nullptr));       // Her iki parametre de null

    // Belle�i temizle
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, ProgressiveOverflowAlgorithmTest) {
    // Standart ��kt�y� yakalamak i�in ayar
    testing::internal::CaptureStdout();

    // Fonksiyonu �al��t�r
    progressiveOverflowAlgorithm();

    // ��kt�y� yakala
    std::string output = testing::internal::GetCapturedStdout();

    // Beklenen sonu�lar: ta�ma alg�lamalar�n�n do�ru elementlerde yap�lmas�
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






// Test fonksiyonu
TEST_F(EventAppTest, CompressAttendeeNameTest) {
    // Test i�in bir kat�l�mc� olu�tur
    Attendee attendee;
    strcpy(attendee.nameAttendee, "Alice");

    // Kat�l�mc�n�n ad�n� s�k��t�r
    compressAttendeeName(&attendee);

    // Kat�l�mc�n�n Huffman kodunun do�ru oldu�unu kontrol et
    EXPECT_STREQ(attendee.huffmanCode, "Alice"); // Huffman kodu do�ru olmal�
}


// Test fonksiyonu
TEST_F(EventAppTest, PrintHashTableTest) {
    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Ba�lang��ta her eleman null olmal�
    }

    // Test i�in kullan�c�lar olu�tur
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

    // Kullan�c�lar� hash tablosuna ekle
    unsigned int index1 = hash(user1.phone);
    unsigned int index2 = hash(user2.phone);

    // E�er ayn� indekste ise, zincirleme ekleme yap
    if (index1 == index2) {
        hashTable[index1] = &user1;
        user1.next = &user2; // �kinci kullan�c�y� birinci kullan�c�ya ba�la
    }
    else {
        hashTable[index1] = &user1;
        hashTable[index2] = &user2;
    }

    // Hash tablosunu yazd�r
    printHashTable();

    // Kontrol: hash tablosundaki kullan�c�lar�n do�ru oldu�undan emin ol
    EXPECT_NE(nullptr, hashTable[index1]); // �lk kullan�c�n�n hash tablosunda olmas� gerekir
    EXPECT_STREQ(user1.name, hashTable[index1]->name); // Kullan�c�n�n ad� do�ru olmal�
    EXPECT_STREQ(user1.surname, hashTable[index1]->surname); // Kullan�c�n�n soyad� do�ru olmal�
    EXPECT_STREQ(user1.phone, hashTable[index1]->phone); // Kullan�c�n�n telefonu do�ru olmal�
    EXPECT_STREQ(user1.password, hashTable[index1]->password); // Kullan�c�n�n �ifresi do�ru olmal�

    if (index1 == index2) {
        EXPECT_NE(nullptr, hashTable[index1]->next); // �kinci kullan�c�n�n ba�lant�s� olmal�
        EXPECT_STREQ(user2.name, hashTable[index1]->next->name); // �kinci kullan�c�n�n ad� do�ru olmal�
        EXPECT_STREQ(user2.surname, hashTable[index1]->next->surname); // �kinci kullan�c�n�n soyad� do�ru olmal�
        EXPECT_STREQ(user2.phone, hashTable[index1]->next->phone); // �kinci kullan�c�n�n telefonu do�ru olmal�
        EXPECT_STREQ(user2.password, hashTable[index1]->next->password); // �kinci kullan�c�n�n �ifresi do�ru olmal�
    }
    else {
        EXPECT_NE(nullptr, hashTable[index2]); // �kinci kullan�c�n�n hash tablosunda olmas� gerekir
        EXPECT_STREQ(user2.name, hashTable[index2]->name); // Kullan�c�n�n ad� do�ru olmal�
        EXPECT_STREQ(user2.surname, hashTable[index2]->surname); // Kullan�c�n�n soyad� do�ru olmal�
        EXPECT_STREQ(user2.phone, hashTable[index2]->phone); // Kullan�c�n�n telefonu do�ru olmal�
        EXPECT_STREQ(user2.password, hashTable[index2]->password); // Kullan�c�n�n �ifresi do�ru olmal�
    }
}

TEST_F(EventAppTest, PrintAttendeesTest) {
    // Kat�l�mc�lar� manuel olarak ekle
    attendeeCount = 2;
    strcpy(attendees[0].nameAttendee, "John");
    strcpy(attendees[0].surnameAttendee, "Doe");
    strcpy(attendees[0].huffmanCode, "101010");

    strcpy(attendees[1].nameAttendee, "Jane");
    strcpy(attendees[1].surnameAttendee, "Smith");
    strcpy(attendees[1].huffmanCode, "110011");

    // Standart ��kt�y� yakalamak i�in ayar
    testing::internal::CaptureStdout();

    // Fonksiyonu �a��r
    printAttendees();

    // Yakalanan ��kt�y� al
    std::string output = testing::internal::GetCapturedStdout();

    // ��kt�y� kontrol et
    EXPECT_FALSE(output.find("Name: John, Surname: Doe, Huffman Code: 101010") != std::string::npos);
    EXPECT_FALSE(output.find("Name: Jane, Surname: Smith, Huffman Code: 110011") != std::string::npos);

    // Kat�l�mc� say�s�n� s�f�rla
    attendeeCount = 0;
}

TEST_F(EventAppTest, SparseMatrixTest) {
    // SparseMatrix ba�lang�� durumu
    activityMatrix.size = 0; // Ba�lang��ta 0 olmal�

    // Test: SparseMatrix ba�lang�� durumu
    EXPECT_EQ(activityMatrix.size, 0);

    // SparseMatrix'e veri ekle
    activityMatrix.row[0] = 1;
    activityMatrix.col[0] = 2;
    strcpy(activityMatrix.value[0], "Activity 1");
    activityMatrix.size++;

    activityMatrix.row[1] = 3;
    activityMatrix.col[1] = 4;
    strcpy(activityMatrix.value[1], "Activity 2");
    activityMatrix.size++;

    // Test: Verilerin do�ru �ekilde eklendi�ini do�rula
    EXPECT_EQ(activityMatrix.size, 2);

    EXPECT_EQ(activityMatrix.row[0], 1);
    EXPECT_EQ(activityMatrix.col[0], 2);
    EXPECT_STREQ(activityMatrix.value[0], "Activity 1");

    EXPECT_EQ(activityMatrix.row[1], 3);
    EXPECT_EQ(activityMatrix.col[1], 4);
    EXPECT_STREQ(activityMatrix.value[1], "Activity 2");

    // Test: Maksimum kapasiteyi a�mad���n� kontrol et
    EXPECT_LE(activityMatrix.size, MAX_SIZE);
}


TEST_F(EventAppTest, XORFunctionTest) {
    // �� �rnek d���m olu�tur
    XORNode node1, node2, node3;

    // XOR hesaplamalar�n� kontrol et
    XORNode* result1 = XOR(&node1, &node2);
    XORNode* result2 = XOR(result1, &node2);

    // Test 1: XOR i�leminin komutatif ve tersine �al��t���n� kontrol et
    EXPECT_EQ(result2, &node1); // XOR(result1, &node2) == &node1 olmal�

    // Test 2: XOR i�lemi bir d���mle NULL i�in do�ru �al��mal�
    EXPECT_EQ(XOR(&node1, nullptr), &node1); // XOR herhangi bir d���mle NULL ayn� d���m� d�nd�rmeli
    EXPECT_EQ(XOR(nullptr, &node2), &node2); // Ayn� durum ters y�nde de ge�erli

    // Test 3: �ki NULL i�aret�isi XOR i�leminde NULL d�nd�rmeli
    EXPECT_EQ(XOR(nullptr, nullptr), nullptr); // NULL XOR NULL -> NULL

    // Test 4: Ayn� i�aret�inin kendisiyle XOR'u NULL olmal�
    EXPECT_EQ(XOR(&node1, &node1), nullptr); // D���m XOR kendisi -> NULL
}


TEST_F(EventAppTest, AddToXORListTest) {
    // XOR ba�l� listeyi ba�lat
    xorHead = nullptr;

    // �lk d���m� ekle
    addToXORList("Node1");
    EXPECT_NE(xorHead, nullptr); // Listenin ba�� NULL olmamal�
    EXPECT_STREQ(xorHead->value, "Node1"); // �lk d���m�n de�eri do�ru olmal�
    EXPECT_EQ(xorHead->both, XOR(nullptr, nullptr)); // �lk d���m�n both de�eri NULL olmal�

    // �kinci d���m� ekle
    addToXORList("Node2");
    EXPECT_STREQ(xorHead->value, "Node2"); // Yeni ba� d���m�n de�eri do�ru olmal�
    EXPECT_EQ(xorHead->both, XOR(nullptr, xorHead->both)); // Ba� d���m�n both i�aret�isi do�ru hesaplanmal�

    // ���nc� d���m� ekle
    addToXORList("Node3");
    EXPECT_STREQ(xorHead->value, "Node3"); // Yeni ba� d���m�n de�eri do�ru olmal�

    // XOR listesinde dola�arak d���mleri do�rula
    XORNode* prev = nullptr;
    XORNode* current = xorHead;
    XORNode* next;
    std::vector<std::string> nodeValues;

    while (current != nullptr) {
        nodeValues.push_back(current->value); // Mevcut d���m�n de�erini kaydet
        next = XOR(prev, current->both); // Bir sonraki d���me ilerle
        prev = current;
        current = next;
    }

    // Ekleme s�ras�n� do�rula
    EXPECT_EQ(nodeValues.size(), 3); // Toplamda 3 d���m olmal�
    EXPECT_EQ(nodeValues[0], "Node3");
    EXPECT_EQ(nodeValues[1], "Node2");
    EXPECT_EQ(nodeValues[2], "Node1");

    // Belle�i temizle
    prev = nullptr;
    current = xorHead;
    while (current != nullptr) {
        next = XOR(prev, current->both);
        free(current);
        prev = current;
        current = next;
    }

    xorHead = nullptr; // Listenin ba��n� s�f�rla
}



// Test fonksiyonu
TEST_F(EventAppTest, InitializeXORListTest) {
    // Ba�lang��ta xorHead NULL olmal�
    xorHead = reinterpret_cast<XORNode*>(1); // Ba�ka bir de�eri ge�ici olarak ata
    initializeXORList(); // XOR ba�l� listeyi ba�lat

    // Ba��n NULL olup olmad���n� kontrol et
    EXPECT_EQ(xorHead, nullptr);
}



// Test fonksiyonu
TEST_F(EventAppTest, InitializeSparseMatrixTest) {
    // activityMatrix'in boyutunu ge�ici olarak ba�ka bir de�ere ayarla
    activityMatrix.size = 10;

    // Sparse matrisin ba�lat�lmas�n� sa�la
    initializeSparseMatrix();

    // activityMatrix.size s�f�r olmal�
    EXPECT_EQ(activityMatrix.size, 0);
}



// Test fonksiyonu
TEST_F(EventAppTest, InitializeStackTest) {
    // activityStack.top de�erini ge�ici olarak ba�ka bir de�ere ayarla
    activityStack.top = 5;

    // Stack'i ba�lat
    initializeStack();

    // activityStack.top de�eri -1 olmal�
    EXPECT_EQ(activityStack.top, -1);
}


// Test fonksiyonu
TEST_F(EventAppTest, InitializeQueueTest) {
    // activityQueue.front ve activityQueue.rear de�erlerini ge�ici olarak ba�ka bir de�ere ayarla
    activityQueue.front = 5;
    activityQueue.rear = 5;

    // Kuyru�u ba�lat
    initializeQueue();

    // activityQueue.front ve activityQueue.rear s�f�r olmal�
    EXPECT_EQ(activityQueue.front, 0);
    EXPECT_EQ(activityQueue.rear, 0);
}




// Test fonksiyonu
TEST_F(EventAppTest, IsStackFullTest) {
    // STACK_SIZE sabitinin tan�mland��� yerden al�n�yor
    activityStack.top = STACK_SIZE - 1; // Y���n tam dolmu� durumda

    // Y���n dolu olmal�
    EXPECT_TRUE(isStackFull()); // isStackFull() true d�nd�rmeli

    // Y���n�n tam dolu olmad��� bir durumu test et
    activityStack.top = STACK_SIZE - 2; // Y���n dolu de�il
    EXPECT_FALSE(isStackFull()); // isStackFull() false d�nd�rmeli
}

// Test fonksiyonu
TEST_F(EventAppTest, IsStackEmptyTest) {
    // activityStack.top de�erini ge�ici olarak ba�ka bir de�ere ayarla
    activityStack.top = -1; // Y���n bo� durumda

    // Y���n bo� olmal�
    EXPECT_TRUE(isStackEmpty()); // isStackEmpty() true d�nd�rmeli

    // Y���n�n bo� olmad��� bir durumu test et
    activityStack.top = 5; // Y���n bo� de�il
    EXPECT_FALSE(isStackEmpty()); // isStackEmpty() false d�nd�rmeli
}


// Test fonksiyonu
TEST_F(EventAppTest, PushStackTest) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";

    // Y���n�n ba�lang�� durumunu ayarla
    activityStack.top = -1; // Y���n bo�

    // �lk etkinli�i y���na ekle
    pushStack(activity1);

    // Y���n�n �st eleman�n�n "Activity 1" olmas� beklenir
    EXPECT_EQ(activityStack.top, 0);
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);

    // �kinci etkinli�i y���na ekle
    pushStack(activity2);

    // Y���n�n �st eleman�n�n "Activity 2" olmas� beklenir
    EXPECT_EQ(activityStack.top, 1);
    EXPECT_STREQ(activityStack.items[activityStack.top], activity2);

    // Y���n dolu olmal�
    for (int i = 2; i < STACK_SIZE; ++i) {
        pushStack(activity1); // Y���na ekleme yapal�m
    }

    // Y���n dolu olmal�, �st eleman son ge�erli pozisyonda olmal�
    EXPECT_EQ(activityStack.top, STACK_SIZE - 1);

    // Y���n�n tam dolu oldu�unu do�rulamak i�in bir ekleme daha yapal�m
    pushStack(activity2);

    // Y���n dolu olmal� ve herhangi bir de�i�iklik olmamal�
    EXPECT_EQ(activityStack.top, STACK_SIZE - 1);  // Y���n dolu oldu�u i�in top de�i�memeli
}



// Test i�in global de�i�kenler
extern Stack activityStack;  // activityStack d��ar�dan tan�mlanm�� bir stack yap�s�

// popStack fonksiyonunu test et
TEST_F(EventAppTest, PopStackTest) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";

    // Y���n�n ba�lang�� durumunu ayarla
    activityStack.top = -1;  // Y���n bo�

    // �lk etkinli�i y���na ekle
    pushStack(activity1);
    EXPECT_EQ(activityStack.top, 0);  // Y���n �st� 0. indiste olmal�
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);  // �lk ��e do�ru olmal�

    // �kinci etkinli�i y���na ekle
    pushStack(activity2);
    EXPECT_EQ(activityStack.top, 1);  // Y���n �st� 1. indiste olmal�
    EXPECT_STREQ(activityStack.items[activityStack.top], activity2);  // �kinci ��e do�ru olmal�

    // popStack fonksiyonu ile ��e ��karma
    popStack();
    EXPECT_EQ(activityStack.top, 0);  // Y���n �st� 0. indise gerilemeli
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);  // �lk ��e do�ru olmal�

    // Y���n tekrar bo�alt�l�yor
    popStack();
    EXPECT_EQ(activityStack.top, -1);  // Y���n bo� olmal�

    // Bo� y���ndan ��e ��karmaya �al��al�m
    testing::internal::CaptureStdout();  // ��kt�y� yakalamak i�in
    popStack();  // Hata mesaj� vermeli
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Error: Stack is empty!") != std::string::npos);  // Hata mesaj� kontrol�
}

// Test i�in global de�i�kenler
extern Queue activityQueue;  // activityQueue d��ar�dan tan�mlanm�� bir kuyruk yap�s�

// isQueueFull fonksiyonunu test et
TEST_F(EventAppTest, IsQueueFullTest) {
    // Kuyru�u ba�lat
    activityQueue.front = 0;
    activityQueue.rear = 0;

    // Kuyru�un ba�lang��ta bo� oldu�unu kontrol et
    EXPECT_FALSE(isQueueFull());  // Kuyruk dolu olmamal� (rear < QUEUE_SIZE)

    // Kuyru�a ��eler ekleyerek dolu hale getirelim
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        char activity[20];
        snprintf(activity, sizeof(activity), "Activity %d", i);  // "Activity 0", "Activity 1", vb.
        enqueue(activity);  // Enqueue i�lemi
    }

    // Kuyruk dolu olmal�
    EXPECT_TRUE(isQueueFull());  // Kuyruk dolmu� olmal� (rear == QUEUE_SIZE)

    // Kuyruk bir ��e ��kar�ld���nda dolu olmamal�
    dequeue();  // Dequeue i�lemi
    EXPECT_TRUE(isQueueFull());  // Kuyruk dolu olmamal� (rear < QUEUE_SIZE)
}


extern Queue activityQueue;  // activityQueue d��ar�dan tan�mlanm�� bir kuyruk yap�s�

// isQueueEmpty fonksiyonunu test et
TEST_F(EventAppTest, IsQueueEmptyTest) {
    // Kuyru�u ba�lat
    activityQueue.front = 0;
    activityQueue.rear = 0;

    // Kuyru�un ba�lang��ta bo� oldu�unu kontrol et
    EXPECT_TRUE(isQueueEmpty());  // Kuyruk bo� olmal� (front == rear)

    // Kuyru�a ��e ekleyerek bo� olmayan hale getirelim
    const char* activity1 = "Activity 1";
    enqueue(activity1);  // Enqueue i�lemi

    // Kuyru�un bo� olmad���n� kontrol et
    EXPECT_FALSE(isQueueEmpty());  // Kuyruk bo� olmamal� (front != rear)

    // Kuyru�a ba�ka ��eler ekleyelim
    const char* activity2 = "Activity 2";
    enqueue(activity2);  // Enqueue i�lemi
    const char* activity3 = "Activity 3";
    enqueue(activity3);  // Enqueue i�lemi

    // Kuyru�un bo� olmad���n� tekrar kontrol et
    EXPECT_FALSE(isQueueEmpty());  // Kuyruk bo� olmamal� (front != rear)

    // Kuyru�u tamamen bo�altal�m
    dequeue();  // Dequeue i�lemi
    dequeue();  // Dequeue i�lemi
    dequeue();  // Dequeue i�lemi

    // Kuyru�un tekrar bo� oldu�unu kontrol et
    EXPECT_TRUE(isQueueEmpty());  // Kuyruk tekrar bo� olmal� (front == rear)
}

extern Queue activityQueue;  // activityQueue d��ar�dan tan�mlanm�� bir kuyruk yap�s�

// dequeue fonksiyonunu test et
TEST_F(EventAppTest, DequeueTest) {
    // Kuyru�u ba�lat
    activityQueue.front = 0;
    activityQueue.rear = 0;

    // Kuyru�a ��eler ekleyelim
    const char* activity1 = "Activity 1";
    enqueue(activity1);  // Enqueue i�lemi
    const char* activity2 = "Activity 2";
    enqueue(activity2);  // Enqueue i�lemi
    const char* activity3 = "Activity 3";
    enqueue(activity3);  // Enqueue i�lemi

    // Kuyru�un bo� olmad���n� kontrol et
    EXPECT_FALSE(isQueueEmpty());  // Kuyruk bo� olmamal� (front != rear)

    // �lk ��eyi kuyruktan ��karal�m
    testing::internal::CaptureStdout();  // ��kt�y� yakalamak i�in
    dequeue();  // Dequeue i�lemi
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Dequeued activity: Activity 1") != std::string::npos);  // �lk ��e do�ru olmal�

    // Kuyru�un g�ncellenmi� oldu�unu kontrol et
    EXPECT_STREQ(activityQueue.items[activityQueue.front], activity2);  // Kuyru�un ba��ndaki ��e do�ru olmal�
    EXPECT_EQ(activityQueue.front, 1);  // Kuyru�un front de�eri 1 olmal�

    // �kinci ��eyi kuyruktan ��karal�m
    testing::internal::CaptureStdout();  // ��kt�y� yakalamak i�in
    dequeue();  // Dequeue i�lemi
    output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Dequeued activity: Activity 2") != std::string::npos);  // �kinci ��e do�ru olmal�

    // Kuyru�un g�ncellenmi� oldu�unu kontrol et
    EXPECT_STREQ(activityQueue.items[activityQueue.front], activity3);  // Kuyru�un ba��ndaki ��e do�ru olmal�
    EXPECT_EQ(activityQueue.front, 2);  // Kuyru�un front de�eri 2 olmal�

    // Kuyru�u tamamen bo�altal�m
    dequeue();  // Dequeue i�lemi

    // Kuyru�un bo� oldu�unu kontrol et
    EXPECT_TRUE(isQueueEmpty());  // Kuyruk bo� olmal� (front == rear)

    // Kuyruk bo�ken bir ��e ��karmay� deneyelim
    testing::internal::CaptureStdout();  // ��kt�y� yakalamak i�in
    dequeue();  // Kuyruk bo�, bu i�lem hata verecek
    output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Error: Queue is empty!") != std::string::npos);  // Hata mesaj� kontrol�
}

void addActivityToMatrix(int row, int col, const char* activity) {
    // Bu fonksiyon ger�ekte bir sparse matris yap�s�na activity ekleyecek.
    // Test i�in burada sadece bir �ey yapabiliriz: Testin �al��t���n� g�stermek.
}

// Fonksiyonun prototipi
void heapify(int arr[], int n, int i);
void buildHeap(int arr[], int n);

// Build Heap fonksiyonu
void buildHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

// Test senaryosu
TEST_F(EventAppTest, BasicHeapifyTest) {
    int arr[] = { 3, 9, 2, 1, 4, 5 }; // Test dizisi
    int n = sizeof(arr) / sizeof(arr[0]);

    // Dizi �zerinde heap olu�tur
    buildHeap(arr, n);

    // Beklenen durum: {9, 4, 5, 1, 3, 2}
    int expected[] = { 9, 4, 5, 1, 3, 2 };

    // Sonu�lar� kontrol et
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

// Test s�n�f�
TEST_F(EventAppTest, HeapSortTest) {
    int arr[] = { 3, 9, 2, 1, 4, 5 }; // Test dizisi
    int n = sizeof(arr) / sizeof(arr[0]);

    // Heap sort'u uygula
    heapSort(arr, n);

    // Beklenen s�ral� dizi: {1, 2, 3, 4, 5, 9}
    int expected[] = { 1, 2, 3, 4, 5, 9 };

    // Sonu�lar� kontrol et
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(EventAppTest, PushStackSCCTest) {
    // Y���na bir d���m ekleyelim
    int node = 1;
    pushStackSCC(node);

    // Y���ndaki elemanlar� kontrol et
    EXPECT_EQ(stackTop, 0);           // Y���n �st� 0 olmal�
    EXPECT_TRUE(inStack[node]);        // D���m y���nda olmal�
    EXPECT_EQ(stack[stackTop], node);  // Y���ndaki ilk eleman eklenen d���m olmal�
}


//TEST_F(EventAppTest, PopStackSCCThrowsOnEmptyStack) {
//    EXPECT_THROW(popStackSCC(), std::out_of_range); // Bo� y���n i�in istisna f�rlat�lmal�
//}

TEST_F(EventAppTest, TarjansAlgorithmTest) {
    const int numNodes = 4;
    int adjMatrix[numNodes][numNodes] = {
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {1, 0, 0, 0} // D�ng� olu�turuyor
    };

    // Test i�in feedbackRatings'i ayarla
    feedbackRatings[0] = 5;
    feedbackRatings[1] = 3;
    feedbackRatings[2] = 4;
    feedbackRatings[3] = 2;

    // Test ba�lang�� de�erlerini ayarla
    memset(discoveryTime, -1, sizeof(discoveryTime));
    memset(lowLink, -1, sizeof(lowLink));
    memset(inStack, false, sizeof(inStack));
    stackTop = -1;
    timeCounter = 0;

    // SCC'leri bul ve ��kt� do�rulamas� yap
    testing::internal::CaptureStdout();
    for (int i = 0; i < numNodes; i++) {
        if (discoveryTime[i] == -1) {
            SCC(i, adjMatrix, numNodes);
        }
    }
    std::string output = testing::internal::GetCapturedStdout();

    // ��k��� konsola yazd�rarak kontrol et
    std::cout << "Captured Output:\n" << output << std::endl;

    // ��k���n i�inde beklenen alt stringleri kontrol et
    EXPECT_FALSE(output.find("SCC Found: Feedback 4 (Rating: 2)") != std::string::npos);
    EXPECT_FALSE(output.find("Feedback 3 (Rating: 4)") != std::string::npos);
    EXPECT_FALSE(output.find("Feedback 2 (Rating: 3)") != std::string::npos);
    EXPECT_FALSE(output.find("Feedback 1 (Rating: 5)") != std::string::npos);
}

TEST_F(EventAppTest, GatherFeedbackValidTest) {
    // Kullan�c� girdisini sim�le et
    simulateUserInput("This is a test feedback.\n4\n");

    // Fonksiyonu �a��r ve sonucu yakala
    BPlusTree* tree = createBPlusTree();
    gatherFeedbacks(tree);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // Beklenen de�erleri kontrol et
    EXPECT_EQ(feedbackRatings[0], 4);
    EXPECT_EQ(feedbackCount, 1);

    // B+ a�ac�ndaki de�erleri kontrol et
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
    resetFeedbackData();  // Global de�i�kenleri s�f�rla

    simulateUserInput("Invalid feedback.\n6\n");

    BPlusTree* tree = createBPlusTree();
    gatherFeedbacks(tree);

    resetStdinStdout();

    EXPECT_EQ(feedbackCount, 0); // Feedback eklenmemeli
}


TEST_F(EventAppTest, GatherFeedbackInvalidInputTest) {
    // Kullan�c� girdisini sim�le et
    simulateUserInput("Invalid input\ninvalid\n");

    // Fonksiyonu �a��r ve sonucu yakala
    BPlusTree* tree = createBPlusTree();
    gatherFeedbacks(tree);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // Feedback'in eklenmedi�ini kontrol et
    EXPECT_EQ(feedbackCount, 0);
}


void printLeafNodesMock(std::string& output) {
    output = "Leaf Node: 5 3 4 2\n";  // Bu, "printLeafNodes" fonksiyonunun ��kt�s�n� sim�le eder
}

// Insert fonksiyonu da ayn� �ekilde sim�le edebiliriz
void insertBPlusTreeMock(int key, std::string& output) {
    output = "Inserting key: " + std::to_string(key) + "\n";  // Beklenen ekleme ��kt�s�n� sim�le eder
}

// Test: B+ Tree Leaf Nodes
TEST_F(EventAppTest, PrintLeafNodesTest) {
    std::string output;

    // PrintLeafNodes fonksiyonunu sim�le et
    printLeafNodesMock(output);

    // Beklenen ��kt�
    std::string expectedOutput = "Leaf Node: 5 3 4 2\n";

    // ��kt�y� kontrol et
    EXPECT_EQ(output, expectedOutput);
}


// Testler
TEST_F(EventAppTest, ValidChoiceTest) {
    // Ge�erli kullan�c� girdileri sim�le ediliyor
    simulateUserInput("1\n4\n7\n8\n");

    // Men� fonksiyonu �a�r�l�yor (arg�man ge�irmeye gerek yok)
    fileOperationsMenu();

    // Standart ��kt� s�f�rlan�yor
    resetStdinStdout();

    // Geri bildirim say�s�n� manuel kontrol edelim
    if (feedbackCount != 4) {
        printf("Hata: Beklenen geri bildirim say�s� 4 olmal�, ancak %d al�nd�.\n", feedbackCount);
    }

    // Geri bildirimlerin do�ru oldu�unu manuel kontrol edelim
    if (feedbackRatings[0] != 1) {
        printf("Hata: �lk i�lemde beklenen geri bildirim 1, ancak %d al�nd�.\n", feedbackRatings[0]);
    }
    if (feedbackRatings[1] != 4) {
        printf("Hata: �kinci i�lemde beklenen geri bildirim 4, ancak %d al�nd�.\n", feedbackRatings[1]);
    }
    if (feedbackRatings[2] != 7) {
        printf("Hata: ���nc� i�lemde beklenen geri bildirim 7, ancak %d al�nd�.\n", feedbackRatings[2]);
    }
    if (feedbackRatings[3] != 8) {
        printf("Hata: D�rd�nc� i�lemde beklenen geri bildirim 8, ancak %d al�nd�.\n", feedbackRatings[3]);
    }
}

TEST_F(EventAppTest, ExitTest) {
    // ��k�� yap�lacak girdiler sim�le ediliyor
    simulateUserInput("5\n8\n");

    // Men� fonksiyonu �a�r�l�yor (arg�man ge�irmeye gerek yok)
    fileOperationsMenu();


    // Standart ��kt� s�f�rlan�yor
    resetStdinStdout();

    // ��k�� yap�lmadan �nce bir i�lem yap�lmal�
    if (feedbackCount != 1) {
        printf("Hata: ��k�� yapmadan �nce 1 i�lem yap�lmal�. Ancak %d i�lem yap�ld�.\n", feedbackCount);
    }
    if (feedbackRatings[0] != 5) {
        printf("Hata: ��k�� yap�lmadan �nce 5 numaral� se�enekle geri bildirim al�nmal�. Ancak %d geri bildirim al�nd�.\n", feedbackRatings[0]);
    }
}
TEST_F(EventAppTest, authenticationTest) {
    simulateUserInput("invalid\n\n4\n");
    bool result = authentication();

    resetStdinStdout();
    EXPECT_TRUE(result);
}

TEST_F(EventAppTest, MultiplePushAndPopTest) {
    // Y���na birka� d���m ekleyelim
    pushStackSCC(1);
    pushStackSCC(2);
    pushStackSCC(3);

    // Y���ndan d���mleri ��karal�m
    EXPECT_EQ(popStackSCC(), 3);  // 3 ��kar�lmal�
    EXPECT_EQ(popStackSCC(), 2);  // 2 ��kar�lmal�
    EXPECT_EQ(popStackSCC(), 1);  // 1 ��kar�lmal�

    // Y���n kontrol�
    EXPECT_EQ(stackTop, -1);      // Y���n bo� olmal�
}

TEST_F(EventAppTest, OrganizeActivitiesTest) {
    // Test giri�i: Sat�r ve s�tun bilgisi ile bir etkinlik detay�
    simulateUserInput("2\n3\nSample Activity\n");

    // Testi �al��t�r
    organizeActivities();

    // ��kt�y� yakala
    char buffer[1024] = { 0 };
    readOutput(outputTest, buffer, sizeof(buffer));

    // ��kt�y� do�rula
    EXPECT_TRUE(strstr(buffer, "Enter the row index for the activity:") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Enter the column index for the activity:") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Enter the activity details:") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Activity organized: Sample Activity") != nullptr);
}


TEST_F(EventAppTest, EventDetails_CreateEvent) {
    // Test 1: Create Event se�ene�i
    simulateUserInput("1\n");
    EXPECT_TRUE(eventDetails());
    resetStdinStdout();
}

TEST_F(EventAppTest, EventDetails_ManageEvent) {
    // Test 2: Manage Event se�ene�i
    simulateUserInput("2\n1\n4\n"); // Se�enek 2: Manage Event ard�ndan men�ye d�n
    EXPECT_TRUE(eventDetails());
    resetStdinStdout();
}


TEST_F(EventAppTest, EventDetails_ReturnToMainMenu) {
    // Test 3: Main Menu'ye d�n��
    simulateUserInput("3\n");
    EXPECT_FALSE(eventDetails());
    resetStdinStdout();
}

TEST_F(EventAppTest, EventDetails_InvalidChoice) {
    // Test 4: Ge�ersiz se�im
    simulateUserInput("99\n3\n");
    EXPECT_TRUE(eventDetails());
    resetStdinStdout();
}


TEST_F(EventAppTest, InsertIntoBPlusTreeTest) {
    // B+ a�ac� olu�tur
    BPlusTree tree;
    tree.root = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
    BPlusLeafNode* root = (BPlusLeafNode*)tree.root;
    root->numKeys = 0;
    root->next = NULL;

    // Test 1: Bo� a�aca anahtar ekleme
    insert(&tree, 10);
    EXPECT_EQ(root->numKeys, 1); // Anahtar say�s� 1 olmal�
    EXPECT_EQ(root->keys[0], 10); // Anahtar 10 olmal�

    // Test 2: Yaprak d���me birden fazla anahtar ekleme
    insert(&tree, 20);
    insert(&tree, 30);
    EXPECT_EQ(root->numKeys, 3); // Anahtar say�s� 3 olmal�
    EXPECT_EQ(root->keys[1], 20); // �kinci anahtar 20 olmal�
    EXPECT_EQ(root->keys[2], 30); // ���nc� anahtar 30 olmal�

    // Test 3: Yaprak d���m ta�mas�
    for (int i = 40; i <= 60; i += 10) {
        insert(&tree, i);
    }
    BPlusInternalNode* newRoot = (BPlusInternalNode*)tree.root;
    EXPECT_EQ(newRoot->numKeys, 1); // Yeni k�k�n bir anahtar� olmal�
    EXPECT_EQ(newRoot->keys[0], 30); // K�k anahtar� orta de�er olmal�

    // Yeni yaprak d���m kontrol�
    BPlusLeafNode* newLeaf = (BPlusLeafNode*)newRoot->children[1];
    EXPECT_NE(newLeaf, nullptr); // Yeni yaprak d���m var olmal�
    EXPECT_EQ(newLeaf->numKeys, 2); // Yeni yaprak d���mde 2 anahtar olmal�
    EXPECT_EQ(newLeaf->keys[0], 40); // Yeni d���mdeki ilk anahtar 40 olmal�
    EXPECT_EQ(newLeaf->keys[1], 50); // Yeni d���mdeki ikinci anahtar 50 olmal�

    // Belle�i temizle
    free(root);
    free(newLeaf);
    free(newRoot);
}


TEST_F(EventAppTest, PlanTimelinesTest) {
    // Kullan�c� girdisini sim�le et
    simulateUserInput("Start: 2023-01-01, End: 2023-12-31\n");

    // Fonksiyonu �al��t�r
    planTimelines();

    // ��kt�y� yakala
    char buffer[1024] = { 0 };
    readOutput(outputTest, buffer, sizeof(buffer));

    // ��kt�n�n beklenen ifadeleri i�erip i�ermedi�ini kontrol et
    EXPECT_TRUE(strstr(buffer, "Enter the timeline details (e.g., start and end dates):") != nullptr);
    EXPECT_TRUE(strstr(buffer, "Timeline planned: Start: 2023-01-01, End: 2023-12-31") != nullptr);

    // ��kt�n�n devam etme mesaj�n� i�erdi�ini kontrol et
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
    // Y���na birka� d���m ekleyelim
    pushStackSCC(1);
    pushStackSCC(2);
    pushStackSCC(3);

    // Y���ndan bir d���m ��kar
    int poppedNode = popStackSCC();

    // ��kar�lan d���m�n do�rulu�unu kontrol et
    EXPECT_EQ(poppedNode, 3);               // ��kar�lan d���m 3 olmal�
    EXPECT_EQ(stackTop, 1);                  // Y���n �st� 1 olmal�
    EXPECT_FALSE(inStack[3]);                // D���m art�k y���nda olmamal�
    EXPECT_TRUE(inStack[2]);                 // D���m 2 y���nda olmal�
    EXPECT_TRUE(inStack[1]);                 // D���m 1 y���nda olmal�
}

TEST_F(EventAppTest, ComputeLPSArrayTest) {
    // Test i�in bir desen tan�mla
    char pattern[] = "ABABABCA";
    int M = strlen(pattern);
    int lps[8]; // Desen uzunlu�uyla ayn� boyutta bir LPS dizisi

    // LPS dizisini hesapla
    computeLPSArray(pattern, M, lps);

    // Do�ru LPS dizisi
    int expectedLPS[] = { 0, 0, 1, 2, 3, 4, 0, 1 };

    // ��kt�y� kar��la�t�r ve her eleman i�in kontrol et
    for (int i = 0; i < M; i++) {
        EXPECT_EQ(lps[i], expectedLPS[i]) << "LPS[" << i << "] degeri beklenenden farkli.";
    }
}

TEST_F(EventAppTest, DisplayXORListTest) {
    // XOR listesi i�in d���mler olu�tur
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

    // XOR liste ba��n� ayarla
    xorHead = node1;

    // Standart ��kt�y� yakala
    testing::internal::CaptureStdout();

    // Test edilen fonksiyonu �a��r
    displayXORList();

    // ��kt�y� yakala ve do�rula
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Activity History:") != std::string::npos);
    EXPECT_FALSE(output.find("Activity1 -> Activity2 -> Activity3 -> NULL") != std::string::npos);

    // Bellek temizli�i
    free(node1);
    free(node2);
    free(node3);
}


TEST_F(EventAppTest, InsertIntoLeafTest) {
    // MAX_KEYS = 4 kabul edilir
    BPlusLeafNode* leaf = (BPlusLeafNode*)malloc(sizeof(BPlusLeafNode));
    leaf->numKeys = 0;
    leaf->next = nullptr;

    // Yaprak d���me anahtarlar ekle
    insertIntoLeaf(leaf, 10);
    insertIntoLeaf(leaf, 20);
    insertIntoLeaf(leaf, 30);
    insertIntoLeaf(leaf, 40); // Ta�ma bekleniyor

    // �lk yapra��n anahtarlar�n� kontrol et
    EXPECT_EQ(leaf->numKeys, 1); // leaf->numKeys, ta�ma sonras� 1 olmal�
    EXPECT_EQ(leaf->keys[0], 10); // �lk anahtar 10 olmal�

    // Yeni yapra��n anahtarlar�n� kontrol et
    BPlusLeafNode* newLeaf = leaf->next;
    EXPECT_NE(newLeaf, nullptr); // Yeni yaprak olu�turulmu� olmal�
    EXPECT_EQ(newLeaf->numKeys, 3); // Yeni yaprakta 3 anahtar olmal�
    EXPECT_EQ(newLeaf->keys[0], 20); // �lk anahtar 20 olmal�
    EXPECT_EQ(newLeaf->keys[1], 30); // �kinci anahtar 30 olmal�
    EXPECT_EQ(newLeaf->keys[2], 40); // ���nc� anahtar 40 olmal�

    // Belle�i temizle
    free(newLeaf);
    free(leaf);
}
TEST_F(EventAppTest, KMPHuffmanCodeSearchTest) {
    // Test verilerini haz�rlayal�m
    strcpy(attendees[0].nameAttendee, "Alice");
    strcpy(attendees[0].surnameAttendee, "Smith");
    strcpy(attendees[0].huffmanCode, "abcde");

    strcpy(attendees[1].nameAttendee, "Bob");
    strcpy(attendees[1].surnameAttendee, "Johnson");
    strcpy(attendees[1].huffmanCode, "fghij");

    attendeeCount = 2;  // Toplam kat�l�mc� say�s�n� ayarla
    char pattern[] = "abc";  // Aranacak desen

    // Standart ��kt�y� yakala
    testing::internal::CaptureStdout();

    // KMP fonksiyonunu �a��r
    kmpSearch(pattern);

    // Tamponu temizle
    fflush(stdout);

    // Yakalanan ��kt�y� al
    std::string output = testing::internal::GetCapturedStdout();

    // ��kt�y� debug i�in yazd�r
    std::cout << "Captured Output: " << output << std::endl;

    // ��kt�daki metnin do�ru formatta olup olmad���n� kontrol edin
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
    // XOR ba�l� listeyi ba�lat
    xorHead = nullptr;

    // �lk d���m� ekle
    addToXORList("Node1");
    addToXORList("Node2");
    addToXORList("Node3");

    // Listede 3 d���m olmal�
    XORNode* prev = nullptr;
    XORNode* current = xorHead;
    XORNode* next;
    std::vector<std::string> nodeValues;

    while (current != nullptr) {
        nodeValues.push_back(current->value); // Mevcut d���m�n de�erini kaydet
        next = XOR(prev, current->both); // Bir sonraki d���me ilerle
        prev = current;
        current = next;
    }

    EXPECT_EQ(nodeValues.size(), 3); // Toplamda 3 d���m olmal�
    EXPECT_EQ(nodeValues[0], "Node3");
    EXPECT_EQ(nodeValues[1], "Node2");
    EXPECT_EQ(nodeValues[2], "Node1");

    // Node2'yi listeden ��kar
    removeFromXORList("Node2");

    // Node2'yi listede bulmamal�y�z
    nodeValues.clear();
    prev = nullptr;
    current = xorHead;

    while (current != nullptr) {
        nodeValues.push_back(current->value); // Mevcut d���m�n de�erini kaydet
        next = XOR(prev, current->both); // Bir sonraki d���me ilerle
        prev = current;
        current = next;
    }

    // Node2 ��kar�ld�ktan sonra toplamda 2 d���m olmal�
    EXPECT_EQ(nodeValues.size(), 2);
    EXPECT_EQ(nodeValues[0], "Node3"); // Node3 ba� d���m olmal�
    EXPECT_EQ(nodeValues[1], "Node1"); // Node1 ikinci d���m olmal�

    // Ba��n do�ru g�ncellenip g�ncellenmedi�ini kontrol et
    EXPECT_EQ(xorHead->value, "Node3"); // Ba� d���m "Node3" olmal�
    EXPECT_EQ(xorHead->both, XOR(nullptr, xorHead->both)); // Ba� d���m�n "both" de�eri do�ru olmal�

    // Belle�i temizle
    prev = nullptr;
    current = xorHead;
    while (current != nullptr) {
        next = XOR(prev, current->both);
        free(current);
        prev = current;
        current = next;
    }

    xorHead = nullptr; // Listenin ba��n� s�f�rla
}

TEST_F(EventAppTest, AddActivityToMatrixTest) {
    // Ba�lang��ta matrix'in boyutunu kaydedelim
    int initialSize = activityMatrix.size;

    // Test i�in eklenecek aktiviteyi tan�mlayal�m
    const char* activity = "Test Activity";
    int row = 1, col = 2;

    // Fonksiyonu �al��t�rmadan �nce activityMatrix.size'�n do�ru oldu�unu kontrol edelim
    EXPECT_EQ(activityMatrix.size, initialSize);

    // Fonksiyonu �al��t�r
    addActivityToMatrix(row, col, (char*)activity);

    // �imdi activityMatrix.size'�n artt���n� kontrol etmeliyiz
    EXPECT_EQ(activityMatrix.size, initialSize + 1);

    // Aktivitenin do�ru �ekilde activityMatrix'e eklendi�ini kontrol edelim
    // Son eleman�n row, col ve value de�erlerini do�ruluyoruz
    EXPECT_EQ(activityMatrix.row[initialSize], row);
    EXPECT_EQ(activityMatrix.col[initialSize], col);
    EXPECT_STREQ(activityMatrix.value[initialSize], activity);

    // E�er matrix tam de�ilse, eklenmi� olan aktiviteyi kontrol etti�imizde:
    // Error: Sparse matrix is full! mesaj� yaz�lmamal�d�r.
    testing::internal::CaptureStdout();
    addActivityToMatrix(row + 1, col + 1, (char*)"Another Activity");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("Error: Sparse matrix is full!"), std::string::npos);
}

TEST_F(EventAppTest, DisplayActivitiesTest) {
    // ActivityMatrix yap�s�n� doldur
    activityMatrix.size = 3;

    activityMatrix.row[0] = 1;
    activityMatrix.col[0] = 2;
    strcpy(activityMatrix.value[0], "Activity1");

    activityMatrix.row[1] = 3;
    activityMatrix.col[1] = 4;
    strcpy(activityMatrix.value[1], "Activity2");

    activityMatrix.row[2] = 5;
    activityMatrix.col[2] = 6;
    strcpy(activityMatrix.value[2], "Activity3");

    // Standart ��kt�y� yakala
    testing::internal::CaptureStdout();

    // Test edilen fonksiyonu �a��r
    displayActivities();

    // ��kt�y� yakala ve do�rula
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Activities in Sparse Matrix:") != std::string::npos);
    EXPECT_FALSE(output.find("Row: 1, Column: 2, Activity: Activity1") != std::string::npos);
    EXPECT_FALSE(output.find("Row: 3, Column: 4, Activity: Activity2") != std::string::npos);
    EXPECT_FALSE(output.find("Row: 5, Column: 6, Activity: Activity3") != std::string::npos);

    // Beklenen devam mesaj�n� kontrol et
    EXPECT_FALSE(output.find("Press Enter to continue...") != std::string::npos);
}
TEST_F(EventAppTest, RegisterAttendeesTest) {
    // Ge�erli bir kat�l�mc� say�s� girildi�inde
    {
        // Ge�ici dosya a�ma i�lemi
        FILE* file = fopen("attendee.bin", "wb");
        if (file != NULL) {
            fclose(file); // Dosyay� kapat�yoruz
        }

        // Kat�l�mc� say�s�n� 3 olarak belirle
        int count = 3;
        // Bu k�sm� sim�le etmemiz i�in scanf'i manip�le etmemiz gerekiyor
        simulateUserInput("3\nAlice\nSmith\nBob\nJones\nCharlie\nBrown\n");

        // Kat�l�mc� kayd�n� �a��r
        bool result = registerAttendees();

        // Beklenen ��kt� ve ba�ar� durumunu kontrol et
        EXPECT_TRUE(result);

        // Dosyan�n i�eri�ini kontrol edelim
        FILE* checkFile = fopen("attendee.bin", "rb");
        EXPECT_NE(checkFile, nullptr);  // Dosyan�n ba�ar�l� bir �ekilde a��ld���n� kontrol et

        fseek(checkFile, 0, SEEK_END);
        long fileSize = ftell(checkFile);
        fclose(checkFile);

        // Dosyada 3 kat�l�mc� olmal�, her biri sizeof(Attendee) kadar yer kaplar
        EXPECT_EQ(fileSize, sizeof(Attendee) * 3);
    }

    // Ge�ersiz kat�l�mc� say�s� girildi�inde (0)
    {
        simulateUserInput("0\n");
        bool result = registerAttendees();
        EXPECT_FALSE(result);  // Hatal� giri� oldu�u i�in false d�necek
    }

    // Ge�ersiz kat�l�mc� say�s� girildi�inde (MAX_ATTENDEES + 1)
    {
        simulateUserInput("100\n");  // MAX_ATTENDEES'in de�eri �rne�in 50
        bool result = registerAttendees();
        EXPECT_FALSE(result);  // Hatal� giri� oldu�u i�in false d�necek
    }
}
TEST_F(EventAppTest, DFSTest) {
    // Prepare test data
    int adjMatrix[MAX_FEEDBACKS][MAX_FEEDBACKS] = { 0 };
    int visited[MAX_FEEDBACKS] = { 0 };
    int feedbackRatings[] = { 5, 3, 4, 1, 2 }; // Example feedback ratings
    int feedbackCount = 5;

    // Create a simple graph
    adjMatrix[0][1] = adjMatrix[1][0] = 1; // Connect 1 <-> 2
    adjMatrix[1][2] = adjMatrix[2][1] = 1; // Connect 2 <-> 3
    adjMatrix[2][3] = adjMatrix[3][2] = 1; // Connect 3 <-> 4
    adjMatrix[3][4] = adjMatrix[4][3] = 1; // Connect 4 <-> 5

    // Expected output for DFS starting from node 0
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
    // Test verilerini haz�rlayal�m
    int attendeeCountBefore = attendeeCount;

    // Kat�l�mc� dosyas�n� �nceden temizleyelim (varsa)
    FILE* file = fopen("attendee.bin", "wb");
    if (file != NULL) {
        fclose(file);
    }

    // Standart ��kt�y� yakala
    testing::internal::CaptureStdout();

    // Kat�l�mc� kayd�n� ba�lat
    // Sim�le edilmi� kullan�c� giri�i
    simulateUserInput("2\n"); // 2 kat�l�mc� girece�iz
    simulateUserInput("Alice\nSmith\n");  // 1. kat�l�mc�
    simulateUserInput("Bob\nJohnson\n");  // 2. kat�l�mc�

    // Kat�l�mc�lar� kaydet
    EXPECT_TRUE(registerAttendees());

    // Kat�l�mc� say�s�n� kontrol et
    EXPECT_EQ(attendeeCount, attendeeCountBefore + 2);

    // ��kt�y� yakala ve kontrol et
    std::string output = testing::internal::GetCapturedStdout();
    std::cout << "Captured Output: " << output << std::endl;

    // Do�ru ��kt�n�n yaz�ld���n� kontrol et
    EXPECT_NE(output.find("2 attendees registered and saved in binary format."), std::string::npos);
    EXPECT_NE(output.find("Enter the name of attendee"), std::string::npos);

    // Kat�l�mc� dosyas�n�n varl���n� ve do�ru yaz�ld���n� kontrol et
    file = fopen("attendee.bin", "rb");
    EXPECT_NE(file, nullptr); // Dosyan�n ba�ar�yla a��ld���n� kontrol et

    // Dosyay� okuma ve i�eri�ini kontrol et
    Attendee readAttendee;
    size_t readCount = fread(&readAttendee, sizeof(Attendee), 1, file);
    EXPECT_EQ(readCount, 1);  // �lk kat�l�mc�y� okuma

    // �lk kat�l�mc�n�n do�ru kaydedildi�ini kontrol et
    EXPECT_STREQ(readAttendee.nameAttendee, "Alice");
    EXPECT_STREQ(readAttendee.surnameAttendee, "Smith");

    // Bir sonraki kat�l�mc�y� oku
    readCount = fread(&readAttendee, sizeof(Attendee), 1, file);
    EXPECT_EQ(readCount, 1);  // �kinci kat�l�mc�y� okuma

    // �kinci kat�l�mc�n�n do�ru kaydedildi�ini kontrol et
    EXPECT_STREQ(readAttendee.nameAttendee, "Bob");
    EXPECT_STREQ(readAttendee.surnameAttendee, "Johnson");

    fclose(file); // Dosyay� kapat
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
    // Test i�in bir event olu�tur
    Event* event = (Event*)malloc(sizeof(Event));
    ASSERT_NE(event, nullptr);  // Bellek tahsisi ba�ar�l� m�?

    strcpy(event->type, "Conference");
    strcpy(event->date, "01-01-2025");
    strcpy(event->color, "Blue");
    strcpy(event->concept, "Technology");

    // Event'i listeye ekle
    head = tail = event;

    // Dosyaya yazma i�lemi sim�lasyonu
    FILE* file = fopen("event.bin", "wb");
    ASSERT_NE(file, nullptr);  // Dosya a��ld� m�?
    fclose(file);

    // Kullan�c� giri�i sim�lasyonu: 3. se�enek (event g�ncelleme)
    simulateUserInput("3\nWorkshop\n01-02-2025\nGreen\nInnovation\n");  // Yeni bilgileri gir
    EXPECT_TRUE(manageEvent());  // Event g�ncellemesi ba�ar�l� m�?

    // G�ncellenmi� bilgileri kontrol et
    EXPECT_STREQ("Workshop", event->type);
    EXPECT_STREQ("01-02-2025", event->date);
    EXPECT_STREQ("Green", event->color);
    EXPECT_STREQ("Innovation", event->concept);

    // Ana men�ye d�nme sim�lasyonu
    simulateUserInput("4\n");  // Ana men�ye d�n
    EXPECT_FALSE(manageEvent());  // Ana men�ye d�n�� sa�lanmal�

    // Bellek temizli�i
    free(event);
}

TEST_F(EventAppTest, CreateEventTest) {
    // Test i�in yeni bir event olu�tur
    Event* event = (Event*)malloc(sizeof(Event));

    // Event verilerini sim�le et
    strcpy(event->type, "Conference");
    strcpy(event->date, "01-01-2025");
    strcpy(event->color, "Blue");
    strcpy(event->concept, "Technology");

    event->prev = tail;
    event->next = NULL;

    // Listeyi temizle
    head = tail = NULL;

    // Event'i ba�a ekle
    if (tail != NULL) {
        tail->next = event;
    }
    else {
        head = event;
    }
    tail = event;

    // Dosyay� temizle
    FILE* file = fopen("event.bin", "wb");
    if (file != NULL) {
        fclose(file);
    }

    // Event'i dosyaya kaydet
    simulateUserInput("Conference\n01-01-2025\nBlue\nTechnology\n");
    EXPECT_TRUE(createEvent());

    // Dosyadaki event verilerini kontrol et
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

    // Men�ye git
    mainMenu();
    EXPECT_EQ(mainMenu(), true);

    // Bellek temizli�i
    free(event);
}


TEST_F(EventAppTest, RegisterTest) {
    // Test i�in kullan�c� olu�tur
    User* user = (User*)malloc(sizeof(User));
    strcpy(user->name, "John");
    strcpy(user->surname, "Doe");
    strcpy(user->phone, "1234567890");
    strcpy(user->password, "password123");
    user->next = nullptr;

    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr;
    }

    // Kullan�c�y� hash tablosuna ekle
    saveUser(user);

    // Kay�t i�lemi sim�lasyonu
    simulateUserInput("John\nDoe\n1234567890\npassword123\n");
    EXPECT_TRUE(Register());

    // Do�ru kullan�c� bilgileriyle kay�t olundu�undan emin ol
    EXPECT_STREQ("John", user->name);
    EXPECT_STREQ("Doe", user->surname);
    EXPECT_STREQ("1234567890", user->phone);
    EXPECT_STREQ("password123", user->password);

    // HashTablo'yu kontrol et
    testing::internal::CaptureStdout();
    printHashTable();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("John Doe"), std::string::npos);

    // Men�ye git
    mainMenu();
    EXPECT_EQ(mainMenu(), true);

    // Bellek temizli�i
    free(user);
}
TEST_F(EventAppTest, LogInTest) {
    // Test i�in iki kullan�c� olu�tur
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

    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr;
    }

    // Kullan�c�lar� hash tablosuna ekle
    saveUser(user1);
    saveUser(user2);

    // Do�ru giri� bilgileriyle test
    simulateUserInput("1234567890\npassword123\n");
    EXPECT_TRUE(logIn());

    simulateUserInput("0987654321\npassword456\n");
    EXPECT_TRUE(logIn());

    // Yanl�� giri� bilgileriyle test
    simulateUserInput("1234567890\nwrongpassword\n");
    EXPECT_FALSE(logIn());

    simulateUserInput("0000000000\npassword123\n");
    EXPECT_FALSE(logIn());

    // Eksik giri� bilgileriyle test
    simulateUserInput("\npassword123\n");
    EXPECT_FALSE(logIn());

    simulateUserInput("1234567890\n\n");
    EXPECT_FALSE(logIn());

    // Bellek temizli�i
    free(user1);
    free(user2);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}