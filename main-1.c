#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME "D:\\mengoding\\cpp\\tugas\\output\\output.txt" //change as where your file is located


// Define the maximum number of users
#define MAX_USERS 100


// Define the structure for a user
struct User {
    int id;
    char name[50];
    float costPerDay;
    float totalCost;
    int dueDate[3];
};

// Function prototypes
void addUser(struct User users[], int *numUsers);
void removeUser(struct User users[], int *numUsers, int userId);
void editUser(struct User users[], int *numUsers, int userId);
float totalCost(struct User users[], int numUsers, int d[6]);
void printUsers(struct User users[], int numUsers);
void readFromFile(struct User users[], int *numUsers);
void saveToFile(struct User users);


int main() {
    struct User users[MAX_USERS];
    int numUsers = 0;
    int  cur;
    char s[20];
    int d[6]; //DD1, MM1, YY1, DD2, MM2, YY2
            readFromFile(users, &numUsers);
    while(1){
        printf("********************************\n1. print user\n2. add user\n3. edit user\n4. remove user\n5. print total cost\n0. Exit\n********************************\nMasukkan opsi : ");
        scanf("%d", &cur);
        if(cur ==  0) break;
        switch(cur){
            case 1 : printUsers(users, numUsers); break;
            case 2 : addUser(users, &numUsers); break;
            case 3 : printf("masukkan id: "); scanf("%d", &cur); editUser(users, &numUsers, cur);  break;
            case 4 : printf("masukkan id: "); scanf("%d", &cur); removeUser(users, &numUsers, cur); break;
            case 5 : printf("masukkan tanggal  sekarang DD-MM-YYYY "); scanf("%d-%d-%d", &d[3], &d[4], &d[5]); printf("%.2f\n", totalCost(users, numUsers, d)); break;
            case 0 : printUsers(users,  numUsers); break;
            default : printf("masukkan angka  yang  benar!\n");
        }
    }



    return 0;
}

void saveToFile(struct User user) {
    FILE* file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Error opening the file for writing");
        return;
    }

    fprintf(file, "%d %s %.2f %.2f %d-%d-%d\n", user.id, user.name, user.costPerDay, user.totalCost, user.dueDate[0], user.dueDate[1], user.dueDate[2]);

    fclose(file);
    printf("Data saved to file successfully.\n");
}

int isExist(struct User users[], int numUsers, int id) {
    for (int i = 0; i < numUsers; i++) {
        if (users[i].id == id) {
            return 1;  // ID already exists
        }
    }
    return 0;  // ID not found
}
void readFromFile(struct User users[], int* numUsers) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening the file for reading");
        return;
    }

    *numUsers = 0;

    while (*numUsers < MAX_USERS && fscanf(file, "%d %s %f %f %d-%d-%d", &users[*numUsers].id, users[*numUsers].name, &users[*numUsers].costPerDay, &users[*numUsers].totalCost, &users[*numUsers].dueDate[0], &users[*numUsers].dueDate[1], &users[*numUsers].dueDate[2]) == 7 && !isExist(users, *numUsers, users[*numUsers].id) ) {
        (*numUsers)++;
    }

    if (feof(file)) {
        // End of file reached, not an error
        printf("End of file reached.\n");
    } else if (ferror(file)) {
        perror("Error reading from file");
    }

    fclose(file);
}


void printUsers(struct User users[], int numUsers) {
    printf("\ndaftar pengguna:\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-10s | %-10s |\n", "ID", "nama", "perhari", "total", "tanggal");
    printf("------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < numUsers; i++) {
        printf("| %-5d | %-20s | %-10.2f | %-10.2f | %d-%d-%d |\n", users[i].id, users[i].name, users[i].costPerDay, users[i].totalCost, users[i].dueDate[0], users[i].dueDate[1], users[i].dueDate[2]);
    }
    
    printf("--------------------------------------------------\n");
}

void addUser(struct User users[], int *numUsers) {
    if (*numUsers < MAX_USERS) {
        struct User newUser;
        newUser.id = *numUsers + 1;

        printf("Masukkan  nama: ");
        scanf("%s", newUser.name);

        printf("Masukkan biaya harian: ");
        scanf("%f", &newUser.costPerDay);

        newUser.totalCost = 0; // Initialize total cost
        printf("masukkan tanggal jatuh tempo (DD-MM-YYYY): ");
        scanf("%d-%d-%d", &newUser.dueDate[0], &newUser.dueDate[1], &newUser.dueDate[2]);

        users[*numUsers] = newUser;
        saveToFile(users[*numUsers]);
        (*numUsers)++;
        printf("user  berhasil di tambahkan.\n");
    } else {
        printf("Jumlah maksimal user terlah tercapai.\n");
    }
}

void removeUser(struct User users[], int* numUsers, int userId) {
    int foundIndex = -1;

    for (int i = 0; i < *numUsers; i++) {
        if (users[i].id == userId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        // Open the file for writing
        FILE* file = fopen(FILENAME, "w");
        if (file == NULL) {
            perror("Error opening the file for writing");
            return;
        }
        // Remove the user from the array
        for (int i = foundIndex; i < *numUsers - 1; i++) {
            users[i] = users[i + 1];
        }
        // Write the updated array to the file
        for (int i = 0; i < *numUsers; i++) {
            fprintf(file, "%d %s %.2f %.2f %d-%d-%d\n", users[i].id, users[i].name, users[i].costPerDay, users[i].totalCost, users[i].dueDate[0], users[i].dueDate[1], users[i].dueDate[2]);
        }

        // Close the file
        fclose(file);

       

        printf("User berhasil dihapus.\n");
        (*numUsers)--;
    } else {
        printf("User tidak ditemukan.\n");
    }
}

void editUser(struct User users[], int *numUsers, int userId) {
    int foundIndex = -1;
    char  s[50];
    for (int i = 0; i < *numUsers; i++) {
        if (users[i].id == userId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        printf("masukkan nama baru: ");
        scanf("%s", users[foundIndex].name);

        printf("masukkan biaya perhari: ");
        scanf("%f", &users[foundIndex].costPerDay);

        printf("masukkan jatuh tempo (DD-MM-YYYY): ");
        scanf("%d-%d-%d", &users[foundIndex].dueDate[0], &users[foundIndex].dueDate[1], &users[foundIndex].dueDate[2]);

        // Write the updated array to the file
        FILE* file =  fopen(FILENAME, "w");
        for (int i = 0; i < *numUsers; i++) {
            fprintf(file, "%d %s %.2f %.2f %d-%d-%d\n", users[i].id, users[i].name, users[i].costPerDay, users[i].totalCost, users[i].dueDate[0], users[i].dueDate[1], users[i].dueDate[2]);
        }

        // Close the file
        fclose(file);
        printf("User berhasil di edit.\n");
    } else {
        printf("user tidak ditemukan.\n");
    }
}

float totalCost(struct User users[], int numUsers, int d[]) {

    float total = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        users[i].totalCost += abs(((users[i].dueDate[0] - d[3]) + (users[i].dueDate[1] - d[4]) * 30 + (users[i].dueDate[2] - d[5]) * 365)) * users[i].costPerDay;
    }

    return total;
}
