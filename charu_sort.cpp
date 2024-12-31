#include <iostream>
#include <vector>

void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i]; // 当前要插入的元素
        int j = i - 1;

        // 将比 key 大的元素向后移动，为 key 留出位置
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key; // 将 key 插入到正确位置
    }
}

int main() {
    std::vector<int> arr = {12, 11, 13, 5, 6};
    
    std::cout << "排序前数组: ";
    for (int i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    insertionSort(arr);

    std::cout << "排序后数组: ";
    for (int i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
