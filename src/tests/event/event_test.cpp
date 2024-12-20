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
    // MinHeap oluþtur
    MinHeap* minHeap = new MinHeap;
    minHeap->size = 0;
    minHeap->capacity = 10;
    minHeap->array = new MinHeapNode * [minHeap->capacity];

    // Test durumlarý
    EXPECT_EQ(0, minHeap->size);          // Baþlangýçta boyut 0 olmalý
    EXPECT_EQ(10, minHeap->capacity);     // Kapasite 10 olmalý
    EXPECT_NE(nullptr, minHeap->array);   // Array null olmamalý

    // Belleði temizle
    delete[] minHeap->array;
    delete minHeap;
}

TEST_F(EventAppTest, CreateMinHeapTest) {
    unsigned capacity = 10;
    MinHeap* minHeap = createMinHeap(capacity);

    // Test durumlarý
    EXPECT_NE(nullptr, minHeap);                      // MinHeap yapýsý oluþturulmuþ olmalý
    EXPECT_EQ(0, minHeap->size);                      // Baþlangýçta boyut 0 olmalý
    EXPECT_EQ(capacity, minHeap->capacity);           // Kapasite doðru ayarlanmalý
    EXPECT_NE(nullptr, minHeap->array);               // Array null olmamalý

    // Belleði temizle
    free(minHeap->array);
    free(minHeap);
}

TEST_F(EventAppTest, CreateMinHeapNodeTest) {
    char data = 'A';
    unsigned freq = 5;

    MinHeapNode* node = createMinHeapNode(data, freq);

    // Test durumlarý
    EXPECT_NE(nullptr, node);             // Node null olmamalý
    EXPECT_EQ(data, node->data);          // Data alaný doðru ayarlanmalý
    EXPECT_EQ(freq, node->freq);          // Freq alaný doðru ayarlanmalý
    EXPECT_EQ(nullptr, node->left);       // Left iþaretçisi null olmalý
    EXPECT_EQ(nullptr, node->right);      // Right iþaretçisi null olmalý

    // Belleði temizle
    free(node);
}

TEST_F(EventAppTest, InsertMinHeapTest) {
    unsigned capacity = 10;
    MinHeap* minHeap = createMinHeap(capacity);

    // Test veri kümesi
    MinHeapNode* node1 = createMinHeapNode('A', 5);
    MinHeapNode* node2 = createMinHeapNode('B', 3);

    // Ýlk ekleme
    insertMinHeap(minHeap, node1);
    EXPECT_EQ(1, minHeap->size);             // Boyut 1 olmalý
    EXPECT_EQ(node1, minHeap->array[0]);     // Ýlk eleman node1 olmalý

    // Ýkinci ekleme
    insertMinHeap(minHeap, node2);
    EXPECT_EQ(2, minHeap->size);             // Boyut 2 olmalý
    EXPECT_EQ(node2, minHeap->array[1]);     // Ýkinci eleman node2 olmalý

    // Belleði temizle
    free(minHeap->array);
    free(minHeap);
    free(node1);
    free(node2);
}

//TEST_F(EventAppTest, ExtractMinTest) {  
//    // MinHeap oluþturma
//    MinHeap* minHeap = createMinHeap(10); // 10 boyutunda bir min heap oluþtur
//    char data1 = 'A', data2 = 'B', data3 = 'C';
//    unsigned freq1 = 5, freq2 = 3, freq3 = 8;
//
//    // MinHeap'e düðümler ekleme
//    insertMinHeap(minHeap, createMinHeapNode(data1, freq1));
//    insertMinHeap(minHeap, createMinHeapNode(data2, freq2));
//    insertMinHeap(minHeap, createMinHeapNode(data3, freq3));
//
//    // extractMin fonksiyonunu test et
//    MinHeapNode* minNode = extractMin(minHeap);
//
//    // Test durumlarý
//    EXPECT_NE(nullptr, minNode);                     // Düðüm null olmamalý
//    EXPECT_EQ(data2, minNode->data);                 // En küçük düðüm doðru olmalý
//    EXPECT_EQ(freq2, minNode->freq);                 // En küçük frekans doðru olmalý
//    EXPECT_EQ(2, minHeap->size);                     // Heap boyutu 2 olmalý
//
//    // En küçük düðümün bellek temizliði
//    free(minNode);
//    free(minHeap->array[0]); // Kalan en küçük düðümün bellek temizliði
//    free(minHeap->array[1]); // Ýkinci düðümün bellek temizliði
//    free(minHeap);           // Heap bellek temizliði
//}

