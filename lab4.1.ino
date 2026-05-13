#include <Arduino.h>

// Функція для зміни частоти CPU (для ESP32)
void setCpuFreq(uint32_t mhz) {
    setCpuFrequencyMhz(mhz);
    Serial.printf("\n--- Поточна частота CPU: %d MHz ---\n", getCpuFrequencyMhz());
}

// QuickSort Template
template <typename T>
void quickSort(T arr[], int left, int right) {
    int i = left, j = right;
    T pivot = arr[(left + right) / 2];
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++; j--;
        }
    }
    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

// Структура вузла бінарного дерева
template <typename T>
struct Node {
    T data;
    Node *left, *right;
    Node(T val) : data(val), left(NULL), right(NULL) {}
};

// Вставка в дерево
template <typename T>
Node<T>* insert(Node<T>* root, T val) {
    if (!root) return new Node<T>(val);
    if (val < root->data) root->left = insert(root->left, val);
    else root->right = insert(root->right, val);
    return root;
}

// Очищення дерева (пам'ять)
template <typename T>
void deleteTree(Node<T>* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete(root);
    }
}

void runExperiment(int size) {
    int* intArr = new int[size];
    for(int i=0; i<size; i++) intArr[i] = random(0, 10000);

    uint32_t start = micros();
    quickSort(intArr, 0, size - 1);
    uint32_t end = micros();
    
    Serial.printf("QuickSort (int, size %d): %u us\n", size, end - start);
    
    Node<int>* root = NULL;
    start = micros();
    for(int i=0; i<size; i++) root = insert(root, intArr[i]);
    end = micros();
    
    Serial.printf("Binary Tree build (int, size %d): %u us\n", size, end - start);
    
    deleteTree(root);
    delete[] intArr;
}

void setup() {
    Serial.begin(115200);
    uint32_t freqs[] = {240, 160, 80, 40};
    int sizes[] = {50, 100, 500, 1000};

    for (uint32_t f : freqs) {
        setCpuFreq(f);
        for (int s : sizes) {
            runExperiment(s);
        }
    }
}

void loop() {}