#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    char name[50];
    int roll_no;
    float marks;
};

// Function to encrypt student data
void encryptData(char *data) {
    while (*data) {
        *data = *data + 3; // Simple Caesar Cipher Encryption
        data++;
    }
}

// Function to decrypt student data
void decryptData(char *data) {
    while (*data) {
        *data = *data - 3; // Reversing the encryption
        data++;
    }
}

// Password check for admin access
int checkPassword() {
    char password[20];
    printf("\n🔒 Enter admin password: ");
    scanf("%s", password);
    return strcmp(password, "admin123") == 0; // Change password as needed
}

void addStudent() {
    if (!checkPassword()) {
        printf("❌ Access Denied!\n");
        return;
    }

    FILE *file = fopen("students.bin", "ab"); // Binary file storage
    struct Student s;
    printf("Enter Name, Roll No, Marks: ");
    scanf("%s %d %f", s.name, &s.roll_no, &s.marks);
    encryptData(s.name); // Encrypt name before saving
    fwrite(&s, sizeof(struct Student), 1, file);
    fclose(file);
    printf("✅ Student added successfully!\n");
}

void viewStudents() {
    FILE *file = fopen("students.bin", "rb");
    if (!file) {
        printf("⚠ No records found!\n");
        return;
    }
    struct Student s;
    printf("\n📌 Student Records:\n");
    while (fread(&s, sizeof(struct Student), 1, file)) {
        decryptData(s.name); // Decrypt name for viewing
        printf("Name: %s | Roll No: %d | Marks: %.2f\n", s.name, s.roll_no, s.marks);
    }
    fclose(file);
}

void searchStudent() {
    int rollNo;
    printf("Enter Roll No to search: ");
    scanf("%d", &rollNo);
    FILE *file = fopen("students.bin", "rb");
    struct Student s;
    int found = 0;
    while (fread(&s, sizeof(struct Student), 1, file)) {
        decryptData(s.name);
        if (s.roll_no == rollNo) {
            printf("✅ Student Found: Name: %s | Marks: %.2f\n", s.name, s.marks);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) printf("❌ Student not found!\n");
}

void deleteStudent() {
    if (!checkPassword()) {
        printf("❌ Access Denied!\n");
        return;
    }

    int rollNo;
    printf("Enter Roll No to delete: ");
    scanf("%d", &rollNo);
    FILE *file = fopen("students.bin", "rb");
    FILE *temp = fopen("temp.bin", "wb");
    struct Student s;
    int found = 0;
    while (fread(&s, sizeof(struct Student), 1, file)) {
        if (s.roll_no != rollNo) {
            fwrite(&s, sizeof(struct Student), 1, temp);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp);
    remove("students.bin");
    rename("temp.bin", "students.bin");

    if (found)
        printf("✅ Student deleted successfully!\n");
    else
        printf("❌ Student not found!\n");
}

// Sorting Students by Marks
void sortStudents() {
    FILE *file = fopen("students.bin", "rb");
    if (!file) {
        printf("⚠ No records found!\n");
        return;
    }

    struct Student students[100];
    int count = 0;
    while (fread(&students[count], sizeof(struct Student), 1, file)) {
        decryptData(students[count].name);
        count++;
    }
    fclose(file);

    // Sorting by Marks (Bubble Sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].marks < students[j + 1].marks) {
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    // Display sorted records
    printf("\n📌 Sorted Student Records (By Marks):\n");
    for (int i = 0; i < count; i++) {
        printf("Name: %s | Roll No: %d | Marks: %.2f\n", students[i].name, students[i].roll_no, students[i].marks);
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n🔹 Student Record System 🔹\n");
        printf("1️⃣ Add Student\n2️⃣ View Students\n3️⃣ Search Student\n4️⃣ Delete Student\n5️⃣ Sort Students by Marks\n6️⃣ Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: sortStudents(); break;
            case 6: printf("🚀 Exiting...\n"); return 0;
            default: printf("⚠ Invalid choice! Try again.\n");
        }
    }
}