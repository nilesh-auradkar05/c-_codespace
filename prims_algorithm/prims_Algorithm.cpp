/*
    Name: Nilesh Rajiv Auradkar
    B-number: B01044448
*/

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>

// Define infinity (i.e max int range in c++) for distances between vertices.
const int INF = std::numeric_limits<int>::max();

// Structure to represent an edge in the graph 
struct Edge 
{
    int to;
    int weight;

    Edge(int edge_to, int edge_weight) : to(edge_to), weight(edge_weight) {}
};

// Structure to represent a node in the priority queue
struct Node 
{
    int vertex;
    int distance;

    Node(int vertex_label, int vertex_distance) : vertex(vertex_label), distance(vertex_distance) {}
};

// MinHeap class for efficient priority queue operations
class MinHeap 
{
    private:
        std::vector<Node> heap;
        std::vector<int> position;

        // Move a node up the heap to maintain the heap property
        void heapify_up(int i) 
        {
            while (i > 0) 
            {
                int parent = (i - 1) / 2;
                if (heap[i].distance >= heap[parent].distance)
                    break;
                
                std::swap(heap[i], heap[parent]);
                position[heap[i].vertex] = i;
                position[heap[parent].vertex] = parent;
                
                i = parent;
            }
        }

        // Move a node down the heap to maintain the heap property
        void heapify_down(int i) 
        {
            int heap_size = heap.size();
            while (true) 
            {
                int smallest = i;
                int left = 2 * i + 1;
                int right = 2 * i + 2;
                
                if (left < heap_size && heap[left].distance < heap[smallest].distance)
                    smallest = left;
                
                if (right < heap_size && heap[right].distance < heap[smallest].distance)
                    smallest = right;
                
                if (smallest == i)
                    break;
                
                std::swap(heap[i], heap[smallest]);
                position[heap[i].vertex] = i;
                position[heap[smallest].vertex] = smallest;
                
                i = smallest;
            }
        }

    public:
        MinHeap(int size) : position(size, -1) 
        {}

        // Add a new node to the heap
        void push(const Node& node) 
        {
            heap.push_back(node);
            position[node.vertex] = heap.size() - 1;
            heapify_up(heap.size() - 1);
        }

        // Remove and return the node with the minimum distance
        Node pop() 
        {
            Node root = heap[0];
            heap[0] = heap.back();
            heap.pop_back();
            if (!heap.empty()) 
            {
                position[heap[0].vertex] = 0;
                heapify_down(0);
            }
            position[root.vertex] = -1;
            return root;
        }

        // Check if the heap is empty
        bool empty() const 
        {
            return heap.empty();
        }
};

// Graph class to represent the graph and implement Prim's algorithm
class Graph 
{
    private:
        int vertices;
        std::vector<std::vector<Edge>> adjacency_list;

    public:

        Graph(int v) : vertices(v), adjacency_list(v) 
        {}

        // Add an edge to the graph
        void add_edge(int from, int to, int weight) 
        {
            adjacency_list[from].emplace_back(to, weight);
            adjacency_list[to].emplace_back(from, weight); // for undirected graph
        }

        // Implementation of Prim's algorithm to find the Minimum Spanning Tree
        void prim_algorithm(int start) 
        {
            std::vector<bool> visited(vertices, false);
            std::vector<int> parent(vertices, -1);
            std::vector<int> distances_Vector(vertices, INF);

            // Create a min heap to efficiently get the minimum weight edge
            MinHeap priority_queue(vertices);

            // Start with the given vertex
            priority_queue.push(Node(start, 0));
            distances_Vector[start] = 0;

            // Main loop of Prim's algorithm
            while (!priority_queue.empty()) 
            {

                // Get the vertex with minimum distance from the priority queue
                Node current_vertex = priority_queue.pop();
                int vertex_label = current_vertex.vertex;

                // Mark the vertex as visited
                visited[vertex_label] = true;

                // Explore all adjacent vertices
                for (const Edge& edge : adjacency_list[vertex_label]) 
                {
                    int edge_to = edge.to;
                    int edge_weight = edge.weight;

                    // If the adjacent vertex is not visited and has a smaller weight
                    if (!visited[edge_to] && edge_weight < distances_Vector[edge_to]) 
                    {

                        // Update the parent, distance, and add to priority queue
                        parent[edge_to] = vertex_label;
                        distances_Vector[edge_to] = edge_weight;
                        priority_queue.push(Node(edge_to, distances_Vector[edge_to]));
                    }
                }
            }

            // Calculate and print the total weight of the Minimum Spanning Tree
            int total_tree_length = std::accumulate(distances_Vector.begin(), distances_Vector.end(), 0);
            std::cout << total_tree_length << std::endl;
        }
    };

    int main(int argc, char* argv[]) 
    {
        if (argc != 1) 
        {
            std::cerr << "Usage: " << argv[0] << " < [graph_file_name.txt]" << std::endl;
            return 1;
        }

        // Read the number of vertices and edges
        int n_vertices, n_edges;
        std::cin >> n_vertices >> n_edges;

        // Create the graph
        Graph graph(n_vertices);

        // Read and add edges to the graph
        for (int i = 0; i < n_edges; ++i) 
        {
            int vertex1, vertex2, weight;
            std::cin >> vertex1 >> vertex2 >> weight;
            graph.add_edge(vertex1, vertex2, weight);
        }

        // Run Prim's algorithm starting from vertex 0
        graph.prim_algorithm(0);

        return 0;
}