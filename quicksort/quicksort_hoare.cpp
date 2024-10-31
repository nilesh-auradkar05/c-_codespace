/*
    Name: Nilesh Rajiv Auradkar
    B-number: B01044448
*/

#include <random>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>


void printArray(const std::vector<int>& arr, int start_idx, int end_idx) {

    /*
        Description:
            Prints the array from start_idx to end_idx provided in arguments else prints entire array.

        Returns:
            Void
    */

    for(int i=start_idx; i<end_idx; i++){
        std::cout << std::setprecision(2) << std::fixed << arr[i] << std::endl;
    }
    std::cout << std::endl;
}

struct ReadFileReturnValue {

    /*
        Description:
            Structure to encapsulate the return values of readFile function.
    */

    std::vector<int> arr;
    int n;
};

ReadFileReturnValue readFile() {

    /*
        Description:
            Reads input from stdin: first line as count, subsequent lines as elements of the array.

        Returns:
            array: array of int data type
            n: number of elements in the array
    */

    int n = 0;
    std::vector<int> arr;

    std::cin >> n;
    std::cout << "n: " << n << std::endl;
    
    for (int i = 0; i < n; ++i) {
        int num;
        if (std::cin >> num) {
            arr.push_back(num);
        } else {
            // Handle input error
            std::cerr << "Error reading input at position " << i << std::endl;
            break;
        }
    }

    printArray(arr, 0, n);
    return {arr, n};
}

int random_number_generator(int low, int high){

    /*
        Description:
            Generates a random number between low and high.

        Returns:
            num: (int) - random index number between low and high.
    */

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(low, high+1);
    int num = distr(gen);
    return num;
}


int partition(std::vector<int>& arr, int low, int high){

    /*
        Description:
            - This function chooses a pivot randomly and partitions the array segment [low, high]
                such that elements smaller than the pivot are on the left side and larger elements
                are on the right side. The pivot itself can be anywhere in the partitioned array.
            - Uses random pivot selection to avoid worst-case scenarios on already sorted arrays.
            - Implements two-way partitioning for better performance on arrays with many duplicates.
            - The returned index is used to divide the array for subsequent recursive calls.

        Args:
            arr: (vector<int>) - array of int data type
            low: (int) - lower bound of the array
            high: (int) - upper bound of the array

        Returns:
            j: (int) - index where the partition ends (last index of the left subarray).
    */

    if (low >= high) {
        return low;
    }

    int pivotIndex = random_number_generator(low, high);
    int pivot = arr[pivotIndex];

	int i = low-1;
	int j = high+1;

	while (true) {

		do{
			i++;
		} while(arr[i]<pivot);

		do{
			j--;
		} while(arr[j]>pivot);

		if(i>=j){
			return j;
		}

		std::swap(arr[i], arr[j]);
	}
}

void quickSort(std::vector<int>& arr, int low, int high){

    /*
        Description:
            - This function recursively divides the array into smaller subarrays, partitions them, and
                sorts them.
            - The algorithm works as follows:
                1. Partition the subarray.
                2. Recursively sort the left subarray.
                3. Recursively sort the right subarray.
            - The base case low < high (subarrays less that 0 or 1 element) requires no sorting.

        Returns:
            Void
    */

    if(low<high){
        
    	int pi = partition(arr, low, high);
    	
        // Recursively call quickSort for the left subarray.
        quickSort(arr, low, pi);

        // Recursively call quickSort for the right subarray.
    	quickSort(arr, pi+1, high);

    }
}

int main(int argc, char *argv[]){

    /*
        Description:
            Main function parse start and end index from stdin, sort the array using 
            quickSort function and print the sorted array in the given range.

        Args:
            argc: (int) - number of arguments passed to the program
            argv: (char *[]) - array of arguments passed to the program

        Returns:
            int: 0
    */

   
    ReadFileReturnValue readFileReturnValue = readFile();
    int n = readFileReturnValue.n;
    std::vector<int> arr = readFileReturnValue.arr;

    int start_idx = 0;
    int end_idx = n;
    if (argc > 1) {
        
        // choosing the max of 0 and the start index provided as argument to handle negative index case.
        start_idx = std::max(0, std::stoi(std::string(argv[1])) - 1);
    }
    if (argc > 2) {
        end_idx = std::atoi(argv[2])-1;
    }

    if (start_idx < 0 || end_idx > n) {
        std::cout << "Invalid start or end index to print subset of sorted array!" << std::endl;
        return 0;
    }
        
    quickSort(arr, 0, n-1);

    std::cout << "Sorted array is: \n";
    std::cout << end_idx-start_idx << std::endl;
    printArray(arr, start_idx, end_idx);

    return 0;
}
