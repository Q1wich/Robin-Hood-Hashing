#include <stdlib.h>
#include <stdio.h>
#include <math.h>
struct HashInfo {
    int psl = 0;
    char name_key[20];
    char surname[20];
    short int birthDate;
    short int deathDate;
};
typedef HashInfo Info;



unsigned int HashSize = 17;
int coefficientOfLoad = 0;


unsigned hash(const char* str);
void copyData(Info* node, const Info* data);
bool keyCompare(const char* key1, const char* key2);
Info createElement();
Info* insert(Info* hash_table, Info data);
void findByName(Info* ptr, Info data, char key[]);
Info findElement(Info* hash_table, const char* key);
void deleteByName(Info* ptr, char key[]);
int deleteElement(Info* hash_table, const char* key);
void printHashTable(const Info* hash_table);
int findNextPrime();
Info* rebuildHashTable(Info** hash_table);
void printMenu();
Info* LoadFile(Info* hash_table);



int main(void) {
    Info* hash_table = (Info*)malloc(HashSize * sizeof(Info));
    for (int i = 0; i < 17; i++) {
        hash_table[i].psl = 0;
        hash_table[i].name_key[0] = '\0';
    }
    int choice = 0;
    int res = 0;
    char key[20];
    Info* ptr;
    ptr = hash_table;
    Info data;
    data.psl = 0;
    data.name_key[0] = '\0';
    do {
        printMenu();
        printf("\nEnter your choice: ");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1:
            getchar();
            data = createElement();
            ptr = insert(ptr, data);
            printf("Element added.\n\n");
            break;
        case 2:
            getchar();
            findByName(ptr, data, key);
            break;
        case 3:
            getchar();
            deleteByName(ptr, key);
            break;
        case 4:
            printHashTable(ptr);
            break;
        case 5:
            ptr = LoadFile(ptr);
            break;
        case 6:
            printf("Ending...");
        default:
            printf("Wrong choice.Try again:\n\n");
            break;
        }
    } while (choice != 6);
    free(ptr);
    return 0;
}



