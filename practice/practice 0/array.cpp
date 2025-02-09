#include <iostream>

using namespace std;

int* resizeArray(int* arr, int currentSize, int newSize) {
    // Create new array with new size
    int* newArr = new int[newSize];
    
    // Copy elements from old array
    // Only copy up to smaller of current or new size to avoid overflow
    int elementsToCopy = (currentSize < newSize) ? currentSize : newSize;
    for (int i = 0; i < elementsToCopy; i++) {
        newArr[i] = arr[i];
    }
    
    // Delete old array
    delete[] arr;
    
    return newArr;
}

int main() {
    // Create initial array
    int size = 5;
    int* numbers = new int[size];
    
    // Initialize with some values
    for (int i = 0; i < size; i++) {
        numbers[i] = i + 1;
    }
    
    // Print original array
    cout << "Original array: ";
    for (int i = 0; i < size; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;
    
    // Resize to larger size
    int newSize = 8;
    numbers = resizeArray(numbers, size, newSize);
    
    // Add some new values
    for (int i = size; i < newSize; i++) {
        numbers[i] = i + 1;
    }
    
    // Print resized array
    cout << "Resized array: ";
    for (int i = 0; i < newSize; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;
    
    // Clean up
    delete[] numbers;
    
    return 0;
}
