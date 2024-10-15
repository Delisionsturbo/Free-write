#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

static char dir[100];

static void clearScreen() {
    system("cls");
}

static void Dirload() {
    printf("\nEnter file directory: ");

    if (fgets(dir, sizeof(dir), stdin) == NULL) {
        printf("Error loading file directory.\n");
        return;
    }
    dir[strcspn(dir, "\n")] = 0;

    printf("File directory: %99s", dir);
    (void)getchar();
}

static void Edit() {
    if (strlen(dir) == 0) {
        printf("Error: No directory loaded. Please load a directory first.\n");
        return;
    }

    char name[100];
    char fullPath[200];

    printf("Enter file name to edit: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Name column is null.\n");
        return;
    }

    name[strcspn(name, "\n")] = 0; // Удаление символа новой строки
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", dir, name);

    FILE* file = fopen(fullPath, "w");
    if (file == NULL) {
        printf("Error loading file.\n");
        return;
    }

    char put[100];
    printf("\nEnter data to save it in %s (MAX 100 SYMBOLS): ", fullPath);
    if (fgets(put, sizeof(put), stdin) != NULL) {
        put[strcspn(put, "\n")] = 0; // Удаление символа новой строки
        fputs(put, file);
        printf("Data saved successfully.\n");
    }
    else {
        printf("Error reading input.\n");
    }
    fclose(file);
}

static void Read() {
    if (strlen(dir) == 0) {
        printf("Error: No directory loaded. Please load a directory first.\n");
        return;
    }

    char name[100];
    char fullPath[200];

    printf("Enter file name to read: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Name column is null.\n");
        return;
    }

    name[strcspn(name, "\n")] = 0; // Удаление символа новой строки
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", dir, name);

    char buffer[100];

    FILE* file = fopen(fullPath, "r");
    if (file == NULL) {
        printf("Error loading file.\n");
        return;
    }

    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("File data: %s\n", buffer);
    }
    else {
        printf("Error reading file or file is empty.\n");
    }
    fclose(file);
}

static void Info() {
    if (strlen(dir) == 0) {
        printf("Error: No directory loaded. Please load a directory first.\n");
        return;
    }

    char name[100];
    char fullPath[200];

    printf("Enter file name to get info: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Error reading file name.\n");
        return;
    }

    name[strcspn(name, "\n")] = 0;
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", dir, name);

    struct stat fileInfo;
    struct tm tm_info;
    char timeBuffer[26];

    if (stat(fullPath, &fileInfo) == 0) {
        const char* fileName = strrchr(fullPath, '\\');

        if (fileName != NULL) {
            fileName++;
        }
        else {
            fileName = fullPath;
        }

        localtime_s(&tm_info, &fileInfo.st_ctime);
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &tm_info);

        printf("File name: %s\nFile size: %ld bytes\nFile creation date: %s\n", fileName, fileInfo.st_size, timeBuffer);
    }
    else {
        printf("Error printing file info. Maybe you forgot to load the file directory? Use the 1st function.\n");
    }
}

static void Conf() {
    FILE* file = fopen("config.txt", "w");

    printf("If you wanna add file directory in config to save and use it automaticly after restart tool type it below\nYou also can clean confing by entering 10 also below\n");
    printf("-> ");

    if (fgets(dir, sizeof(dir), stdin) == NULL) {
        printf("Error loading file directory\n");
        return;
    }

    dir[strcspn(dir, "\n")] = 0;

    if (file == NULL) {
        printf("Tool config file is damaged! You can create the config.txt file manually if it doesnt exist.\n");
        return;
    }

    fprintf(file, "%s\n", dir);
    fclose(file);

    printf("Directory saved as: %s\n", dir);
}

static void LoadConf() {
    FILE* file = fopen("config.txt", "r");

    if (file == NULL) {
        printf("Tool config file damaged! You can create config.txt file manually if it didnt exist or re-create if exists.");
        return;
    }

    if (fgets(dir, sizeof(dir), file) != NULL) {
        size_t len = strlen(dir);
        if (len > 0 && dir[len - 1] == '\n') {
            dir[len - 1] = '\0';
        }
    } // <-- Here, via else function
}

static void CleanConf() {
    FILE* file = fopen("config.txt", "w");
    
    if (file == NULL) {
        printf("Tool config file damaged! You can create config.txt file manually if it didnt exist or re-create if exists.");
        return;
    }

    fclose(file);
    printf("Config cleaned!\n");
}

static void CreateFile() {
    char name[100] = { 0 };
    char fullPath[200];

    printf("Enter file name: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Name column is null.\n");
        return;
    }

    name[strcspn(name, "\n")] = 0;
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", dir, name);

    FILE* file = fopen(fullPath, "a");
    if (file == NULL) {
        printf("File creation failed.\n");
        return;
    }

    fprintf(file, "Example text.\n");
    fclose(file);
    printf("File created successfully at %s\n", fullPath);
}

int main() {
    char input[10];
    int func;

    while (1) {
        clearScreen();
        LoadConf(); // Do error log maybe?

        printf(" ___ ___ ___ _____      _____ ___ _____ ___    \n");
        printf("| __| _ \\ __| __\\ \\    / / _ \\_ _|_   _| __|\n");
        printf("| _||   / _|| _| \\ \\/\\/ /|   /| |  | | | _|\n");
        printf("|_| |_|_\\___|___| \\_/\\_/ |_|_\\___| |_| |___|\n");

        printf("\n\n1. Load directory\n2. Edit file\n3. Read file\n4. Get file info\n5. Config editor\n6. Clean config\n7. File creator\n\n-> ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }

        func = atoi(input);

        switch (func) {
        case 1:
            Dirload();
            break;
        case 2:
            Edit();
            break;
        case 3:
            Read();
            break;
        case 4:
            Info();
            break;
        case 5:
            Conf();
            break;
        case 6:
            CleanConf();
            break;
        case 7:
            CreateFile();
            break;
        default:
            printf("Invalid function selected, try again.\n");
            break;
        }

        printf("Press Enter to continue...");
        (void)getchar();
    }
    return 0;
}

// FreeWrite v1.0 (Final, for public) //