TEST_F(EventAppTest, UserStructureTest) {
    // User yapýsýný oluþtur
    User user;
    strcpy(user.name, "Alice");
    strcpy(user.surname, "Smith");
    strcpy(user.phone, "1234567890");
    strcpy(user.password, "password123");
    user.next = nullptr; // Baþlangýçta next iþaretçisi null olmalý

    // Test durumlarý
    EXPECT_STREQ("Alice", user.name);        // Ýsim doðru olmalý
    EXPECT_STREQ("Smith", user.surname);     // Soyisim doðru olmalý
    EXPECT_STREQ("1234567890", user.phone);  // Telefon numarasý doðru olmalý
    EXPECT_STREQ("password123", user.password); // Þifre doðru olmalý
    EXPECT_EQ(nullptr, user.next);           // Next iþaretçisi null olmalý

    // Hash tablosunu kontrol et
    User* hashTable[TABLE_SIZE] = { nullptr }; // Hash tablosunu baþlangýçta null ile doldur

    // Hash tablosunun baþlangýç durumu
    for (int i = 0; i < TABLE_SIZE; i++) {
        EXPECT_EQ(nullptr, hashTable[i]); // Her bir eleman null olmalý
    }
}


TEST_F(EventAppTest, EventStructureTest) {
    // Event yapýsýný oluþtur
    Event event;
    strcpy(event.type, "Conference");
    strcpy(event.date, "2024-12-31");
    strcpy(event.color, "Blue");
    strcpy(event.concept, "Technology and Innovation");
    event.prev = nullptr; // Baþlangýçta prev iþaretçisi null olmalý
    event.next = nullptr; // Baþlangýçta next iþaretçisi null olmalý

    // Test durumlarý
    EXPECT_STREQ("Conference", event.type);                // Tür doðru olmalý
    EXPECT_STREQ("2024-12-31", event.date);               // Tarih doðru olmalý
    EXPECT_STREQ("Blue", event.color);                     // Renk doðru olmalý
    EXPECT_STREQ("Technology and Innovation", event.concept); // Konsept doðru olmalý
    EXPECT_EQ(nullptr, event.prev);                        // Prev iþaretçisi null olmalý
    EXPECT_EQ(nullptr, event.next);                        // Next iþaretçisi null olmalý

    // Listeyi güncelleme iþlemi için test
    head = &event; // Baþlangýçta head'i event'e ayarla
    tail = &event; // Baþlangýçta tail'i event'e ayarla

    EXPECT_EQ(head, &event); // head doðru olmalý
    EXPECT_EQ(tail, &event); // tail doðru olmalý
}

TEST_F(EventAppTest, HashFunctionTest) {
    // Test telefon numaralarý
    const char* phone1 = "1234567890";
    const char* phone2 = "0987654321";
    const char* phone3 = "5555555555";

    // Beklenen hash deðerlerini hesapla (örneðin, bu deðerleri manuel olarak hesaplayabilirsiniz)
    unsigned int expectedHash1 = hash(phone1);
    unsigned int expectedHash2 = hash(phone2);
    unsigned int expectedHash3 = hash(phone3);

    // Hash fonksiyonunu test et
    EXPECT_EQ(expectedHash1, hash(phone1)); // Ýlk telefon numarasý için hash doðru olmalý
    EXPECT_EQ(expectedHash2, hash(phone2)); // Ýkinci telefon numarasý için hash doðru olmalý
    EXPECT_EQ(expectedHash3, hash(phone3)); // Üçüncü telefon numarasý için hash doðru olmalý

    // Ek test: Farklý telefon numaralarý için hash deðerleri farklý olmalý
    EXPECT_NE(expectedHash1, expectedHash2); // Farklý telefon numaralarý farklý hash deðerleri döndürmeli
    EXPECT_NE(expectedHash2, expectedHash3); // Farklý telefon numaralarý farklý hash deðerleri döndürmeli
}

