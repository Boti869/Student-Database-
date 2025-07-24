#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<stdbool.h>

#define INITIAL_SIZE 10
#define GENDER_MALE 'M'
#define GENDER_FEMALE 'F'
#define MAX_MAILNUM 200


typedef struct {
    long long id; 
    char name[100];
    char gender;
    int age;
    char email[50];
    char phone[20];
    char address[100];
    char className[20]; // e.g: 10.a 

} Student;

int compareStudent(const Student *a, const Student *b) {
    int classCmp = strcmp(a->className, b->className);
    if (classCmp != 0) return classCmp;
    return strcmp(a->name, b->name);
}
void bubbleSortStudents(Student *students, int count, int (*compare)(const Student *, const Student *)) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compare(&students[j], &students[j + 1]) > 0) {
                // Swap students[j] and students[j + 1]
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

int fileExists(const char *filename) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if(!d) {
        return 0; // Directory could not be opened
    }
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, filename) == 0) {
            closedir(d);
            return 1; // File exists
        }
    }
    closedir(d);
    return 0; // File does not exist
}

void printAllTxtFiles() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (!d) {
        printf("Could not open current directory.\n");
        return;
    }
    printf("Student files in directory:\n");
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".txt") != NULL) {
            printf("%s\n", dir->d_name);
        }
    }
    closedir(d);
}

int countTxtFiles() {
    DIR *d;
    struct dirent *dir;
    int count = 0;
    d = opendir(".");
    if (!d) {
        printf("Could not open current directory.\n");
        return 0;
    }
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".txt") != NULL) {
            count++;
        }
    }
    closedir(d);
    return count;
}
void searchTxtFiles(const char *searchTerm) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (!d) {
        printf("Could not open current directory.\n");
        return;
    }
    printf("Searching for '%s' in student files:\n", searchTerm);
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".txt") != NULL) {
            FILE *file = fopen(dir->d_name, "r");
            if (file) {
                char line[256];
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, searchTerm) != NULL) {
                        printf("Found in %s: %s", dir->d_name, line);
                    }
                }
                fclose(file);
            }
        }
    }
    closedir(d);
}
void exportToCSV(const Student *students, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing\n", filename);
        return;
    }
    fprintf(file, "ID,Name,Class,Age,Email,Phone,Address,Gender\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%lld,%s,%s,%d,%s,%s,%s,%c\n",
                students[i].id,
                students[i].name,
                students[i].className,
                students[i].age,
                students[i].email,
                students[i].phone,
                students[i].address,
                students[i].gender);
        }
    fclose(file);
    printf("Data exported to %s successfully.\n", filename);}
        


