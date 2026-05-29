#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <stack>

using namespace std;

// Insertion Sort
void insertionSort(int* data, int size) {
    for (int i = 1; i < size; ++i) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            --j;
        }
        data[j + 1] = key;
    }
}

// Merge function used in Iterative version of Merge Sort
void merge(int* data, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = data[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = data[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            data[k++] = L[i++];
        }
        else {
            data[k++] = R[j++];
        }
    }

    while (i < n1) data[k++] = L[i++];
    while (j < n2) data[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Iterative version of Merge Sort
void mergeSort(int* data, int size) {
    for (int width = 1; width < size; width *= 2) {
        for (int i = 0; i < size; i += 2 * width) {
            int left = i;
            int mid = min(i + width, size);
            int right = min(i + 2 * width, size);
            merge(data, left, mid - 1, right - 1);
        }
    }
}

// Partition function used in Quick Sort
int partition(int* data, int low, int high) {
    int pivot = data[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (data[j] <= pivot) {
            ++i;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[high]);
    return i + 1;
}

// Iterati version of Quick Sort 
void quickSort(int* data, int size) {
    stack<pair<int, int>> s;
    s.push(make_pair(0, size - 1));

    while (!s.empty()) {
        int low = s.top().first;
        int high = s.top().second;
        s.pop();

        if (low < high) {
            int pi = partition(data, low, high);
            s.push(make_pair(low, pi - 1));
            s.push(make_pair(pi + 1, high));
        }
    }
}

// Counting Sort (only for positive integers in range 1..1000)
void countingSort(int* data, int size) {
    const int MAX_VAL = 1000;
    int* count = new int[MAX_VAL + 1]();
    int* output = new int[size];

    for (int i = 0; i < size; ++i) {
        ++count[data[i]];
    }

    int index = 0;
    for (int i = 1; i <= MAX_VAL; ++i) {
        while (count[i]-- > 0) {
            output[index++] = i;
        }
    }

    for (int i = 0; i < size; ++i)
        data[i] = output[i];

    delete[] count;
    delete[] output;
}

// Generate random data in range [1, 1000]
void generateRandomData(int* data, int size) {
    for (int i = 0; i < size; ++i) {
        data[i] = rand() % 1000 + 1;
    }
}

// Generate sorted data
void generateSortedData(int* data, int size) {
    generateRandomData(data, size);
    sort(data, data + size);
}

// Generate reverse sorted data
void generateReverseSortedData(int* data, int size) {
    generateSortedData(data, size);
    reverse(data, data + size);
}

// Function to test performance of a sorting algorithm
void testSortPerformance(void (*sortFunc)(int*, int), const string& sortName, const string& dataType, int* originalData, int size) {
    int* data = new int[size];
    copy(originalData, originalData + size, data);

    auto start = chrono::high_resolution_clock::now();
    sortFunc(data, size);
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << sortName << "\t" << duration << " micro_second" << endl;

    delete[] data;
}

int main() {
    srand(time(0));
    int sizes[6];

    
    for (int i = 0; i < 6; ++i) {
        cout << "Enter the size of set #" << i + 1 << ": ";
        cin >> sizes[i];
    }

    

    for (int i = 0; i < 6; ++i) {
        int size = sizes[i];
        int* data = new int[size];

        cout << "\n--- Testing for Size: " << size << " ---\n";

        cout << "\n< Testing for Random Data >\n";
        cout << endl;
        generateRandomData(data, size);
        testSortPerformance(insertionSort, "Insertion Sort", "Random", data, size);
        testSortPerformance(mergeSort, "Merge Sort", "Random", data, size);
        testSortPerformance(quickSort, "Quick Sort", "Random", data, size);
        testSortPerformance(countingSort, "Counting Sort", "Random", data, size);
        cout << "\n< Testing for Sorted Data >\n";
        cout << endl;
        generateSortedData(data, size);
        testSortPerformance(insertionSort, "Insertion Sort", "Sorted", data, size);
        testSortPerformance(mergeSort, "Merge Sort", "Sorted", data, size);
        testSortPerformance(quickSort, "Quick Sort", "Sorted", data, size);
        testSortPerformance(countingSort, "Counting Sort", "Sorted", data, size);
        cout << "\n< Testing for Reverse sorted Data >\n";
        cout << endl;
        generateReverseSortedData(data, size);
        testSortPerformance(insertionSort, "Insertion Sort", "Reverse", data, size);
        testSortPerformance(mergeSort, "Merge Sort", "Reverse", data, size);
        testSortPerformance(quickSort, "Quick Sort", "Reverse", data, size);
        testSortPerformance(countingSort, "Counting Sort", "Reverse", data, size);

        cout << "--------------------------------------------------------------------------\n";
        delete[] data;
    }

    return 0;
}