/*
    Name: Nilesh Rajiv Auradkar
    B-number: B01044448
*/

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

// Define infinity (i.e max int range in c++) for distance comparisons
const int INF = std::numeric_limits<int>::max();

//Edge Structure representing a weighted connection between vertices
struct Edge
{
    int to;
    int weight;

    Edge(int edge_to, int edge_weight) : to(edge_to), weight(edge_weight)
    {

    }
};

// Node structure for the priority queue
struct Node
{
    int vertex;
    int distance;


    Node(int vertex_label, int vertex_distance) : vertex(vertex_label), distance(vertex_distance)
    {

    }
};

// Custom min-heap for O(log n) vertex extraction and distance updates
class Heap
{
    private:
        std::vector<Node> heap;
        std::vector<int> position;

        // Restores the min-heap property by moving a node up the heap
        // Used when a node's priority decreases (distance becomes smaller)
        void heapify_Up(int i)
        {
            while(i > 0)
            {
                int parent = (i - 1) / 2;
                if(heap[i].distance >= heap[parent].distance)
                    break;
                std::swap(heap[i], heap[parent]);
                position[heap[i].vertex] = i;
                position[heap[parent].vertex] = parent;
                
                i = parent;
            }
        }

        // Restores the min-heap property by moving a node down the heap
        // Used after extracting the minimum element or when a node's priority increases
        void heapify_Down(int i)
        {
            int heap_size = heap.size();


            while(true)
            {
                int smallest_ele = i;
                int left_vertex = 2 * i + 1;
                int right_vertex = 2 * i + 2;
                if(left_vertex < heap_size && heap[left_vertex].distance < heap[smallest_ele].distance)
                    smallest_ele = left_vertex;
                
                if(right_vertex < heap_size && heap[right_vertex].distance < heap[smallest_ele].distance)
                    smallest_ele = right_vertex;

                if(smallest_ele == i)
                    break;

                std::swap(heap[i], heap[smallest_ele]);
                position[heap[i].vertex] = i;
                position[heap[smallest_ele].vertex] = smallest_ele;

                i = smallest_ele;
            }
        }

    public:
        Heap(int size) : position(size, - 1)
        {

        }

        // core heap operations push, pop, heap decrease key
        void push(const Node& node)
        {
            heap.push_back(node);
            position[node.vertex] = heap.size() - 1;
            heapify_Up(heap.size() - 1);
        }

        Node pop()
        {
            Node root = heap[0];
            heap[0] = heap.back();
            heap.pop_back();
            if(!heap.empty())
            {
                position[heap[0].vertex] = 0;
                heapify_Down(0);
            }

            position[root.vertex] = -1;
            return root;
        }

        // Updates the distance of a vertex when a shorter path is found and restores the heap property
        void heap_Decrease_Key(int vertex, int newDistance)
        {
            int i = position[vertex];
            if (i == -1) 
            {
                push(Node(vertex, newDistance));
            } 
            else if (newDistance < heap[i].distance) 
            {
                heap[i].distance = newDistance;
                heapify_Up(i);
            }
        }

        bool empty() const
        {
            return heap.empty();
        }

};


// constructing graph network with nodes and vertices
class Graph
{
    private:
        int vertices;
        std::vector<std::vector<Edge>> adjacency_List;

    public:
        Graph(int v) : vertices(v), adjacency_List(v)
        {

        }

        // add directed edge between the vertex in graph
        void addEdge(int from, int to, int weight)
        {
            adjacency_List[from].emplace_back(to, weight);
        }

        // Finds the shortest path from start to end using Dijkstra's algorithm
        // Returns the shortest distance or -1 if no path exists
        int dijkstra_Algorithm(int start, int end)
        {
            std::vector<int> dist(vertices, INF);
            dist[start] = 0;

            Heap priority_queue(vertices);
            priority_queue.push(Node(start, 0));

            while(!priority_queue.empty())
            {
                Node current_vertex = priority_queue.pop();
                int vertex_label = current_vertex.vertex;
                int vertex_distance = current_vertex.distance;

                if(vertex_label == end)
                    return vertex_distance;
                
                if(vertex_distance > dist[vertex_label])
                    continue;

                for(const Edge& edge : adjacency_List[vertex_label])
                {
                    int edge_to = edge.to;
                    int edge_weight = edge.weight;

                    if(dist[vertex_label] + edge_weight < dist[edge_to])
                    {
                        dist[edge_to] = dist[vertex_label] + edge_weight;
                        priority_queue.heap_Decrease_Key(edge_to, dist[edge_to]);
                    }
                }
            }

            return -1;
        }
};

int main(int argc, char *argv[])
{

    // check for number of arguments passed through command line
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " [start vertex] [end_vertex] < [graph_file_name.txt]" << std::endl;
        return 1;
    }

    int start_vertex = std::atoi(argv[1]);
    int end_vertex = std::atoi(argv[2]);

    // Read number of vertex and number of edges from file using standard input
    int n_vertex, n_edges;
    std::cin >> n_vertex >> n_edges;

    // validate start vertex and end vertex are within range
    if(start_vertex < 0 || start_vertex > n_vertex || end_vertex < 0 || end_vertex > n_vertex)
    {
        std::cerr << "Start index or end index out of range. Total vertex: " << n_vertex << std::endl;
        return 1;
    }

    Graph graph(n_vertex);

    // Construct graph from input
    for(int i = 0; i < n_edges; ++i)
    {
        int vertex1, vertex2, weight;
        std::cin >> vertex1 >> vertex2 >> weight;
        graph.addEdge(vertex1, vertex2, weight);
    }


    // calling dijkstra's algorithm function and finding shortest path between start and end vertex
    int result = graph.dijkstra_Algorithm(start_vertex, end_vertex);

    if(result == -1)
    {
        std::cout << "not connected" << std::endl;
    }
    else
    {
        std::cout << result << std::endl;
    }

    return 0;
}