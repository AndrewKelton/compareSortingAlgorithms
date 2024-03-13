#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{for (int i = n / 2 - 1; i >= 0; i--) {
        int high = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

		// Go through left side
        while (left < n) {
            if (arr[left] > arr[high])
                high = left;

            if (right < n && arr[right] > arr[high])
                high = right;

            if (high != i) {
                // Swap and continue heapifying if necessary
                int tmp = arr[i];
                arr[i] = arr[high];
                arr[high] = tmp;

                i = high;
                left = 2 * i + 1;
                right = 2 * i + 2;
            } else {
                break;
            }
        }
    }

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Heapify the reduced heap
        int j = 0;
        int high = 0;
        int left = 1;
        int right = 2;

        while (left < i) {
            if (arr[left] > arr[high])
                high = left;

            if (right < i && arr[right] > arr[high])
                high = right;

            if (high != j) {
                // Swap and continue heapifying if necessary
                int tmp = arr[j];
                arr[j] = arr[high];
                arr[high] = tmp;

                j = high;
                left = 2 * j + 1;
                right = 2 * j + 2;
            } else {
                break;
            }
        }
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
		int m = l + (r-l) / 2;

		mergeSort(pData, l, m);

		mergeSort(pData, m + 1, r);

		int tmp[r-l+1];
		int i = l, j = m + 1, k = 0;

		while (i <= m && j <= r) {
            if (pData[i] <= pData[j])
                tmp[k++] = pData[i++];
            else
                tmp[k++] = pData[j++];
        }

        // Copy the remaining elements of left half
        while (i <= m)
            tmp[k++] = pData[i++];

        // Copy the remaining elements of right half
        while (j <= r)
            tmp[k++] = pData[j++];

        // Copy the merged elements back to original array
        for (i = l, k = 0; i <= r; i++, k++)
            pData[i] = tmp[k];
    }
}


// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int j, key;
	int sortedI = 0;
	for (int i = 1; i < n; i++) {
		key = pData[i];
		j = i - 1;
		while (j >= 0 && pData[j] > key) {
			pData[j + 1] = pData[j];
			j = j - 1;
		}
		pData[j + 1] = key;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{	
	int swapped = 0;
	for (int i = 0; i < n; i++) {
		for (int x = 0; x < n-i-1; x++) {
			if (pData[x] > pData[x+1]) {
				int tmp = pData[x];
				pData[x] = pData[x+1];
				pData[x+1] = tmp;
				swapped = 1;
			}
		}
		// check if array is already sorted
		if (swapped == 0) {
			break;
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	for (int i = 0; i < n; i++) {
        int low = i; 
        for (int x = low+1; x < n; x++) {
            // check if value is lower than low
            if (pData[x] < pData[low]) {
                low = x;
            }
        }
		if (low != i) {
			int tmp = pData[i];
			pData[i] = pData[low];
			pData[low] = tmp;
		}
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		int i = 0;
		while (fscanf(inFile, "%d", &((*ppData)[i])) != EOF) {
			i++;
		}
	}
	fclose(inFile);
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
}
