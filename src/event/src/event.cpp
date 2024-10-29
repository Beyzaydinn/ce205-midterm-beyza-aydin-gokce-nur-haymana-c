#include "../header/event.h"
#include <stdexcept>
#include <string.h>
#include <stdio.h>

typedef struct {
    char name[50];
    char surname[50];
    char phone[20];
    char password[20];
} User;

void saveUserData(User user);
bool validateLogin(const char* phone, const char* password);
bool mainMenu();
bool authentication();
bool Register();
bool logIn();
bool guest();
bool eventDetails();
bool attendee();
bool schedule();
bool feedback();

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
        exit(0); // Programý sonlandýrmak için exit kullanýlýr.
    default:
        printf("Invalid choice. Please try again.\n");
        return false; // Geçersiz seçim durumunda false döner.
    }

    return true; // Geçerli bir seçenek olduðunda true döner.
}

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
            printf("Login successful!\n");
            // Kullanýcý giriþ yaptýktan sonra diðer fonksiyonlarý burada çaðýrabilirsiniz
        }
        else {
            printf("Invalid login. Returning to main menu.\n");
            return mainMenu();
        }
        break;
    case 3:
        guest();
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        return false; // Geçersiz seçim durumunda false döner.
    }
    return true;
}

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

    // Kullanýcý verilerini dosyaya kaydet
    saveUserData(newUser);
    printf("Registration successful! You can now log in.\n");
    return mainMenu();
}

void saveUserData(User user) {
    FILE* file = fopen("users.dat", "ab"); // Dosyayý ekleme modunda aç
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fwrite(&user, sizeof(User), 1, file); // Kullanýcý verisini dosyaya yaz
    fclose(file);
}

bool logIn() {
    char phone[20];
    char password[20];
    printf("Enter your phone number: ");
    scanf("%s", phone);
    printf("Enter your password: ");
    scanf("%s", password);

    // Kullanýcý doðrulamasýný yap
    return validateLogin(phone, password);
}

bool validateLogin(const char* phone, const char* password) {
    User user;
    FILE* file = fopen("users.dat", "rb"); // Dosyayý okuma modunda aç
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    while (fread(&user, sizeof(User), 1, file) == 1) { // Dosyadan kullanýcý verilerini oku
        if (strcmp(user.phone, phone) == 0 && strcmp(user.password, password) == 0) {
            fclose(file);
            return true; // Giriþ baþarýlý
        }
    }
    fclose(file);
    return false; // Giriþ baþarýsýz
}

// Misafir giriþi fonksiyonu
bool guest() {
    printf("Guest login successful!\n");
    return true;
}

bool eventDetails() {
    // Etkinlik detaylarý ile ilgili iþlemler
    return true;
}

bool attendee() {
    // Katýlýmcý yönetimi iþlemleri
    return true;
}

bool schedule() {
    // Zaman çizelgesi düzenleme iþlemleri
    return true;
}

bool feedback() {
    // Geri bildirim toplama iþlemleri
    return true;
}
