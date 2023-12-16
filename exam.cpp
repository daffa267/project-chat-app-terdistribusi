#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

struct Node {
    string kode;
    string nama_sekolah;
    string telepon;
    int jumlah_siswa;
    Node* next;
};

Node* createNode(string kode, string nama_sekolah, string telepon, int jumlah_siswa) {
    Node* newNode = new Node;
    newNode->kode = kode;
    newNode->nama_sekolah = nama_sekolah;
    newNode->telepon = telepon;
    newNode->jumlah_siswa = jumlah_siswa;
    newNode->next = NULL;
    return newNode;
}

Node* addAndSave(Node* head, string kode, string nama_sekolah, string telepon, int jumlah_siswa, const char* filename) {
    Node* newNode = createNode(kode, nama_sekolah, telepon, jumlah_siswa);
    newNode->next = head;

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        delete newNode;
        return head;
    }

    file << kode << "," << nama_sekolah << "," << telepon << "," << jumlah_siswa << "\n";
    file.close();

    cout << "Data added and saved to " << filename << endl;
    return newNode;
}

void searchNode(Node* head, string kode) {
    Node* current = head;
    while (current != NULL) {
        if (current->kode == kode) {
            cout << "Data with code " << kode << " found in the list." << endl;
            cout << "School Name: " << current->nama_sekolah << "\nTelephone: " << current->telepon << "\nNumber of Students: " << current->jumlah_siswa << endl;
            return;
        }
        current = current->next;
    }
    cout << "Data with code " << kode << " not found in the list." << endl;
}

void sortList(Node*& head) {
    if (head == NULL || head->next == NULL) {
        return; // No need to sort if 0 or 1 element
    }

    Node* sortedList = NULL;

    while (head != NULL) {
        Node* current = head;
        head = head->next;

        if (sortedList == NULL || current->kode < sortedList->kode) {
            current->next = sortedList;
            sortedList = current;
        } else {
            Node* temp = sortedList;
            while (temp->next != NULL && temp->next->kode < current->kode) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
    }

    head = sortedList;
}

void printList(Node* head) {
    while (head != NULL) {
        cout << "Code: " << head->kode << "\nSchool Name: " << head->nama_sekolah << "\nTelephone: " << head->telepon << "\nNumber of Students: " << head->jumlah_siswa << endl << endl;
        head = head->next;
    }
}

void displayAll(Node* head) {
    if (head == NULL) {
        cout << "No school data to display." << endl;
        return;
    }

    cout << "All School Data:" << endl;
    printList(head);
}

int main() {
    Node* head = NULL;

    string kode, nama_sekolah, telepon;
    int jumlah_siswa;
    int choice;

    do {
        cout << "\n1. Add School Data\n2. Search School Data\n3. Sort School Data\n4. Display All School Data\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter school code: ";
                cin >> kode;
                cout << "Enter school name: ";
                cin.ignore(); // To consume the newline character left by previous cin
                getline(cin, nama_sekolah);
                cout << "Enter telephone: ";
                cin >> telepon;
                cout << "Enter number of students: ";
                cin >> jumlah_siswa;
                head = addAndSave(head, kode, nama_sekolah, telepon, jumlah_siswa, "data.csv");
                break;
            case 2:
                cout << "Enter school code to search: ";
                cin >> kode;
                searchNode(head, kode);
                break;
            case 3:
                sortList(head);
                cout << "\nSchool Data After Sorting:" << endl;
                printList(head);
                break;
            case 4:
                displayAll(head);
                break;
            case 0:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

