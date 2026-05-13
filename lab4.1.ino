#include <Arduino.h>

// Change CPU Frequency: 240, 160, 80, 40
void setFrequency(uint32_t mhz) {
    setCpuFrequencyMhz(mhz);
    Serial.print("\n--- Frequency set to: ");
    Serial.print(getCpuFrequencyMhz());
    Serial.println(" MHz ---");
}

// QuickSort Implementation
template <typename T>
void quickSort(T arr[], int left, int right) {
    int i = left, j = right;
    T pivot = arr[(left + right) / 2];
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++; j--;
        }
    }
    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

// Binary Tree Logic
struct Node {
    int data;
    Node *left, *right;
    Node(int val) : data(val), left(NULL), right(NULL) {}
};

Node* insert(Node* root, int val) {
    if (!root) return new Node(val);
    if (val < root->data) root->left = insert(root->left, val);
    else root->right = insert(root->right, val);
    return root;
}

void deleteTree(Node* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

void setup() {
    Serial.begin(115200);
    int sizes[] = {50, 100, 500, 1000};
    
    setFrequency(240); // Repeat for 160, 80, 40

    for (int size : sizes) {
        int* arr = new int[size];
        for(int i=0; i<size; i++) arr[i] = random(0, 5000);

        uint32_t start = micros();
        quickSort(arr, 0, size - 1);
        uint32_t end = micros();

        Serial.printf("Size %d | QuickSort Time: %u us\n", size, end - start);
        
        Node* root = NULL;
        start = micros();
        for(int i=0; i<size; i++) root = insert(root, arr[i]);
        end = micros();
        Serial.printf("Size %d | Tree Build Time: %u us\n", size, end - start);

        deleteTree(root);
        delete[] arr;
    }
}

void loop() {}
