#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* generateRandomArray(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100; // Generate random numbers between 0 and 99
    }

    return arr;
}


void bucketSort(int* arr, int n, int num_buckets) {
    int max_value = 100; // Maximum possible value in the array
    int* buckets[num_buckets];
    int bucket_sizes[num_buckets];

    // Initialize buckets and bucket sizes
    for (int i = 0; i < num_buckets; i++) {
        buckets[i] = (int*)malloc(n * sizeof(int));
        bucket_sizes[i] = 0;
    }

    // Assign elements to buckets
    #pragma omp parallel for shared(arr, buckets, bucket_sizes) num_threads(2)
    for (int i = 0; i < n; i++) {
        int bucket_index = arr[i] / (max_value / num_buckets);
        
        #pragma omp critical
        {
            buckets[bucket_index][bucket_sizes[bucket_index]] = arr[i];
            bucket_sizes[bucket_index]++;
        }
    }

    // Sort elements within each bucket (using any sorting algorithm)
    for (int i = 0; i < num_buckets; i++) {
        // TODO: Implement sorting algorithm (e.g., insertion sort, quicksort, etc.)
    }

    // Concatenate elements from all buckets back into the original array
    int index = 0;
    for (int i = 0; i < num_buckets; i++) {
        for (int j = 0; j < bucket_sizes[i]; j++) {
            arr[index] = buckets[i][j];
            index++;
        }
        free(buckets[i]);
    }
}


int main() {
    int n = 10; // Number of array elements
    int num_buckets = 5; // Number of buckets
    int* arr = generateRandomArray(n);

    printf("Original Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    clock_t start = clock();
    bucketSort(arr, n, num_buckets);
    clock_t end = clock();
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Sorted Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Execution Time: %.6f seconds\n", execution_time);

    free(arr);
    return 0;
}