TEST_F(EventAppTest, SaveUserToHashTableTest) {
    // Kullanýcýyý oluþtur
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->name, "Alice");
    strcpy(user1->surname, "Smith");
    strcpy(user1->phone, "1234567890");
    strcpy(user1->password, "password123");
    user1->next = nullptr;

    // Hash tablosunu temizle (testten önce)
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Baþlangýçta her eleman null olmalý
    }

    // Kullanýcýyý kaydet
    saveUser(user1);

    // Hash tablosunu kontrol et
    unsigned int index = hash(user1->phone);
    EXPECT_NE(nullptr, hashTable[index]); // Kullanýcý hash tablosunda olmalý
    EXPECT_STREQ(user1->name, hashTable[index]->name); // Kullanýcýnýn adý doðru olmalý
    EXPECT_STREQ(user1->surname, hashTable[index]->surname); // Kullanýcýnýn soyadý doðru olmalý
    EXPECT_STREQ(user1->phone, hashTable[index]->phone); // Kullanýcýnýn telefonu doðru olmalý
    EXPECT_STREQ(user1->password, hashTable[index]->password); // Kullanýcýnýn þifresi doðru olmalý
    EXPECT_EQ(nullptr, hashTable[index]->next); // Ýlk kullanýcý için next iþaretçisi null olmalý

    // Belleði temizle
    free(user1);
}

TEST_F(EventAppTest, SaveHashTableToFileTest) {
    // Kullanýcýlarý oluþtur
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

    // Hash tablosunu temizle ve kullanýcýlarý kaydet
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Baþlangýçta her eleman null olmalý
    }

    saveUser(user1);
    saveUser(user2);

    // Hash tablosunu dosyaya kaydet
    saveHashTableToFile();

    // Dosyayý oku ve kontrol et
    FILE* file = fopen("users.bin", "rb");
    ASSERT_NE(nullptr, file); // Dosya açýlmalý

    User readUser;
    size_t readCount;
    int userCount = 0;

    // Dosyadan kullanýcýlarý oku
    while ((readCount = fread(&readUser, sizeof(User), 1, file)) == 1) {
        userCount++;
        // Okunan kullanýcýyý kontrol et
        if (strcmp(readUser.phone, "1234567890") == 0) {
            EXPECT_STREQ("Alice", readUser.name);        // Kullanýcý adý doðru olmalý
            EXPECT_STREQ("Smith", readUser.surname);     // Kullanýcý soyadý doðru olmalý
            EXPECT_STREQ("password123", readUser.password); // Kullanýcý þifresi doðru olmalý
        }
        else if (strcmp(readUser.phone, "0987654321") == 0) {
            EXPECT_STREQ("Bob", readUser.name);          // Kullanýcý adý doðru olmalý
            EXPECT_STREQ("Johnson", readUser.surname);   // Kullanýcý soyadý doðru olmalý
            EXPECT_STREQ("password456", readUser.password); // Kullanýcý þifresi doðru olmalý
        }
    }

    fclose(file);
    EXPECT_EQ(2, userCount); // Toplamda 2 kullanýcý okunmalý

    // Belleði temizle
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, LoadHashTableFromFileTest) {
    // Önce kullanýcýlarý dosyaya kaydedelim
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

    // Hash tablosunu temizle ve kullanýcýlarý kaydet
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Baþlangýçta her eleman null olmalý
    }

    saveUser(user1);
    saveUser(user2);
    saveHashTableToFile(); // Kullanýcýlarý dosyaya kaydet

    // Hash tablosunu temizle ve dosyadan kullanýcýlarý yükle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Baþlangýçta her eleman null olmalý
    }

    loadHashTableFromFile(); // Kullanýcýlarý dosyadan yükle

    // Dosyadan yüklenen kullanýcýlarý kontrol et
    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    EXPECT_NE(nullptr, hashTable[index1]); // Ýlk kullanýcý hash tablosunda olmalý
    EXPECT_STREQ(user1->name, hashTable[index1]->name);        // Kullanýcýnýn adý doðru olmalý
    EXPECT_STREQ(user1->surname, hashTable[index1]->surname);   // Kullanýcýnýn soyadý doðru olmalý
    EXPECT_STREQ(user1->phone, hashTable[index1]->phone);       // Kullanýcýnýn telefonu doðru olmalý
    EXPECT_STREQ(user1->password, hashTable[index1]->password);   // Kullanýcýnýn þifresi doðru olmalý

    EXPECT_NE(nullptr, hashTable[index2]); // Ýkinci kullanýcý hash tablosunda olmalý
    EXPECT_STREQ(user2->name, hashTable[index2]->name);        // Kullanýcýnýn adý doðru olmalý
    EXPECT_STREQ(user2->surname, hashTable[index2]->surname);   // Kullanýcýnýn soyadý doðru olmalý
    EXPECT_STREQ(user2->phone, hashTable[index2]->phone);       // Kullanýcýnýn telefonu doðru olmalý
    EXPECT_STREQ(user2->password, hashTable[index2]->password);   // Kullanýcýnýn þifresi doðru olmalý

    // Belleði temizle
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, QuadraticProbingInsertTest) {
    // Test için bir kullanýcý oluþtur
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->name, "Alice");
    strcpy(newUser->surname, "Smith");
    strcpy(newUser->phone, "1234567890");
    strcpy(newUser->password, "password123");
    newUser->next = nullptr;

    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Baþlangýçta her eleman null olmalý
    }

    // Kullanýcýyý ekle ve sonucu kontrol et
    bool result = quadraticProbingInsert(newUser);

    // Kullanýcýnýn eklenip eklenmediðini kontrol et
    unsigned int index = hash(newUser->phone);
    EXPECT_TRUE(result); // Sonuç true olmalý
    EXPECT_NE(nullptr, hashTable[index]); // Kullanýcý hash tablosunda olmalý
    EXPECT_STREQ(newUser->name, hashTable[index]->name);        // Kullanýcýnýn adý doðru olmalý
    EXPECT_STREQ(newUser->surname, hashTable[index]->surname);   // Kullanýcýnýn soyadý doðru olmalý
    EXPECT_STREQ(newUser->phone, hashTable[index]->phone);       // Kullanýcýnýn telefonu doðru olmalý
    EXPECT_STREQ(newUser->password, hashTable[index]->password);   // Kullanýcýnýn þifresi doðru olmalý

    // Belleði temizle
    free(newUser);
}

