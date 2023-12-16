#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char kode[50];
    char nama_sekolah[100];
    char telepon[20];
    int jumlah_siswa;
    struct Node* next;
};

struct Node* createNode(char kode[], char nama_sekolah[], char telepon[], int jumlah_siswa) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->kode, kode);
    strcpy(newNode->nama_sekolah, nama_sekolah);
    strcpy(newNode->telepon, telepon);
    newNode->jumlah_siswa = jumlah_siswa;
    newNode->next = NULL;
    return newNode;
}

struct Node* addAndSave(struct Node* head, char kode[], char nama_sekolah[], char telepon[], int jumlah_siswa, const char* filename) {
    struct Node* newNode = createNode(kode, nama_sekolah, telepon, jumlah_siswa);
    newNode->next = head;

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        free(newNode);
        return head;
    }

    fprintf(file, "%s,%s,%s,%d\n", kode, nama_sekolah, telepon, jumlah_siswa);
    fclose(file);

    printf("Data added and saved to %s\n", filename);
    return newNode;
}

void searchNode(struct Node* head, char kode[]) {
    struct Node* current = head;
    while (current != NULL) {
        if (strcmp(current->kode, kode) == 0) {
            printf("Data with code %s found in the list.\n", kode);
            printf("School Name: %s\nTelephone: %s\nNumber of Students: %d\n", current->nama_sekolah, current->telepon, current->jumlah_siswa);
            return;
        }
        current = current->next;
    }
    printf("Data with code %s not found in the list.\n", kode);
}

void sortList(struct Node** head) {
    if (*head == NULL || (*head)->next == NULL) {
        return; // No need to sort if 0 or 1 element
    }

    struct Node* sortedList = NULL;

    while (*head != NULL) {
        struct Node* current = *head;
        *head = (*head)->next;

        if (sortedList == NULL || strcmp(current->kode, sortedList->kode) < 0) {
            current->next = sortedList;
            sortedList = current;
        } else {
            struct Node* temp = sortedList;
            while (temp->next != NULL && strcmp(temp->next->kode, current->kode) < 0) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
    }

    *head = sortedList;
}

void printList(struct Node* head) {
    while (head != NULL) {
        printf("Code: %s\nSchool Name: %s\nTelephone: %s\nNumber of Students: %d\n\n", head->kode, head->nama_sekolah, head->telepon, head->jumlah_siswa);
        head = head->next;
    }
}

void displayAll(struct Node* head) {
    if (head == NULL) {
        printf("No school data to display.\n");
        return;
    }

    printf("All School Data:\n");
    printList(head);
}

int main() {
    struct Node* head = NULL;

    char kode[50], nama_sekolah[100], telepon[20];
    int jumlah_siswa;
    char choice;

    do {
        printf("\nA. Add School Data\nS. Search School Data\nD. Sort School Data\nP. Display All School Data\nQ. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (tolower(choice)) {
            case 'a':
                printf("Enter school code: ");
                scanf("%s", kode);
                printf("Enter school name: ");
                scanf(" %[^\n]s", nama_sekolah);
                printf("Enter telephone: ");
                scanf("%s", telepon);
                printf("Enter number of students: ");
                scanf("%d", &jumlah_siswa);
                head = addAndSave(head, kode, nama_sekolah, telepon, jumlah_siswa, "data.csv");
                break;
            case 's':
                printf("Enter school code to search: ");
                scanf("%s", kode);
                searchNode(head, kode);
                break;
            case 'd':
                sortList(&head);
                printf("\nSchool Data After Sorting:\n");
                printList(head);
                break;
            case 'p':
                displayAll(head);
                break;
            case 'q':
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (tolower(choice) != 'q');

    return 0;
}
