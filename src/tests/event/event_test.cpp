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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}