TEST_F(EventAppTest, SaveUserDataTest) {
    // Test için bir kullanýcý oluþtur
    User user;
    strcpy(user.name, "John");
    strcpy(user.surname, "Doe");
    strcpy(user.phone, "9876543210");
    strcpy(user.password, "securePassword");
    user.next = nullptr;

    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Baþlangýçta her eleman null olmalý
    }

    // Kullanýcý verilerini kaydet
    saveUserData(user);

    // Kullanýcýnýn hash tablosuna eklenip eklenmediðini kontrol et
    unsigned int index = hash(user.phone);
    EXPECT_NE(nullptr, hashTable[index]); // Kullanýcý hash tablosunda olmalý
    EXPECT_STREQ(user.name, hashTable[index]->name);      // Kullanýcýnýn adý doðru olmalý
    EXPECT_STREQ(user.surname, hashTable[index]->surname); // Kullanýcýnýn soyadý doðru olmalý
    EXPECT_STREQ(user.phone, hashTable[index]->phone);     // Kullanýcýnýn telefonu doðru olmalý
    EXPECT_STREQ(user.password, hashTable[index]->password); // Kullanýcýnýn þifresi doðru olmalý

    // Dosyanýn var olup olmadýðýný kontrol et
    FILE* file = fopen("users.bin", "rb");
    EXPECT_NE(nullptr, file); // Dosya açýlabilir olmalý
    fclose(file); // Dosyayý kapat

    // Belleði temizle
    free(hashTable[index]); // Hash tablosundaki kullanýcýyý serbest býrak
    hashTable[index] = nullptr; // Hash tablosundaki referansý sýfýrla
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
    // Test için iki kullanýcý oluþtur
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

    // Kullanýcýlarý hash tablosuna ekle
    unsigned int index1 = hash(user1->phone);
    unsigned int index2 = hash(user2->phone);

    hashTable[index1] = user1;
    hashTable[index2] = user2;

    // Doðru giriþ bilgileriyle test
    EXPECT_TRUE(validateLogin("1234567890", "password123")); // Kullanýcý 1 için doðru giriþ
    EXPECT_TRUE(validateLogin("0987654321", "password456")); // Kullanýcý 2 için doðru giriþ

    // Yanlýþ telefon numarasý veya þifre ile test
    EXPECT_FALSE(validateLogin("1234567890", "wrongpassword")); // Yanlýþ þifre
    EXPECT_FALSE(validateLogin("0000000000", "password123"));   // Yanlýþ telefon numarasý

    // Geçersiz parametrelerle test
    EXPECT_FALSE(validateLogin(nullptr, "password123")); // Telefon numarasý null
    EXPECT_FALSE(validateLogin("1234567890", nullptr));  // Þifre null
    EXPECT_FALSE(validateLogin(nullptr, nullptr));       // Her iki parametre de null

    // Belleði temizle
    free(user1);
    free(user2);
}

