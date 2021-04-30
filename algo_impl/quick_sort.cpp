#include <iostream>
#include <vector>

using namespace std;

int partition(vector<int>& a, int low, int high) {
    // 选取数组最低点作为pivot
    int pivot = a[low];
    while (low < high) {
        // 从右向左找到第一个比pivot小的数
        while (low < high && a[high] >= pivot) {
            --high;
        }
        // 将这个比pivot小的数放到左边来
        a[low] = a[high];
        // 从左向右找到第一个比pivot大的数
        while (low < high && a[low] <= pivot) {
            ++low;
        }
        // 将这个比pivot大的数放到右边来
        a[high] = a[low];
    }
    // 交换完了过后，将low的位置放上pivot
    // 这里其实强调了，low的左侧始终都放着小于等于pivot的数，包括一开始
    a[low] = pivot;
    return low;
}

void quick_sort(vector<int>& a, int low, int high) {
    if (low < high) {
        int pivot = partition(a, low, high);
        quick_sort(a, low, pivot - 1);
        quick_sort(a, pivot + 1, high);
    }
}

int main() {
    vector<int> tmp = {1,3,4,5,6,2,3,7,4,5,6,7,8,9,5,6,7,8,9};
    quick_sort(tmp, 0, tmp.size() - 1);
    for (auto it : tmp) {
        cout << it << ",";
    }
    cout << endl;
    return 0;
}