unsigned int hash(const char* key) {
    unsigned int hash_value = 7;
    while (*key) {
        hash_value = hash_value * 31 + *key++;
    }
    return hash_value % HashSize;
}
void copyData(Info* node, const Info* data) {
    int i = 0;
    while (data->name_key[i] != '\0') {
        node->name_key[i] = data->name_key[i];
        i++;
    }
    node->name_key[i] = '\0';

    i = 0;
    while (data->surname[i] != '\0') {
        node->surname[i] = data->surname[i];
        i++;
    }
    node->surname[i] = '\0';

    node->birthDate = data->birthDate;
    node->deathDate = data->deathDate;
    node->psl = data->psl;
}
bool keyCompare(const char* key1, const char* key2) {
    int i = 0;
    while (*(key1 + i) == *(key2 + i) && *(key1 + i) != '\0' && *(key2 + i) != '\0') {
        i++;
    }
    if (*(key1 + i) != '\0' || *(key2 + i) != '\0') return false;
    return true;
}
Info createElement() {
    Info data;
    printf("Enter name: ");
    gets_s(data.name_key);
    printf("Enter surname: ");
    gets_s(data.surname);
    printf("Enter year of birth: ");
    scanf_s("%hd", &data.birthDate);
    printf("Enter year of death: ");
    scanf_s("%hd", &data.deathDate);
    return data;
}
Info* insert(Info* ptr, Info data) {
    coefficientOfLoad++;
    if ((float)coefficientOfLoad / HashSize >= 0.75) {
        ptr = rebuildHashTable(&ptr);
    }
    unsigned int index = hash(data.name_key);
    data.psl = 0;
    while (ptr[index].name_key[0] != '\0') {
        if (data.psl > ptr[index].psl) {
            Info a;
            copyData(&a, &ptr[index]);
            copyData(&ptr[index], &data);
            copyData(&data, &a);
        }
        data.psl++;
        index++;
        if (index > HashSize) {
            ptr = rebuildHashTable(&ptr);
        }
    }
    copyData(&ptr[index], &data);
    return ptr;
}
void findByName(Info* ptr, Info data, char key[]) {
    printf("Enter name: ");
    gets_s(key, 20);
    data = findElement(ptr, key);
    if (data.name_key[0] == '1') {
        printf("There is no such element\n");
    }
    else {
        printf("\nNme: %s\n", data.name_key);
        printf("Surname: %s\n", data.surname);
        printf("Year of birth: %d\n", data.birthDate);
        printf("Year of death: %d\n", data.deathDate);
        printf("PSL: %d\n", data.psl);
        printf("\n");
    }
}
Info findElement(Info* hash_table, const char* key) {
    unsigned int index = hash(key);
    int keyPsl = 0;
    while (hash_table[index].name_key[0] != '\0' && hash_table[index].psl <= keyPsl) {
        if (keyCompare(hash_table[index].name_key, key)) return hash_table[index];
        index++;
        keyPsl++;
    }
    Info a;
    a.name_key[0] = '1';
    return a;
}
void deleteByName(Info* ptr, char key[]) {
    printf("Enter name: ");
    gets_s(key, 20);
    int res = deleteElement(ptr, key);
    if (res == 0) {
        printf("Element deleted successfully\n");
    }
    else {
        printf("Something went wrong\n");
    }
}
int deleteElement(Info* hash_table, const char* key) {
    unsigned int index = hash(key);
    while (hash_table[index].name_key[0] != '\0') {
        if (keyCompare(hash_table[index].name_key, key)) {
            hash_table[index].name_key[0] = '\0';
            int nextIndex = index + 1;
            while (hash_table[nextIndex].name_key[0] != '\0' && hash_table[nextIndex].psl > 0) {
                copyData(&hash_table[index], &hash_table[nextIndex]);
                hash_table[index].psl--;
                index = nextIndex;
                nextIndex++;
            }
            hash_table[index].name_key[0] = '\0';
            coefficientOfLoad--;
            return 0;
        }
        index++;
    }
    return -1;
}
void printHashTable(const Info* ptr) {
    int amountOfCollisions = 0;
    printf("Hash table:\n");
    for (unsigned int i = 0; i < HashSize; i++) {
        if (ptr[i].name_key[0] != '\0') {
            printf("Index %d:\n", i);
            printf("Name: %s\n", ptr[i].name_key);
            printf("Surename: %s\n", ptr[i].surname);
            printf("Year of birth: %d\n", ptr[i].birthDate);
            printf("Year of death: %d\n", ptr[i].deathDate);
            printf("PSL: %d\n", ptr[i].psl);
            printf("\n");
            amountOfCollisions += ptr[i].psl;
        }
    }
    printf("Total collisions amount: %d\n", amountOfCollisions);
}
void printMenu() {
    printf("\n1.Add element");
    printf("\n2.Find element");
    printf("\n3.Delete element");
    printf("\n4.Print table");
    printf("\n5.Load data from file");
    printf("\n6.Close program");
}
int findNextPrime() {
    int prime = HashSize * 2;
    prime++;
    while (true) {
        bool is_prime = true;
        int sqrt_next = sqrt(prime);
        for (int i = 2; i <= sqrt_next; i++) {
            if (prime % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            break;
        }
        prime++;
    }
    return prime;
}
Info* rebuildHashTable(Info** hash_table) {
    coefficientOfLoad = 0;
    int oldHashSize = HashSize;
    HashSize = findNextPrime();
    Info* new_hash_table = (Info*)malloc(HashSize * sizeof(Info));
    for (int i = 0; i < HashSize; i++) {
        new_hash_table[i].psl = 0;
        new_hash_table[i].name_key[0] = '\0';
    }
    for (int i = 0; i < oldHashSize; i++) {
        if ((*hash_table)[i].name_key[0] != '\0') {
            insert(new_hash_table, (*hash_table)[i]);
        }
    }
    free(*hash_table);
    *hash_table = new_hash_table;

    return *hash_table;
}
Info* LoadFile(Info* ptr) {
    FILE* fp;
    char fname[100];

    printf("Enter path(format: Disk name:\\\\folder name\\\\file name.txt): \n");
    scanf_s("%99s", fname, (unsigned)_countof(fname));

    errno_t err = fopen_s(&fp, fname, "r");
    if (err != 0) {
        printf("File opening eror %s\n", fname);
        return ptr;
    }

    Info data;
    data.psl = 0;
    data.name_key[0] = '\0';
    int i = 0;
    int j = 0;
    char buffer[200];

    while (fgets(buffer, 200, fp) != NULL) {
        i = 0;
        j = 0;
        while (buffer[i] != '-') {
            data.name_key[i] = buffer[i];
            i++;
        }
        data.name_key[i] = '\0';
        i++;
        while (buffer[i] != '-') {
            data.surname[j] = buffer[i];
            j++;
            i++;
        }
        data.surname[j] = '\0';
        i++;
        data.birthDate = 0;
        while (buffer[i] != '-') {
            data.birthDate *= 10;
            data.birthDate += buffer[i] - '0';
            i++;
        }
        i++;
        data.deathDate = 0;
        while (buffer[i] != '\n') {
            data.deathDate *= 10;
            data.deathDate += buffer[i] - '0';
            i++;
        }
        ptr = insert(ptr, data);
    }

    fclose(fp);
    data.psl = 0;
    data.name_key[0] = '\0';
    printf("The data has been successfully loaded from the file %s\n", fname);
    return ptr;
}