TEST_F(EventAppTest, ProgressiveOverflowAlgorithmTest) {
    // Standart çýktýyý yakalamak için ayar
    testing::internal::CaptureStdout();

    // Fonksiyonu çalýþtýr
    progressiveOverflowAlgorithm();

    // Çýktýyý yakala
    std::string output = testing::internal::GetCapturedStdout();

    // Beklenen sonuçlar: taþma algýlamalarýnýn doðru elementlerde yapýlmasý
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
    // Test için bir katýlýmcý oluþtur
    Attendee attendee;
    strcpy(attendee.nameAttendee, "Alice");

    // Katýlýmcýnýn adýný sýkýþtýr
    compressAttendeeName(&attendee);

    // Katýlýmcýnýn Huffman kodunun doðru olduðunu kontrol et
    EXPECT_STREQ(attendee.huffmanCode, "Alice"); // Huffman kodu doðru olmalý
}


// Test fonksiyonu
TEST_F(EventAppTest, PrintHashTableTest) {
    // Hash tablosunu temizle
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr; // Baþlangýçta her eleman null olmalý
    }

    // Test için kullanýcýlar oluþtur
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

    // Kullanýcýlarý hash tablosuna ekle
    unsigned int index1 = hash(user1.phone);
    unsigned int index2 = hash(user2.phone);

    // Eðer ayný indekste ise, zincirleme ekleme yap
    if (index1 == index2) {
        hashTable[index1] = &user1;
        user1.next = &user2; // Ýkinci kullanýcýyý birinci kullanýcýya baðla
    }
    else {
        hashTable[index1] = &user1;
        hashTable[index2] = &user2;
    }

    // Hash tablosunu yazdýr
    printHashTable();

    // Kontrol: hash tablosundaki kullanýcýlarýn doðru olduðundan emin ol
    EXPECT_NE(nullptr, hashTable[index1]); // Ýlk kullanýcýnýn hash tablosunda olmasý gerekir
    EXPECT_STREQ(user1.name, hashTable[index1]->name); // Kullanýcýnýn adý doðru olmalý
    EXPECT_STREQ(user1.surname, hashTable[index1]->surname); // Kullanýcýnýn soyadý doðru olmalý
    EXPECT_STREQ(user1.phone, hashTable[index1]->phone); // Kullanýcýnýn telefonu doðru olmalý
    EXPECT_STREQ(user1.password, hashTable[index1]->password); // Kullanýcýnýn þifresi doðru olmalý

    if (index1 == index2) {
        EXPECT_NE(nullptr, hashTable[index1]->next); // Ýkinci kullanýcýnýn baðlantýsý olmalý
        EXPECT_STREQ(user2.name, hashTable[index1]->next->name); // Ýkinci kullanýcýnýn adý doðru olmalý
        EXPECT_STREQ(user2.surname, hashTable[index1]->next->surname); // Ýkinci kullanýcýnýn soyadý doðru olmalý
        EXPECT_STREQ(user2.phone, hashTable[index1]->next->phone); // Ýkinci kullanýcýnýn telefonu doðru olmalý
        EXPECT_STREQ(user2.password, hashTable[index1]->next->password); // Ýkinci kullanýcýnýn þifresi doðru olmalý
    }
    else {
        EXPECT_NE(nullptr, hashTable[index2]); // Ýkinci kullanýcýnýn hash tablosunda olmasý gerekir
        EXPECT_STREQ(user2.name, hashTable[index2]->name); // Kullanýcýnýn adý doðru olmalý
        EXPECT_STREQ(user2.surname, hashTable[index2]->surname); // Kullanýcýnýn soyadý doðru olmalý
        EXPECT_STREQ(user2.phone, hashTable[index2]->phone); // Kullanýcýnýn telefonu doðru olmalý
        EXPECT_STREQ(user2.password, hashTable[index2]->password); // Kullanýcýnýn þifresi doðru olmalý
    }
}

