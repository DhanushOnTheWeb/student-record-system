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
        *data ^= 3; // Simple XOR encryption
        data++;
    }
}

// Function to decrypt student data
void decryptData(char *data) {
    while (*data) {
        *data ^= 3; // Reverse XOR encryption
        data++;
    }
}

// Password check for admin access
int checkPassword() {
    char password[20];
    printf("\nEnter admin password: ");
    scanf("%19s", password); // Prevent buffer overflow
    return strcmp(password, "admin123") == 0;
}

void addStudent() {
    if (!checkPassword()) {
        printf("Access Denied!\n");
        return;
    }

    FILE *file = fopen("students.bin", "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    printf("Enter Name, Roll No, Marks: ");
    scanf("%s %d %f", s.name, &s.roll_no, &s.marks);
    encryptData(s.name);
    fwrite(&s, sizeof(struct Student), 1, file);
    fclose(file);
    printf("Student added successfully!\n");
}

void viewStudents() {
    FILE *file = fopen("students.bin", "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    struct Student s;
    printf("\nStudent Records:\n");
    while (fread(&s, sizeof(struct Student), 1, file)) {
        decryptData(s.name);
        printf("Name: %s | Roll No: %d | Marks: %.2f\n", s.name, s.roll_no, s.marks);
    }
    fclose(file);
}

void searchStudent() {
    int rollNo;
    printf("Enter Roll No to search: ");
    scanf("%d", &rollNo);
    FILE *file = fopen("students.bin", "rb");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    int found = 0;
    while (fread(&s, sizeof(struct Student), 1, file)) {
        decryptData(s.name);
        if (s.roll_no == rollNo) {
            printf("Student Found: Name: %s | Marks: %.2f\n", s.name, s.marks);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) printf("Student not found!\n");
}

void deleteStudent() {
    if (!checkPassword()) {
        printf("Access Denied!\n");
        return;
    }

    int rollNo;
    printf("Enter Roll No to delete: ");
    scanf("%d", &rollNo);
    FILE *file = fopen("students.bin", "rb");
    FILE *temp = fopen("temp.bin", "wb");
    if (!file || !temp) {
        printf("Error opening files!\n");
        return;
    }

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
        printf("Student deleted successfully!\n");
    else
        printf("Student not found!\n");
}

// Sorting Students by Marks Using `qsort`
int compareMarks(const void *a, const void *b) {
    return ((struct Student*)b)->marks - ((struct Student*)a)->marks;
}

void sortStudents() {
    FILE *file = fopen("students.bin", "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    struct Student students[100];
    int count = 0;
    while (fread(&students[count], sizeof(struct Student), 1, file)) {
        decryptData(students[count].name);
        count++;
    }
    fclose(file);

    qsort(students, count, sizeof(struct Student), compareMarks);

    printf("\nSorted Student Records (By Marks):\n");
    int i;
    for ( i = 0; i < count; i++) {
        printf("Name: %s | Roll No: %d | Marks: %.2f\n", students[i].name, students[i].roll_no, students[i].marks);
    }
}

int main() {
    int choice;
    while (1) {
        printf("\nStudent Record System\n");
        printf("1. Add Student\n2. View Students\n3. Search Student\n4. Delete Student\n5. Sort Students by Marks\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: sortStudents(); break;
            case 6: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}
