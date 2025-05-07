#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V;  // number of vertices
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        #pragma omp critical
        {
            adj[u].push_back(v);
            adj[v].push_back(u); // undirected graph
        }
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS starting from vertex " << start << ": ";

        while (!q.empty()) {
            int level_size = q.size();

            #pragma omp parallel for
            for (int i = 0; i < level_size; ++i) {
                int curr;
                
                #pragma omp critical
                {
                    if (!q.empty()) {
                        curr = q.front();
                        q.pop();
                        cout << curr << " ";
                    }
                }

                #pragma omp parallel for
                for (int j = 0; j < adj[curr].size(); ++j) {
                    int neighbor = adj[curr][j];
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    void parallelDFSUtil(int node, vector<bool>& visited) {
        stack<int> s;
        s.push(node);

        while (!s.empty()) {
            int curr = s.top();
            s.pop();

            if (!visited[curr]) {
                visited[curr] = true;
                cout << curr << " ";
            }

            #pragma omp parallel for
            for (int i = 0; i < adj[curr].size(); ++i) {
                int neighbor = adj[curr][i];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        s.push(neighbor);
                    }
                }
            }
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        cout << "Parallel DFS starting from vertex " << start << ": ";
        parallelDFSUtil(start, visited);
        cout << endl;
    }
};

int main() {
    int V = 6;
    Graph g(V);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    g.parallelBFS(0);
    g.parallelDFS(0);

    return 0;
}