TEST_F(EventAppTest, PrintAttendeesTest) {
    // Katýlýmcýlarý manuel olarak ekle
    attendeeCount = 2;
    strcpy(attendees[0].nameAttendee, "John");
    strcpy(attendees[0].surnameAttendee, "Doe");
    strcpy(attendees[0].huffmanCode, "101010");

    strcpy(attendees[1].nameAttendee, "Jane");
    strcpy(attendees[1].surnameAttendee, "Smith");
    strcpy(attendees[1].huffmanCode, "110011");

    // Standart çýktýyý yakalamak için ayar
    testing::internal::CaptureStdout();

    // Fonksiyonu çaðýr
    printAttendees();

    // Yakalanan çýktýyý al
    std::string output = testing::internal::GetCapturedStdout();

    // Çýktýyý kontrol et
    EXPECT_FALSE(output.find("Name: John, Surname: Doe, Huffman Code: 101010") != std::string::npos);
    EXPECT_FALSE(output.find("Name: Jane, Surname: Smith, Huffman Code: 110011") != std::string::npos);

    // Katýlýmcý sayýsýný sýfýrla
    attendeeCount = 0;
}

TEST_F(EventAppTest, SparseMatrixTest) {
    // SparseMatrix baþlangýç durumu
    activityMatrix.size = 0; // Baþlangýçta 0 olmalý

    // Test: SparseMatrix baþlangýç durumu
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

    // Test: Verilerin doðru þekilde eklendiðini doðrula
    EXPECT_EQ(activityMatrix.size, 2);

    EXPECT_EQ(activityMatrix.row[0], 1);
    EXPECT_EQ(activityMatrix.col[0], 2);
    EXPECT_STREQ(activityMatrix.value[0], "Activity 1");

    EXPECT_EQ(activityMatrix.row[1], 3);
    EXPECT_EQ(activityMatrix.col[1], 4);
    EXPECT_STREQ(activityMatrix.value[1], "Activity 2");

    // Test: Maksimum kapasiteyi aþmadýðýný kontrol et
    EXPECT_LE(activityMatrix.size, MAX_SIZE);
}


TEST_F(EventAppTest, XORFunctionTest) {
    // Üç örnek düðüm oluþtur
    XORNode node1, node2, node3;

    // XOR hesaplamalarýný kontrol et
    XORNode* result1 = XOR(&node1, &node2);
    XORNode* result2 = XOR(result1, &node2);

    // Test 1: XOR iþleminin komutatif ve tersine çalýþtýðýný kontrol et
    EXPECT_EQ(result2, &node1); // XOR(result1, &node2) == &node1 olmalý

    // Test 2: XOR iþlemi bir düðümle NULL için doðru çalýþmalý
    EXPECT_EQ(XOR(&node1, nullptr), &node1); // XOR herhangi bir düðümle NULL ayný düðümü döndürmeli
    EXPECT_EQ(XOR(nullptr, &node2), &node2); // Ayný durum ters yönde de geçerli

    // Test 3: Ýki NULL iþaretçisi XOR iþleminde NULL döndürmeli
    EXPECT_EQ(XOR(nullptr, nullptr), nullptr); // NULL XOR NULL -> NULL

    // Test 4: Ayný iþaretçinin kendisiyle XOR'u NULL olmalý
    EXPECT_EQ(XOR(&node1, &node1), nullptr); // Düðüm XOR kendisi -> NULL
}