int main() {
   
    int capacity = INITIAL_SIZE;
    int count = countTxtFiles();

    Student* students = malloc(capacity * sizeof(Student));
    if (students == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1; // Exit if memory allocation fails
    }

    int choice;
    while (1) {
        printf("Menu:\n");
        printf("1. Add Student\n");
        printf("2. Display and search Students\n");
        printf("3. Edit Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            if (count >= capacity) {
                capacity *= 2; // Double the capacity
                students = realloc(students, capacity * sizeof(Student));
                if (students == NULL) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    return 1; // Exit if memory reallocation fails
                }
            }
            getchar(); 
            
            
            char NandC[105];
            Student newStudent;
            printf("Enter ID: ");
            scanf("%lld", &newStudent.id);
            getchar(); 
        while (1) {
            printf("Enter Name and class (e.g Dylan Doe, 10.C): ");
                fgets(NandC, sizeof(NandC), stdin);
                NandC[strcspn(NandC, "\n")] = '\0';
                char *comma = strchr(NandC, ',');
            if (comma == NULL) {
                printf("Invalid input format. Try again.\n");
                continue;
                }
                *comma = '\0';
                strcpy(newStudent.name, NandC);
                strcpy(newStudent.className, comma + 2);
                break;
                }

                printf("Enter Age: ");
            scanf("%d", &newStudent.age);
            getchar();
            printf("Enter gender (M/F): ");
            scanf(" %c", &newStudent.gender);
            getchar(); 
            
            char* MailNum = malloc(200 * sizeof(char));
            if (MailNum == NULL) {
                fprintf(stderr, "Memory allocation for MailNum failed\n");
                free(students); 
                return 1; // Exit if memory allocation fails
            }
        while(1) {
            printf("Enter Email and Phone num (mail@gmail, +1..): ");
            fgets(MailNum, MAX_MAILNUM, stdin);
                MailNum[strcspn(MailNum, "\n")] = '\0'; 
                char *comma = strchr(MailNum, ',');
                if (comma == NULL) {
                    printf("Invalid input format. Please use 'mail@gmail, +1...'.\n");
                    continue; 
                }
                *comma = '\0';
                strcpy(newStudent.email, MailNum);
                strcpy(newStudent.phone, comma + 1 ); 
                free(MailNum); 
                break;
            }
            
            getchar(); 
            printf("Enter Address: ");
            fgets(newStudent.address, sizeof(newStudent.address), stdin);
            newStudent.address[strcspn(newStudent.address, "\n")] = '\0'; 
            
            students[count++] = newStudent; // Add the new student to the array
            
            char fileStudent[100];
            char fReal[100];
            strcpy(fileStudent, newStudent.name);
            for (int i = 0; fileStudent[i]; i++) {
                if (fileStudent[i] == ' ') fileStudent[i] = '_';
            }
            sprintf(fReal, "%s_%s.txt", fileStudent, newStudent.className);
            for (int i = 0; fReal[i]; i++) {
                if (fReal[i] == ' ') fReal[i] = '_';
            } 
            char CSVfile[100];
            sprintf(CSVfile, "%s_%s.csv", fileStudent, newStudent.className);
            
            FILE *fptr = fopen(fReal, "w");
            if (fptr == NULL) {
                printf("Error opening file for writing\n");
                free(students);
                return 1; // Exit if file opening fails
            }
            
            fprintf(fptr, "==========STUDENT RECORD==========\n");
            fprintf(fptr, "Name: %s\n", newStudent.name);
            fprintf(fptr, "ID: %lld\n", newStudent.id);
            fprintf(fptr, "Class: %s\n", newStudent.className);
            fprintf(fptr, "Age: %d\n", newStudent.age);
            fprintf(fptr, "Phone: %s\n", newStudent.phone);
            fprintf(fptr, "Email: %s\n", newStudent.email);
            fprintf(fptr, "Address: %s\n", newStudent.address);
            fprintf(fptr, "Gender: %c\n", newStudent.gender);
        
            fclose(fptr);
            printf("Student added successfully and saved to file: %s\n", fReal);

            exportToCSV(students, count, CSVfile);
        
        } else if (choice == 2) {
            while (1) {
           if (count == 0) {
                printf("No students available.\n");
                continue; 
            }
            printAllTxtFiles();
            printf("\nCTRL + CLICK ON FILE NAME TO OPEN IT (only in C apps)\n");
            int exitORno;
            printf("Type 2 to open file, 1 to search or 0 to continue:");
            scanf("%d", &exitORno);
            getchar(); 
            if (exitORno == 1) {
                char searchTerm[100];
                printf("Enter search term: ");
                fgets(searchTerm, sizeof(searchTerm), stdin);
                searchTerm[strcspn(searchTerm, "\n")] = '\0'; 
                searchTxtFiles(searchTerm);
            } else if (exitORno == 0) {
                break; 
            } else if (exitORno == 2) {
                char fileToOpen[200];
                printf("Enter the student file you want to open (with '.txt'): ");
                fgets(fileToOpen, sizeof(fileToOpen), stdin);
                fileToOpen[strcspn(fileToOpen, "\n")] = '\0'; 
                if (!fileExists(fileToOpen)) {
                    printf("File %s does not exist.\n", fileToOpen);
                    continue; 
                }
                FILE *fptr = fopen(fileToOpen, "r");
                if (fptr == NULL) {
                    printf("Error opening file %s for reading\n", fileToOpen);
                    continue; 
                }
                char line[256];
                while (fgets(line, sizeof(line), fptr)) {
                    printf("%s", line); 
                }
                fclose(fptr);
            } else {
                printf("Invalid input. Please enter 2 toopen file, 1 to search or 0 to continue.\n");
            }
            
            }
        }  else if (choice == 3) {

            if (count == 0) {
                printf("No students available to edit.\n");
                continue; 
            }
            char *temp_ToEdit = malloc(200 * sizeof(char));
            char* ToEdit = malloc(200 * sizeof(char));
            printAllTxtFiles();
            printf("Enter the student file you want to edit (with '.txt'): ");
            fgets(ToEdit, 200, stdin);
            ToEdit[strcspn(ToEdit, "\n")] = '\0'; 
            if (!fileExists(ToEdit)) {
                printf("File %s does not exist.\n", ToEdit);
                free(ToEdit);
                free(temp_ToEdit);
                continue; 
            }
            strcpy(temp_ToEdit, "temp___");
            strcat(temp_ToEdit, ToEdit);
            
            FILE *fptr = fopen(ToEdit, "r");
            FILE *temp_fptr = fopen(temp_ToEdit, "w");
            if (fptr == NULL || temp_fptr == NULL) {
                printf("Error opening file %s for reading or %s for writing\n", ToEdit, temp_ToEdit);
                free(ToEdit);
                free(temp_ToEdit);
                continue; // Skip to the next iteration if file cannot be opened
            }

            char line[256];
            while(fgets(line, sizeof(line), fptr)){
                printf("%s", line); 
            } 
        
            printf("Enter line number to edit (1-based index): ");
            int lineNumber;
            scanf("%d", &lineNumber);
            getchar(); 

            printf("Enter new content for line %d: ", lineNumber);
            char newContent[256];
            fgets(newContent, sizeof(newContent), stdin);
            newContent[strcspn(newContent, "\n")] = '\0'; 

            bool keep_reading = true;
            int currentLine = 1;
            char buffer[256];
            rewind(fptr); // Reset file pointer to the beginning
            do{
                fgets(buffer, sizeof(buffer), fptr);

                if(feof(fptr)) keep_reading = false;
                else if(currentLine == lineNumber) {
                    fprintf(temp_fptr, "%s\n", newContent);
                }
                else {
                    fputs(buffer, temp_fptr);
                }
                currentLine++;
            
            }while (keep_reading == true);        
        
            fclose(fptr);
            fclose(temp_fptr);

            remove(ToEdit);
            rename(temp_ToEdit, ToEdit);

            printf("File %s edited successfully.\n", ToEdit);

        
        } else if (choice == 4) {
            if (count == 0) {
                printf("No students available to delete.\n");
                continue; 
            }
            while (1) {
            char fileToDelete[200];
            printAllTxtFiles();
            printf("Enter the student file you want to delete (with '.txt') or type 'exit': ");
            fgets(fileToDelete, sizeof(fileToDelete), stdin);
            fileToDelete[strcspn(fileToDelete, "\n")] = '\0';
            if (strcmp(fileToDelete, "exit") == 0) {
                break; 
            }
            if(!fileExists(fileToDelete))
            {
                printf("File %s doesn not exist.\n", fileToDelete);
                continue; 
            }
            else {
                char confirmation;
                printf("Are you sure you want to delete %s? (y/n): ", fileToDelete);
                scanf(" %c", &confirmation);
                getchar();
                if (confirmation == 'y' || confirmation == 'Y') {
                    if (remove(fileToDelete) == 0) {
                        printf("File %s deleted successfully.\n", fileToDelete);
                        count = countTxtFiles();
                        break;
                    } else {
                        printf("Error deleting file %s.\n", fileToDelete);
                        continue; 
                    }
                }
            }
        }
        
        
        } else if (choice == 5) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            
        }
    

    free(students);
    }
    free(students);
    return 0;
    }