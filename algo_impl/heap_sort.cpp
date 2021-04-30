#include <iostream>

void swap(int* tree, int i, int j) {
    int tmp = tree[i];
    tree[i] = tree[j];
    tree[j] = tmp;
}

int left(int i) {
    return i * 2 + 1;
}

int right(int i) {
    return i * 2 + 2;
}

void heapify (int* tree, int size, int root) {
    if (root >= size) {
        return;
    }
    heapify(tree, size, left(root));
    heapify(tree, size, right(root));
    if (left(root) < size && tree[root] < tree[left(root)]) {
        swap(tree, root, left(root));
        heapify(tree, size, left(root));
    }
    if (right(root) < size && tree[root] < tree[right(root)]) {
        swap(tree, root, right(root));
        heapify(tree, size, right(root));
    }
}

void heap_sort (int* tree, int size) {
    while (size > 0) {
        heapify(tree, size, 0);
        swap(tree, 0, size - 1);
        size--;
    }
}

int main() {
    int heap[] = {1,4,5,62,5,7,8,5,4,3,3,5,67,8,89,9,99};
    int size = sizeof(heap) / sizeof(heap[0]);
    for(int i = 0; i < size; ++i) {
        std::cout << heap[i] << ",";
    } 
    std::cout << std::endl;
    heapify(heap, size, 0);
    for(int i = 0; i < size; ++i) {
        std::cout << heap[i] << ",";
    }
    std::cout << std::endl;
    heap_sort(heap, size);
    for(int i = 0; i < size; ++i) {
        std::cout << heap[i] << ",";
    }
    std::cout << std::endl;
    return 0;
}