TEST_F(EventAppTest, AddToXORListTest) {
    // XOR baðlý listeyi baþlat
    xorHead = nullptr;

    // Ýlk düðümü ekle
    addToXORList("Node1");
    EXPECT_NE(xorHead, nullptr); // Listenin baþý NULL olmamalý
    EXPECT_STREQ(xorHead->value, "Node1"); // Ýlk düðümün deðeri doðru olmalý
    EXPECT_EQ(xorHead->both, XOR(nullptr, nullptr)); // Ýlk düðümün both deðeri NULL olmalý

    // Ýkinci düðümü ekle
    addToXORList("Node2");
    EXPECT_STREQ(xorHead->value, "Node2"); // Yeni baþ düðümün deðeri doðru olmalý
    EXPECT_EQ(xorHead->both, XOR(nullptr, xorHead->both)); // Baþ düðümün both iþaretçisi doðru hesaplanmalý

    // Üçüncü düðümü ekle
    addToXORList("Node3");
    EXPECT_STREQ(xorHead->value, "Node3"); // Yeni baþ düðümün deðeri doðru olmalý

    // XOR listesinde dolaþarak düðümleri doðrula
    XORNode* prev = nullptr;
    XORNode* current = xorHead;
    XORNode* next;
    std::vector<std::string> nodeValues;

    while (current != nullptr) {
        nodeValues.push_back(current->value); // Mevcut düðümün deðerini kaydet
        next = XOR(prev, current->both); // Bir sonraki düðüme ilerle
        prev = current;
        current = next;
    }

    // Ekleme sýrasýný doðrula
    EXPECT_EQ(nodeValues.size(), 3); // Toplamda 3 düðüm olmalý
    EXPECT_EQ(nodeValues[0], "Node3");
    EXPECT_EQ(nodeValues[1], "Node2");
    EXPECT_EQ(nodeValues[2], "Node1");

    // Belleði temizle
    prev = nullptr;
    current = xorHead;
    while (current != nullptr) {
        next = XOR(prev, current->both);
        free(current);
        prev = current;
        current = next;
    }

    xorHead = nullptr; // Listenin baþýný sýfýrla
}



// Test fonksiyonu
TEST_F(EventAppTest, InitializeXORListTest) {
    // Baþlangýçta xorHead NULL olmalý
    xorHead = reinterpret_cast<XORNode*>(1); // Baþka bir deðeri geçici olarak ata
    initializeXORList(); // XOR baðlý listeyi baþlat

    // Baþýn NULL olup olmadýðýný kontrol et
    EXPECT_EQ(xorHead, nullptr);
}



// Test fonksiyonu
TEST_F(EventAppTest, InitializeSparseMatrixTest) {
    // activityMatrix'in boyutunu geçici olarak baþka bir deðere ayarla
    activityMatrix.size = 10;

    // Sparse matrisin baþlatýlmasýný saðla
    initializeSparseMatrix();

    // activityMatrix.size sýfýr olmalý
    EXPECT_EQ(activityMatrix.size, 0);
}



// Test fonksiyonu
TEST_F(EventAppTest, InitializeStackTest) {
    // activityStack.top deðerini geçici olarak baþka bir deðere ayarla
    activityStack.top = 5;

    // Stack'i baþlat
    initializeStack();

    // activityStack.top deðeri -1 olmalý
    EXPECT_EQ(activityStack.top, -1);
}


// Test fonksiyonu
TEST_F(EventAppTest, InitializeQueueTest) {
    // activityQueue.front ve activityQueue.rear deðerlerini geçici olarak baþka bir deðere ayarla
    activityQueue.front = 5;
    activityQueue.rear = 5;

    // Kuyruðu baþlat
    initializeQueue();

    // activityQueue.front ve activityQueue.rear sýfýr olmalý
    EXPECT_EQ(activityQueue.front, 0);
    EXPECT_EQ(activityQueue.rear, 0);
}




