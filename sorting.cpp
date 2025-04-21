#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;

// Merge function
void merge(vector<int>& a, int i1, int j1, int i2, int j2) {
    //int size = j2 - i1 + 1;
    vector<int> temp(1000);
    int i = i1, j = i2, k = 0;

    while (i <= j1 && j <= j2) {
        if (a[i] < a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= j1)
        temp[k++] = a[i++];
    while (j <= j2)
        temp[k++] = a[j++];

    for (i = i1, k = 0; i <= j2; i++, k++)
        a[i] = temp[k];
}

// MergeSort (Parallel-friendly)
void mergesort(vector<int>& a, int i, int j) {
    if (i < j) {
        int mid = (i + j) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            mergesort(a, i, mid);

#pragma omp section
            mergesort(a, mid + 1, j);
        }

        merge(a, i, mid, mid + 1, j);
    }
}

// Sequential Bubble Sort
void bubbleSort(vector<int>& arr) {
    bool swapped;
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// Parallel Bubble Sort (Odd-Even Transposition Sort)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        int start = i % 2;
#pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {
    omp_set_num_threads(4);

    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    while (true) {
        int choice;
        cout << "\n----- Menu -----\n";
        cout << "1. Merge Sort\n";
        cout << "2. Bubble Sort\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 3) break;

        vector<int> arr_seq(arr), arr_par(arr);

        double start_time, end_time;

        if (choice == 1) {
            // Sequential Merge Sort
            start_time = omp_get_wtime();
            mergesort(arr_seq, 0, n - 1);
            end_time = omp_get_wtime();
            cout << "\nSequential Merge Sort Time: " << (end_time - start_time) << " seconds\n";

            // Parallel Merge Sort
            start_time = omp_get_wtime();
#pragma omp parallel
            {
#pragma omp single
                mergesort(arr_par, 0, n - 1);
            }
            end_time = omp_get_wtime();
            cout << "Parallel Merge Sort Time: " << (end_time - start_time) << " seconds\n";

        }
        else if (choice == 2) {
            // Sequential Bubble Sort
            start_time = omp_get_wtime();
            bubbleSort(arr_seq);
            end_time = omp_get_wtime();
            cout << "\nSequential Bubble Sort Time: " << (end_time - start_time) << " seconds\n";

            // Parallel Bubble Sort
            start_time = omp_get_wtime();
            parallelBubbleSort(arr_par);
            end_time = omp_get_wtime();
            cout << "Parallel Bubble Sort Time: " << (end_time - start_time) << " seconds\n";

        }
        else {
            cout << "Please enter a valid option.\n";
        }
    }

    return 0;
}
