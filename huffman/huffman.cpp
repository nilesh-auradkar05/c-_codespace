/*
    Name: Nilesh Rajiv Auradkar
    B-number: B01044448
*/

#include <iostream>
#include <vector>
#include <algorithm>

// Node structure for Huffman tree
struct Node 
{
    int frequency;
    char letter;
    Node* left;
    Node* right;
    
    // Constructor for creating a new node
    Node(int freq, char letter = '\0', Node* l = nullptr, Node* r = nullptr) : frequency(freq), letter(letter), left(l), right(r)
    {

    }
    
    // Check if the node is a leaf (has no children)
    bool isLeaf() const 
    { 
        return left == nullptr && right == nullptr; 
    }
};

// Custom comparator for the min-heap
struct CompareNode 
{
    bool operator()(const Node* l, const Node* r) const 
    {
        return l->frequency > r->frequency;
    }
};

// Binary Heap (Priority Queue) implementation
class BinaryHeap 
{
    private:
        std::vector<Node*> heap;

        // Maintain heap property by moving an element up
        void heapifyUp(int index) 
        {
            while (index > 0) 
            {
                int parent = (index - 1) / 2;
                if (heap[parent]->frequency <= heap[index]->frequency) break;
                std::swap(heap[parent], heap[index]);
                index = parent;
            }
        }

        // Maintain heap property by moving an element down
        void heapifyDown(int index) 
        {
            int size = heap.size();
            while (true) 
            {
                int smallest = index;
                int left = 2 * index + 1;
                int right = 2 * index + 2;

                if (left < size && heap[left]->frequency < heap[smallest]->frequency)
                    smallest = left;
                if (right < size && heap[right]->frequency < heap[smallest]->frequency)
                    smallest = right;

                if (smallest == index) break;

                std::swap(heap[index], heap[smallest]);
                index = smallest;
            }
        }

    public:

        // Add a new node to the heap
        void push(Node* node) 
        {
            heap.push_back(node);
            heapifyUp(heap.size() - 1);
        }

        // Remove and return the top (smallest) element
        Node* pop() 
        {
            if (heap.empty())
                return nullptr;
            
            Node* top = heap[0];
            heap[0] = heap.back();
            heap.pop_back();
            
            if (!heap.empty())
                heapifyDown(0);
            
            return top;
        }

        // Check if the heap is empty
        bool empty() const 
        {
            return heap.empty();
        }

        // Get the current size of the heap
        // size_t is an unsigned integer type used for sizes and counts
        size_t size() const 
        {
            return heap.size();
        }
};

// Build the Huffman tree from frequency data
Node* buildHuffmanTree(const std::vector<int>& frequencies) 
{
    BinaryHeap pq;

    // Create leaf nodes for each character and add to priority queue
    for (int i = 0; i < frequencies.size(); ++i) 
    {
        if (frequencies[i] > 0) 
        {
            pq.push(new Node(frequencies[i], 'A' + i));
        }
    }

    // Build the tree by repeatedly combining the two nodes with lowest frequency
    while (pq.size() > 1) 
    {
        Node* left = pq.pop();
        Node* right = pq.pop();
        Node* parent = new Node(left->frequency + right->frequency, '\0', left, right);
        pq.push(parent);
    }

    return pq.pop();  // Return the root of the Huffman tree
}

// Function to calculate the total number of bits required
int calculateTotalBits(Node* root, int depth = 0) 
{
    if (root == nullptr)
        return 0;
    if (root->isLeaf())
        return root->frequency * std::max(1, depth);
    
    return calculateTotalBits(root->left, depth + 1) + calculateTotalBits(root->right, depth + 1);
}

// Free the memory allocated for the Huffman tree
void freeTree(Node* root) 
{
    if (root == nullptr)
        return;
    
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() 
{
    int n;
    std::cin >> n;

    std::vector<int> frequencies(n);
    for (int i = 0; i < n; ++i) 
    {
        std::cin >> frequencies[i];
    }

    Node* root = buildHuffmanTree(frequencies);
    int totalBits = calculateTotalBits(root);

    std::cout << totalBits << std::endl;

    // Clean up memory for the node.
    freeTree(root);
    return 0;
}