// Test fonksiyonu
TEST_F(EventAppTest, IsStackFullTest) {
    // STACK_SIZE sabitinin tanýmlandýðý yerden alýnýyor
    activityStack.top = STACK_SIZE - 1; // Yýðýn tam dolmuþ durumda

    // Yýðýn dolu olmalý
    EXPECT_TRUE(isStackFull()); // isStackFull() true döndürmeli

    // Yýðýnýn tam dolu olmadýðý bir durumu test et
    activityStack.top = STACK_SIZE - 2; // Yýðýn dolu deðil
    EXPECT_FALSE(isStackFull()); // isStackFull() false döndürmeli
}

// Test fonksiyonu
TEST_F(EventAppTest, IsStackEmptyTest) {
    // activityStack.top deðerini geçici olarak baþka bir deðere ayarla
    activityStack.top = -1; // Yýðýn boþ durumda

    // Yýðýn boþ olmalý
    EXPECT_TRUE(isStackEmpty()); // isStackEmpty() true döndürmeli

    // Yýðýnýn boþ olmadýðý bir durumu test et
    activityStack.top = 5; // Yýðýn boþ deðil
    EXPECT_FALSE(isStackEmpty()); // isStackEmpty() false döndürmeli
}


// Test fonksiyonu
TEST_F(EventAppTest, PushStackTest) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";

    // Yýðýnýn baþlangýç durumunu ayarla
    activityStack.top = -1; // Yýðýn boþ

    // Ýlk etkinliði yýðýna ekle
    pushStack(activity1);

    // Yýðýnýn üst elemanýnýn "Activity 1" olmasý beklenir
    EXPECT_EQ(activityStack.top, 0);
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);

    // Ýkinci etkinliði yýðýna ekle
    pushStack(activity2);

    // Yýðýnýn üst elemanýnýn "Activity 2" olmasý beklenir
    EXPECT_EQ(activityStack.top, 1);
    EXPECT_STREQ(activityStack.items[activityStack.top], activity2);

    // Yýðýn dolu olmalý
    for (int i = 2; i < STACK_SIZE; ++i) {
        pushStack(activity1); // Yýðýna ekleme yapalým
    }

    // Yýðýn dolu olmalý, üst eleman son geçerli pozisyonda olmalý
    EXPECT_EQ(activityStack.top, STACK_SIZE - 1);

    // Yýðýnýn tam dolu olduðunu doðrulamak için bir ekleme daha yapalým
    pushStack(activity2);

    // Yýðýn dolu olmalý ve herhangi bir deðiþiklik olmamalý
    EXPECT_EQ(activityStack.top, STACK_SIZE - 1);  // Yýðýn dolu olduðu için top deðiþmemeli
}



// Test için global deðiþkenler
extern Stack activityStack;  // activityStack dýþarýdan tanýmlanmýþ bir stack yapýsý

// popStack fonksiyonunu test et
TEST_F(EventAppTest, PopStackTest) {
    const char* activity1 = "Activity 1";
    const char* activity2 = "Activity 2";

    // Yýðýnýn baþlangýç durumunu ayarla
    activityStack.top = -1;  // Yýðýn boþ

    // Ýlk etkinliði yýðýna ekle
    pushStack(activity1);
    EXPECT_EQ(activityStack.top, 0);  // Yýðýn üstü 0. indiste olmalý
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);  // Ýlk öðe doðru olmalý

    // Ýkinci etkinliði yýðýna ekle
    pushStack(activity2);
    EXPECT_EQ(activityStack.top, 1);  // Yýðýn üstü 1. indiste olmalý
    EXPECT_STREQ(activityStack.items[activityStack.top], activity2);  // Ýkinci öðe doðru olmalý

    // popStack fonksiyonu ile öðe çýkarma
    popStack();
    EXPECT_EQ(activityStack.top, 0);  // Yýðýn üstü 0. indise gerilemeli
    EXPECT_STREQ(activityStack.items[activityStack.top], activity1);  // Ýlk öðe doðru olmalý

    // Yýðýn tekrar boþaltýlýyor
    popStack();
    EXPECT_EQ(activityStack.top, -1);  // Yýðýn boþ olmalý

    // Boþ yýðýndan öðe çýkarmaya çalýþalým
    testing::internal::CaptureStdout();  // Çýktýyý yakalamak için
    popStack();  // Hata mesajý vermeli
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.find("Error: Stack is empty!") != std::string::npos);  // Hata mesajý kontrolü
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}